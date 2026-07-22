#include "robtop/CCMenuItemToggler.hpp"
#include "rbot/Toggler.h"
using namespace cocos2d;

bool Toggler::init(const char* text, bool* var, CCObject* target, SEL_MenuHandler callback, int tag)
{
    m_opt = var;

    #if GAME_VERSION > V1P0
    auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    #else
    auto on = CCSprite::create("GJ_checkOn_001.png");
    auto off = CCSprite::create("GJ_checkOff_001.png");
    #endif
    
    m_button = CCMenuItemToggler::create(off, on, target, callback);
    m_button->setTag(tag);
    m_button->setUserData(var);

    if (var && *var) m_button->toggle(true);

    m_label = CCLabelBMFont::create(text, "bigFont.fnt");
    float width = m_label->getContentSize().width;
    m_label->setAnchorPoint(ccp(0.5, 0.5));
    if (width > 55.f) m_label->setScale(55.f / width);
    else m_label->setScale(0.25f);

    auto menu = CCMenu::create(m_button, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    this->addChild(m_label);
    m_label->setPosition(ccp(50, 0));

    return true;
}