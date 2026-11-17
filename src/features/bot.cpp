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
#include "CheckpointObject.hpp"

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

    if (mod::module_by_id<bool>(id::frame_stepper))
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

    auto step = 1.f / 240.f;
    if (modules.stepperOn)
        dt = step;

    if (modules.stepperOn && dt != step) {
        PlayLayer_update(self, 0);
        return;
    }

    if (modules.stepperOn || (mod::module_by_id<bool>(id::update_on_steps) && modules.mode == Modes::kModeRecording))
        dt = step;

    auto& data = rbot::getFrameData();

    if (modules.mode == Modes::kModePlaying)
    {
        // i have to advance every input by one frame for some reason, it will work trust
        auto oldTime = modules.time;
        modules.time += dt;

        if (data.empty())
        {
            rbot::stopMacro();
            PlayLayer_update(self, dt);
            return;
        }

        auto eps = 1e-6;
        while (modules.frame < static_cast<int>(data.size()) && data[modules.frame].frame <= oldTime + eps)
        {
            auto frame = data[modules.frame];

            if (frame.click)
                getPlayer(self)->pushButton(Button1);
            if (frame.release)
                getPlayer(self)->releaseButton(Button1);

            if (mod::module_by_id<bool>(id::click_sounds))
            {
                auto sound = GameSoundManager::sharedManager();
                if (frame.click)
                    sound->playEffect(modules.soundPath.c_str(), 1, 0, 1);
                else if (frame.release)
                    sound->playEffect(modules.releasePath.c_str(), 1, 0, 1);
            }

            modules.frame++;
        }

        if (modules.frame >= static_cast<int>(data.size()))
        {
            rbot::stopMacro();
            PlayLayer_update(self, dt);
            return;
        }
    }

    PlayLayer_update(self, dt);

    if (modules.mode == Modes::kModeRecording)
    {
        if (modules.completed) rbot::stopMacro();

        auto pos = MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition);
        if (MEMBER_BY_OFFSET(bool, self, PlayLayer__m_isFlipped))
        {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto center = winSize.width / 2;
            pos.x = (pos.x - center) + winSize.width - center;
        }

        modules.time += dt; // keep this consistent with playback
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

    if (mod::module_by_id<bool>(id::playback_label) && modules.mode == kModePlaying)
    {
        modules.playbackLabel->setVisible(true);
    }
    else
    {
        modules.playbackLabel->setVisible(false);
    }

    self->toggleLayoutMode(LAYOUT_CHECK);
    if (LAYOUT_CHECK != modules.lastLayoutState)
        modules.lastLayoutState = LAYOUT_CHECK;
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

    PlayerObject_updateJump(self, dt);
/*
    if(m.mode != Modes::kModePlaying) PlayerObject_updateJump(self, dt);
    else
    {
        // fix ground particles
        auto frame = rbot::getFrameData()[m.frame];
        if(frame.onGround) MEMBER_BY_OFFSET(bool, self, PlayerObject__m_groundParticleOff) = false;
        else self->deactivateParticle();
    }
*/
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

    constexpr auto physics_step = 1.0 / 240.0;
    static auto accumulator = 0.0;

    double speed = 1.0;
    if (mod::module_by_id<bool>(id::speedhack) && !modules.paused)
        speed = mod::module_by_id<float>(id::speedhack_val);

    accumulator += static_cast<double>(dt) * speed;

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
        getPlayer(self)->getRotation(),
        MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition),
        MEMBER_BY_OFFSET(float, getPlayer(self), PlayerObject__m_unk)
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
        if (modules.mode == Modes::kModePlaying)
        {
            auto& data = rbot::getFrameData();
            modules.frame = static_cast<int>(
                std::lower_bound(data.begin(), data.end(), modules.time,
                [](const Frame& f, double time) { return f.frame < time; }) - data.begin()
            );
        }

        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_yVelocity) = checkpoints.back().yVelocity;
        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_gravity) = checkpoints.back().gravity;
        getPlayer(self)->setRotation(checkpoints.back().rotation);
        MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition) = checkpoints.back().position;
        MEMBER_BY_OFFSET(float, getPlayer(self), PlayerObject__m_unk) = checkpoints.back().unk;
    }
}

#if GAME_VERSION < V1P4
void (*PlayLayer_visit_t)(PlayLayer*) = nullptr;
void PlayLayer_visit(PlayLayer* self)
{
    PlayLayer_visit_t(self);

    auto gameLayer = MEMBER_BY_OFFSET(CCLayer*, self, PlayLayer__m_gameLayer);

    if (mod::module_by_id<bool>(id::show_hitboxes))
    {
        kmGLPushMatrix();

        gameLayer->transform(); 
        drawHitboxes(self);
        
        kmGLPopMatrix();
    }
}
#endif

bool (*PlayLayer_init)(PlayLayer*, GJGameLevel*);
bool PlayLayer_init_H(PlayLayer* self, GJGameLevel* lvl)
{
    PlayLayer_init(self, lvl);

    rbot::getModules().gameStarted = false;
    
    // MEMBER_BY_OFFSET(CCLayer*, self, PlayLayer__m_gameLayer)->addChild(HitboxLayer::create(self), 9999);

#if GAME_VERSION < V1P4
    if (!PlayLayer_visit_t)
    {
#if GAME_VERSION < V1P7
        void** vtable = *(void***)self;
        void (PlayLayer::* ptr)() = &PlayLayer::visit;
        void* offset = *(void**)&ptr;

        auto index = ((uintptr_t)offset) / sizeof(void*);
        PlayLayer_visit_t = reinterpret_cast<void(*)(PlayLayer*)>(vtable[index]);
#if GAME_VERSION < V1P5
        vtable[((uintptr_t)offset)/sizeof(void*)] = (void*)&PlayLayer_visit;
#else
        DobbyCodePatch(&vtable[((uintptr_t)offset)/sizeof(void*)], uintptrToBytes((uintptr_t)&PlayLayer_visit).data(), 4);
#endif
#else
        DobbyCodePatch(reinterpret_cast<void*>(get_address(__PlayLayer_visit)),
        uintptrToBytes((uintptr_t)&PlayLayer_visit).data(), 4);
#endif
    }
#endif

    self->toggleLayoutMode(LAYOUT_CHECK);
    
    return true;
}

#if GAME_VERSION > V1P3
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
#endif

void (*PlayLayer_toggleFlipped)(PlayLayer*, bool, bool);
void PlayLayer_toggleFlipped_H(PlayLayer* self, bool direction, bool doFlip)
{
    if(!mod::module_by_id<bool>(id::disable_mirror_portals)) PlayLayer_toggleFlipped(self, direction, doFlip);
}

bool (*UILayer_ccTouchBegan)(UILayer*, CCTouch*, CCEvent*);
bool UILayer_ccTouchBegan_H(UILayer* self, CCTouch* touch, CCEvent* event)
{
    auto b = UILayer_ccTouchBegan(self, touch, event);
    if (!b) return false;

    auto& m = rbot::getModules();
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

void (*UILayer_ccTouchEnded)(UILayer*, CCTouch*, CCEvent*);
void UILayer_ccTouchEnded_H(UILayer* self, CCTouch* touch, CCEvent* event)
{
    UILayer_ccTouchEnded(self, touch, event);
    auto& m = rbot::getModules();
    if(m.mode == Modes::kModeRecording)
    {
        m.usedFrame->release = true;
    }

    if(m.mode != Modes::kModePlaying && mod::module_by_id<bool>(id::click_sounds))
    {
        auto sound = GameSoundManager::sharedManager();
        sound->playEffect(m.releasePath.c_str(), 1, 0, 1);
    }
}

void PlayLayer::toggleLayoutMode(bool enabled)
{
    auto& m = rbot::getModules();
    auto objects = MEMBER_BY_OFFSET(CCArray*, this, PlayLayer__m_objects);

    auto camX = MEMBER_BY_OFFSET(CCPoint, this, PlayLayer__m_cameraPos).x;
    auto startSection = std::max(0, static_cast<int>((camX - 300.0f) / 100.0f));
    auto endSection = std::min(static_cast<int>(objects->count()) - 1, static_cast<int>((camX + 900) / 100));

    if (!enabled && m.lastLayoutState)
    {
        startSection = 0;
        endSection = static_cast<int>(objects->count()) - 1;
    }

    if (enabled || m.lastLayoutState)
    {
        for (int i = startSection ; i <= endSection ; i++)
        {
            auto section = static_cast<CCArray*>(objects->objectAtIndex(i));
            auto count = section->count();

            for (int j = 0 ; j < count ; j++)
            {
                if (dynamic_cast<CheckpointObject*>(section->objectAtIndex(j))) continue;

                auto obj = static_cast<GameObject*>(section->objectAtIndex(j));
                auto type = MEMBER_BY_OFFSET(int, obj, GameObject__m_type);
                
                if (((type == 7 || type == 8) && MEMBER_BY_OFFSET(int, obj, GameObject__m_key) != 44) && obj->isVisible() == enabled)
                    obj->setVisible(!enabled);
            }
        }
    }

    if(enabled && !m.lastLayoutState)
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
    HOOK("_ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchEnded_H, UILayer_ccTouchEnded);
    HOOK("_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchBegan_H, UILayer_ccTouchBegan);
#if GAME_VERSION > V1P3
    HOOK("_ZN9PlayLayer5visitEv", PlayLayer_visit_H, PlayLayer_visit);
#endif
    HOOK("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayer_init_H, PlayLayer_init);
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