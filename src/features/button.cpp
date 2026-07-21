#include "PauseLayer.hpp"
#include "RBot.h"
#include "other/hooking.h"
#include "custom_nodes/CCMenuItemExt.h"

void PauseLayer::onRBot(CCObject* sender) {
    
}

void (*PauseLayer_customSetup)(PauseLayer*);
void PauseLayer_customSetup_H(PauseLayer* self)
{
    PauseLayer_customSetup(self);
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto spr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    spr->setScale(0.5f);

    auto btn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [self](CCObject*)
        {
            auto rbot = RBotLayer::create(self);
            self->addChild(rbot, 1000);
            self->setTouchEnabled(false);
        }
    );
    btn->setPosition(CCPointMake(winSize.width - 40, winSize.height - 40));

    auto menu = CCMenu::create(btn, NULL);
    menu->setPosition(CCPointZero);
    self->addChild(menu);
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
}

void button_hook()
{
    HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_customSetup_H, PauseLayer_customSetup);
}