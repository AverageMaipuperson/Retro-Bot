#include "RBot.h"
#include "PlayLayer.hpp"
#include "UILayer.hpp"
#include "PlayerObject.hpp"
#include "tools.hpp"
#include "other/hooking.h"
#include "mod.hpp"
#include "PlayHitboxLayer.hpp"

void createPlaybackLabel()
{
    auto& m = RBot::getModules();
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
    auto& modules = RBot::getModules();
    auto& data = RBot::getFrameData();

    if (modules.mode == Modes::kModePlaying)
    {

        modules.time += dt;

        if (data.empty())
        {
            RBot::stopMacro();
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
            RBot::stopMacro();
            PlayLayer_update(self, dt);
            return;
        }

        auto frame = RBot::getFrameData()[index];

        getPlayer(self)->setPosition(frame.position);
        getPlayer(self)->setLastP(frame.position);
        MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition) = frame.position;
        getPlayer(self)->setRotation(frame.rotation);
        getPlayer(self)->setScaleY(frame.flipY);
        MEMBER_BY_OFFSET(bool, getPlayer(self), PlayerObject__m_onGround) = frame.onGround;
        
        modules.frame = index;
    }

    PlayLayer_update(self, dt);

    if (modules.mode == Modes::kModeRecording)
    {
        if (modules.completed) RBot::stopMacro();
        
        modules.time += dt;
        RBot::getFrameData().push_back({
            modules.time,
            getPlayer(self)->getScaleY(),
            #if GAME_VERSION > V1P0
            getPlayer(self)->getPosition(), 
            #else
            MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_realPosition),
            #endif
            getPlayer(self)->getRotation(),
            MEMBER_BY_OFFSET(bool, getPlayer(self), PlayerObject__m_onGround)
        });
        modules.frame++;
    }

    if(mod::module_by_id<bool>(id::playback_label) && modules.mode == kModePlaying)
    {
        modules.playbackLabel->setVisible(true);
    } else modules.playbackLabel->setVisible(false);

    if(!modules.completed)
    {
        if(mod::module_by_id<bool>(id::speedhack)) CCDirector::sharedDirector()->getScheduler()->setTimeScale(mod::module_by_id<float>(id::speedhack_val));
        else CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
    }
}

void (*PlayLayer_resetLevel)(PlayLayer*);
void PlayLayer_resetLevel_H(PlayLayer* self)
{
    RBot::getModules().frame = 0;
    RBot::getModules().time = 0;
    PlayLayer_resetLevel(self);
    if (RBot::getModules().mode == kModeRecording && !isPractice(self)) 
    {
        RBot::getFrameData().clear();
        RBot::getCheckpointData().clear();
    }
    
    if (RBot::getModules().mode == kModeRecording && isPractice(self) && !RBot::getCheckpointData().empty())
    {
        auto frame = RBot::getModules().time;
        auto& frames /*s*/ = RBot::getFrameData();

        frames.erase(
            std::remove_if(frames.begin(), frames.end(), [frame](const Frame& f)
                {
                    return f.frame >= frame;
                }
            ), 
        frames.end()
        );
    }

    if (RBot::getModules().mode == kModeRecording && isPractice(self) && RBot::getCheckpointData().empty())
    {
        RBot::getFrameData().clear();
    }
}

void (*PlayerObject_updateJump)(PlayerObject*, float);
void PlayerObject_updateJump_H(PlayerObject* self, float dt)
{
    if(RBot::getModules().mode != Modes::kModePlaying) PlayerObject_updateJump(self, dt);
    else
    {
        if(self->getPositionY() <= 105) self->hitGround(true);
        else self->hitGround(false);
    }
}

void (*PlayLayer_levelComplete)(PlayLayer*);
void PlayLayer_levelComplete_H(PlayLayer* self)
{
    PlayLayer_levelComplete(self);
    RBot::getModules().completed = true;
    RBot::stopMacro();
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
    RBot::reset();
    auto& m = RBot::getModules();
    if(mod::module_by_id<bool>(id::playback_label) && m.playbackLabel)
    {
        m.playbackLabel->release();
        m.playbackLabel = nullptr;
    }
}

bool (*UILayer_init)(UILayer*);
bool UILayer_init_H(UILayer* self)
{
    UILayer_init(self);
    auto& m = RBot::getModules();
    if(!m.playbackLabel)
    {
        createPlaybackLabel();
    }
    if(!m.playbackLabel->getParent()) self->addChild(m.playbackLabel);
    if(!mod::module_by_id<bool>(id::playback_label) || m.mode != kModePlaying) m.playbackLabel->setVisible(false);
    return true;
}

void (*PlayLayer_storeCheckpoint)(PlayLayer* self, void* checkpoint);
void PlayLayer_storeCheckpoint_H(PlayLayer* self, void* checkpoint)
{
    PlayLayer_storeCheckpoint(self, checkpoint);
    RBot::getCheckpointData().push_back({static_cast<double>(RBot::getModules().time), MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_yVelocity), MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_gravity)});
}

void (*PlayLayer_removeLastCheckpoint)(PlayLayer* self);
void PlayLayer_removeLastCheckpoint_H(PlayLayer* self)
{
    PlayLayer_removeLastCheckpoint(self);
    auto& checkpoints = RBot::getCheckpointData();
    if(!checkpoints.empty()) checkpoints.pop_back();
}

void (*PlayLayer_loadLastCheckpoint)(PlayLayer* self);
void PlayLayer_loadLastCheckpoint_H(PlayLayer* self)
{
    PlayLayer_loadLastCheckpoint(self);
    auto& checkpoints = RBot::getCheckpointData();
    if(!checkpoints.empty())
    {
        auto& modules = RBot::getModules();
        modules.time = checkpoints.back().frame;

        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_yVelocity) = checkpoints.back().yVelocity;
        MEMBER_BY_OFFSET(double, getPlayer(self), PlayerObject__m_gravity) = checkpoints.back().gravity;
    }
}

bool (*PlayLayer_init)(PlayLayer*, GJGameLevel*);
bool PlayLayer_init_H(PlayLayer* self, GJGameLevel* lvl)
{
    PlayLayer_init(self, lvl);
    MEMBER_BY_OFFSET(CCLayer*, self, PlayLayer__m_gameLayer)->addChild(PlayHitboxLayer::create(self), 9999);
    return true;
}

// force the game to run at 240 fps
void (*CCScheduler_update)(CCScheduler*, float);
void CCScheduler_update_H(CCScheduler* self, float dt)
{
    if(!mod::module_by_id<bool>(id::update_on_steps))
    {
        CCScheduler_update(self, dt);
        return;
    }
    static double accumulator = 0;
    accumulator += static_cast<double>(dt) * 1;
    auto delta = 1.0 / 240;

    auto step_epsilon = delta * 0.001;
    while (accumulator >= (delta - step_epsilon))
    {
        CCScheduler_update(self, static_cast<float>(delta));
        accumulator -= delta;
    }
    if (accumulator > delta * 5.0) accumulator = 0;
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

void bot_hook()
{
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