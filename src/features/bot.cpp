#include "rbot.hpp"
#include "PlayLayer.hpp"
#include "UILayer.hpp"
#include "PlayerObject.hpp"
#include "tools.hpp"
#include "hooking.hpp"
#include "mod.hpp"
#include "PlayHitboxLayer.hpp"
#include "GameSoundManager.hpp"
#include "init.hpp"

void createPlaybackLabel()
{
    auto& m = rbot::getModules();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto label = CCLabelBMFont::create("Playing", "chatFont.fnt");
    label->setPosition(ccp(winSize.width - 15, 15));
    label->setAnchorPoint(ccp(1, .5f));
    m.playbackLabel = label;
    m.playbackLabel->retain();
}

void (*PlayLayer_update)(PlayLayer*, float);
void PlayLayer_update_H(PlayLayer* self, float dt)
{
    auto& modules = rbot::getModules();
    modules.paused = false;

    if(mod::module_by_id<bool>(id::frame_stepper))
    {
        modules.stepperMenu->setEnabled(true);
        modules.stepperMenu->setVisible(true);
    }
    else
    {
        modules.stepperMenu->setEnabled(false);
        modules.stepperMenu->setVisible(false);
        modules.stepperOn = false;
    }

    const float step = 1.0f / 240.0f;
    dt = step;
    if (modules.stepperOn && dt != step) {
        PlayLayer_update(self, 0); 
        return;
    }

    if (modules.stepperOn || (mod::module_by_id<bool>(id::update_on_steps) && modules.mode == Modes::kModeRecording)) dt = step;

    auto& data = rbot::getFrameData();

    if (modules.mode == Modes::kModePlaying)
    {
        modules.time += dt;

        if (data.empty())
        {
            rbot::stopMacro();
            PlayLayer_update(self, dt);
            return;
        }

        int index = 0;
        while (index < data.size() - 1 && data[index].frame < modules.time)
        {
            index++;
        }

        if (index >= data.size() - 1)
        {
            rbot::stopMacro();
            PlayLayer_update(self, dt);
            return;
        }

        auto frame = data[index];

        getPlayer(self)->setPosition(frame.position);
        getPlayer(self)->setLastP(frame.position);
        MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition) = frame.position;
        getPlayer(self)->setRotation(frame.rotation);
        getPlayer(self)->setScaleY(frame.flipY);

        if(mod::module_by_id<bool>(id::click_sounds) && frame.click)
        {
            auto sound = GameSoundManager::sharedManager();
            sound->playEffect(modules.soundPath.c_str(), 1, 0, 1);
        }
        
        modules.frame = index;
    }

    PlayLayer_update(self, dt);

    if (modules.mode == Modes::kModeRecording)
    {
        if (modules.completed) rbot::stopMacro();

        auto pos = MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition);
        if (MEMBER_BY_OFFSET(bool, self, PlayLayer__m_isFlipped)) // yes mirror portals exist in 1.0
        {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto center = winSize.width / 2;
            pos.x = (pos.x - center) + winSize.width - center;
        }
        
        modules.time += dt;
        rbot::getFrameData().push_back({
            modules.time,
            getPlayer(self)->getScaleY(),
            pos,
            getPlayer(self)->getRotation(),
            false,
            false,
            MEMBER_BY_OFFSET(bool, getPlayer(self), PlayerObject__m_onGround)
        });
        modules.frame++;
        modules.usedFrame = &rbot::getFrameData().back();
    }

    if(mod::module_by_id<bool>(id::playback_label) && modules.mode == kModePlaying)
    {
        modules.playbackLabel->setVisible(true);
    } else modules.playbackLabel->setVisible(false);
}

void (*PlayLayer_resetLevel)(PlayLayer*);
void PlayLayer_resetLevel_H(PlayLayer* self)
{
    auto& m = rbot::getModules();

    self->stopAllActions();
    if (!m.gameStarted)
    {
        m.gameStarted = true;
        MEMBER_BY_OFFSET(bool, self, PlayLayer__m_cleanReset) = true;
        getPlayer(self)->setVisible(true);
        self->scheduleUpdate();
    }

    m.frame = 0;
    m.time = 0;
    m.completed = false;
    m.playerHitboxes.clear();
    PlayLayer_resetLevel(self);
    if (m.mode == kModeRecording && !isPractice(self)) 
    {
        rbot::getFrameData().clear();
        rbot::getCheckpointData().clear();
    }
    
    if (m.mode == kModeRecording && isPractice(self) && !rbot::getCheckpointData().empty())
    {
        auto frame = m.time;
        auto& frames = rbot::getFrameData();

        frames.erase(
            std::remove_if(frames.begin(), frames.end(), [frame](const Frame& f)
                {
                    return f.frame >= frame; // TODO: fix some faulty frames still surviving the filter
                }
            ), 
        frames.end()
        );
    }

    if (m.mode == kModeRecording && isPractice(self) && rbot::getCheckpointData().empty())
    {
        rbot::getFrameData().clear();
    }
    self->toggleLayoutMode(LAYOUT_CHECK);
}

void (*PlayerObject_updateJump)(PlayerObject*, float);
void PlayerObject_updateJump_H(PlayerObject* self, float dt)
{
    auto m = rbot::getModules();

    if(m.mode != Modes::kModePlaying) PlayerObject_updateJump(self, dt);
    else
    {
        // fix ground particles
        auto frame = rbot::getFrameData()[m.frame];
        if(frame.onGround) MEMBER_BY_OFFSET(bool, self, PlayerObject__m_groundParticleOff) = false;
        else self->deactivateParticle();
    }
}

void (*PlayLayer_levelComplete)(PlayLayer*);
void PlayLayer_levelComplete_H(PlayLayer* self)
{
    PlayLayer_levelComplete(self);
    rbot::getModules().completed = true;
    rbot::stopMacro();
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
}

void (*PlayLayer_destroyPlayer)(PlayLayer*);
void PlayLayer_destroyPlayer_H(PlayLayer* self)
{
    if(mod::module_by_id<bool>(id::noclip))
    {
        MEMBER_BY_OFFSET(CCArray*, self, PlayLayer__m_hazards)->removeAllObjects();
    } else PlayLayer_destroyPlayer(self);
}

void (*PlayLayer_onQuit)(PlayLayer*);
void PlayLayer_onQuit_H(PlayLayer* self)
{
    PlayLayer_onQuit(self);
    rbot::reset();
    auto& m = rbot::getModules();
    m.playerHitboxes.clear();
    if(mod::module_by_id<bool>(id::playback_label) && m.playbackLabel)
    {
        m.playbackLabel->release();
        m.playbackLabel = nullptr;
    }
}

// force the game to run at 240 fps
void (*CCScheduler_update)(CCScheduler*, float);
void CCScheduler_update_H(CCScheduler* self, float dt)
{
    auto& modules = rbot::getModules();
    if (modules.stepperOn) return; 

    const double physics_step = 1.0 / 240.0;
    double speed_multiplier = 1.0;
    if (mod::module_by_id<bool>(id::speedhack) && !modules.paused)
    {
        speed_multiplier = static_cast<double>(mod::module_by_id<float>(id::speedhack_val));
    }

    static double accumulator = 0.0;
    accumulator += static_cast<double>(dt) * speed_multiplier;

    if (accumulator > physics_step * 10.0) accumulator = physics_step; 

    while (accumulator >= physics_step)
    {
        CCScheduler_update(self, static_cast<float>(physics_step));
        accumulator -= physics_step;
    }
}

bool (*UILayer_init)(UILayer*);
bool UILayer_init_H(UILayer* self)
{
    UILayer_init(self);
    auto& m = rbot::getModules();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if(!m.playbackLabel)
    {
        createPlaybackLabel();
    }
    if(!m.playbackLabel->getParent()) self->addChild(m.playbackLabel);
    if(!mod::module_by_id<bool>(id::playback_label) || m.mode != kModePlaying) m.playbackLabel->setVisible(false);

    auto menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    m.stepperMenu = menu;
    
    auto spr = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        spr,
        self,
        menu_selector(UILayer::onStepper)
    );
    btn->setPosition(ccp(winSize.width / 4, winSize.height - 20));
    menu->addChild(btn);

    #if GAME_VERSION > V1P4
    spr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    #else
    spr = CCSprite::create("GJ_deleteIcon_001.png");
    #endif
    btn = CCMenuItemSpriteExtra::create(
        spr,
        spr,
        self,
        menu_selector(UILayer::disableStepper)
    );
    btn->setPosition(ccp(winSize.width / 4 - 40, winSize.height - 20));
    menu->addChild(btn);

    self->addChild(menu);

    if (!mod::module_by_id<bool>(id::frame_stepper))
    {
        menu->setEnabled(false);
        menu->setVisible(false);
    }

    // im finishing this later im kinda tired :yawning_face:
    auto btnExt = CCMenuItemExt::createWithSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"),
        [self, m](CCObject*)
        {
            mod_map[id::speedhack] = !mod::module_by_id<bool>(id::speedhack);
        }
    );
    return true;
}

void UILayer::onStepper(CCObject*)
{
    auto& m = rbot::getModules();
    auto pl = GameManager::sharedState()->getPlayLayer();
    if (!pl) return;

    m.stepperOn = true;
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.0f);

    float delta = 1.0f / 240.0f;
    
    PlayLayer_update_H(pl, delta);
    CCDirector::sharedDirector()->getScheduler()->update(delta);
}

void UILayer::disableStepper(CCObject*)
{
    auto& m = rbot::getModules();
    m.stepperOn = false;
    
    if(mod::module_by_id<bool>(id::speedhack)) {
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(mod::module_by_id<float>(id::speedhack_val));
    } else {
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
    }
}

void (*PlayLayer_storeCheckpoint)(PlayLayer* self, void* checkpoint);
void PlayLayer_storeCheckpoint_H(PlayLayer* self, void* checkpoint)
{
    PlayLayer_storeCheckpoint(self, checkpoint);
    rbot::getCheckpointData().push_back({
        static_cast<double>(rbot::getModules().time), 
        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_yVelocity), 
        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_gravity),
        getPlayer(self)->getRotation()
    });
}

void (*PlayLayer_removeLastCheckpoint)(PlayLayer* self);
void PlayLayer_removeLastCheckpoint_H(PlayLayer* self)
{
    PlayLayer_removeLastCheckpoint(self);
    auto& checkpoints = rbot::getCheckpointData();
    if(!checkpoints.empty()) checkpoints.pop_back();
}

void (*PlayLayer_loadLastCheckpoint)(PlayLayer* self);
void PlayLayer_loadLastCheckpoint_H(PlayLayer* self)
{
    PlayLayer_loadLastCheckpoint(self);
    auto& checkpoints = rbot::getCheckpointData();
    if(!checkpoints.empty())
    {
        auto& modules = rbot::getModules();
        modules.time = checkpoints.back().frame;

        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_yVelocity) = checkpoints.back().yVelocity;
        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_gravity) = checkpoints.back().gravity;
        getPlayer(self)->setRotation(checkpoints.back().rotation);
    }
}

bool (*PlayLayer_init)(PlayLayer*, GJGameLevel*);
bool PlayLayer_init_H(PlayLayer* self, GJGameLevel* lvl)
{
    PlayLayer_init(self, lvl);

    rbot::getModules().gameStarted = false;
    
    MEMBER_BY_OFFSET(CCLayer*, self, PlayLayer__m_gameLayer)->addChild(PlayHitboxLayer::create(self), 9999);
    self->toggleLayoutMode(LAYOUT_CHECK);
    
    return true;
}

void (*PlayLayer_visit)(PlayLayer*);
void PlayLayer_visit_H(PlayLayer* self)
{
    PlayLayer_visit(self);
    auto gameLayer = MEMBER_BY_OFFSET(CCLayer*, self, PlayLayer__m_gameLayer);

    if (mod::module_by_id<bool>(id::show_hitboxes))
    {
        kmGLPushMatrix();

        gameLayer->transform(); 
        drawHitboxes(self);
        
        kmGLPopMatrix();
    }
}

void (*PlayLayer_toggleFlipped)(PlayLayer*, bool, bool);
void PlayLayer_toggleFlipped_H(PlayLayer* self, bool direction, bool doFlip)
{
    if(!mod::module_by_id<bool>(id::disable_mirror_portals)) PlayLayer_toggleFlipped(self, direction, doFlip);
}

bool (*UILayer_ccTouchBegan)(UILayer*, CCTouch*, CCEvent*);
bool UILayer_ccTouchBegan_H(UILayer* self, CCTouch* touch, CCEvent* event)
{
    UILayer_ccTouchBegan(self, touch, event);
    auto m = rbot::getModules();
    if(m.mode == Modes::kModeRecording)
    {
        m.usedFrame->click = true;
    }

    if(m.mode != Modes::kModePlaying && mod::module_by_id<bool>(id::click_sounds))
    {
        auto sound = GameSoundManager::sharedManager();
        sound->playEffect(m.soundPath.c_str(), 1, 0, 1);
    }
    return true;
}

void PlayLayer::toggleLayoutMode(bool enabled)
{
    auto objects = MEMBER_BY_OFFSET(CCArray*, this, PlayLayer__m_objects);

    for(int i = 0 ; i < objects->count() ; i++)
    {
        auto section = (CCArray*)objects->objectAtIndex(i);
        for(int j = 0 ; j < section->count() ; j++)
        {
            auto obj = (GameObject*)section->objectAtIndex(j);
            int type = MEMBER_BY_OFFSET(int, obj, GameObject__m_type);
            if(type == 7 || type == 8) obj->setVisible(!enabled);
        }
    }

    if(enabled)
    {
        this->tintBackground(ccc3(0,102,255), 0);
        this->tintGround(ccc3(0,102,255), 0);

        #if GAME_VERSION > V1P3
        this->tintObjects(ccc3(255,255,255), 0);
        this->tintLine(ccc3(255,255,255), 0);
        #endif
    
        #if GAME_VERSION > V1P6
        this->tintColorObjects(ccc3(255,255,255), 0);
        #endif
    }
}

void (*PlayLayer_updateLevelColors)(PlayLayer*);
void PlayLayer_updateLevelColors_H(PlayLayer* self) {
    if(!LAYOUT_CHECK) PlayLayer_updateLevelColors(self);
}

void (*PlayLayer_tintBackground)(PlayLayer*, _ccColor3B color, float len);
void PlayLayer_tintBackground_H(PlayLayer* self, _ccColor3B color, float len) {
    if(!LAYOUT_CHECK) PlayLayer_tintBackground(self, color, len);
}

void (*PlayLayer_tintGround)(PlayLayer*, _ccColor3B color, float len);
void PlayLayer_tintGround_H(PlayLayer* self, _ccColor3B color, float len) {
    if(!LAYOUT_CHECK) PlayLayer_tintGround(self, color, len);
}

void (*PlayLayer_tintLine)(PlayLayer*, _ccColor3B color, float len);
void PlayLayer_tintLine_H(PlayLayer* self, _ccColor3B color, float len) {
    if(!LAYOUT_CHECK) PlayLayer_tintLine(self, color, len);
}

void (*PlayLayer_tintObjects)(PlayLayer*, _ccColor3B color, float len);
void PlayLayer_tintObjects_H(PlayLayer* self, _ccColor3B color, float len) {
    if(!LAYOUT_CHECK) PlayLayer_tintObjects(self, color, len);
}

void (*PlayLayer_tintColorObjects)(PlayLayer*, _ccColor3B color, float len);
void PlayLayer_tintColorObjects_H(PlayLayer* self, _ccColor3B color, float len) {
    if(!LAYOUT_CHECK) PlayLayer_tintColorObjects(self, color, len);
}

void (*PlayerObject_playerDestroyed)(PlayerObject*);
void PlayerObject_playerDestroyed_H(PlayerObject* self) {
  if(!mod::module_by_id<bool>(id::no_death_effect)) PlayerObject_playerDestroyed(self);
}

void (*PlayLayer_resume)(PlayLayer*);
void PlayLayer_resume_H(PlayLayer* self)
{
    PlayLayer_resume(self);
    self->toggleLayoutMode(LAYOUT_CHECK);
}

void bot_hook()
{
    HOOK("_ZN9PlayLayer6resumeEv", PlayLayer_resume_H, PlayLayer_resume);
    HOOK("_ZN12PlayerObject15playerDestroyedEv", PlayerObject_playerDestroyed_H, PlayerObject_playerDestroyed);
    HOOK("_ZN9PlayLayer16tintColorObjectsEN7cocos2d10_ccColor3BEf", PlayLayer_tintColorObjects_H, PlayLayer_tintColorObjects); // 1.7
    HOOK("_ZN9PlayLayer17updateLevelColorsEv", PlayLayer_updateLevelColors_H, PlayLayer_updateLevelColors); // 1.6
    HOOK("_ZN9PlayLayer11tintObjectsEN7cocos2d10_ccColor3BEf", PlayLayer_tintObjects_H, PlayLayer_tintObjects); // 1.4
    HOOK("_ZN9PlayLayer8tintLineEN7cocos2d10_ccColor3BEf", PlayLayer_tintLine_H, PlayLayer_tintLine); // 1.4
    HOOK("_ZN9PlayLayer10tintGroundEN7cocos2d10_ccColor3BEf", PlayLayer_tintGround_H, PlayLayer_tintGround); // 1.0
    HOOK("_ZN9PlayLayer14tintBackgroundEN7cocos2d10_ccColor3BEf", PlayLayer_tintBackground_H, PlayLayer_tintBackground); // 1.0

    HOOK("_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchBegan_H, UILayer_ccTouchBegan);
    #if GAME_VERSION == V1P8
    HOOK("_ZN9PlayLayer5visitEv", PlayLayer_visit_H, PlayLayer_visit);
    #else
    HOOK("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayer_init_H, PlayLayer_init);
    #endif
    HOOK("_ZN9PlayLayer13toggleFlippedEbb", PlayLayer_toggleFlipped_H, PlayLayer_toggleFlipped);
    HOOK("_ZN7cocos2d11CCScheduler6updateEf", CCScheduler_update_H, CCScheduler_update);
    HOOK("_ZN9PlayLayer18loadLastCheckpointEv", PlayLayer_loadLastCheckpoint_H, PlayLayer_loadLastCheckpoint);
    HOOK("_ZN9PlayLayer20removeLastCheckpointEv", PlayLayer_removeLastCheckpoint_H, PlayLayer_removeLastCheckpoint);
    HOOK("_ZN9PlayLayer15storeCheckpointEP16CheckpointObject", PlayLayer_storeCheckpoint_H, PlayLayer_storeCheckpoint);
    HOOK("_ZN9PlayLayer6onQuitEv", PlayLayer_onQuit_H, PlayLayer_onQuit);
    HOOK("_ZN7UILayer4initEv", UILayer_init_H, UILayer_init);
    HOOK("_ZN9PlayLayer13destroyPlayerEv", PlayLayer_destroyPlayer_H, PlayLayer_destroyPlayer);
    HOOK("_ZN9PlayLayer13levelCompleteEv", PlayLayer_levelComplete_H, PlayLayer_levelComplete);
    HOOK("_ZN12PlayerObject10updateJumpEf", PlayerObject_updateJump_H, PlayerObject_updateJump);
    HOOK("_ZN9PlayLayer6updateEf", PlayLayer_update_H, PlayLayer_update);
    HOOK("_ZN9PlayLayer10resetLevelEv", PlayLayer_resetLevel_H, PlayLayer_resetLevel);
}