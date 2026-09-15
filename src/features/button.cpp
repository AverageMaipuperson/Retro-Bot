#include "PauseLayer.hpp"
#include "rbot.hpp"
#include "RBotLayer.hpp"
#include "hooking.hpp"
#include "custom_nodes/CCMenuItemExt.h"
#include "tools.hpp"

void (*PauseLayer_customSetup)(PauseLayer*);
void PauseLayer_customSetup_H(PauseLayer* self)
{
    PauseLayer_customSetup(self);
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto spr = CCSprite::create("rbot_menu.png");
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

    rbot::getModules().paused = true;
}

bool (*ButtonSprite_init)(ButtonSprite*, char const*, int, int, float, bool, char const*, char const*, float);
bool ButtonSprite_init_H(ButtonSprite* self, char const* t, int w, int h, float s, bool a, char const* f, char const* b, float i)
{
    ButtonSprite_init(self,t,w,h,s,a,f,b,i);

    if(std::string(f) == "BoonBoldItalic.fnt") MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x284)->setPosition(ccp(MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x284)->getPositionX(), MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x284)->getPositionY() - 1));
    return true;
}

void button_hook()
{
    HOOK("_ZN12ButtonSprite4initEPKciifbS1_S1_f", ButtonSprite_init_H, ButtonSprite_init);
    HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_customSetup_H, PauseLayer_customSetup);
}