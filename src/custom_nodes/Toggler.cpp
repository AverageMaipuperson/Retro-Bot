#include "robtop/CCMenuItemToggler.hpp"
#include "Toggler.hpp"
using namespace cocos2d;

bool Toggler::init(const char* text, bool* var, CCObject* target, SEL_MenuHandler callback, int tag)
{
    m_opt = var;

    auto on = CCSprite::create("rbot_toggleOn.png");
    on->setScale(.5f);
    auto off = CCSprite::create("rbot_toggleOff.png");
    off->setScale(.5f);
    
    m_button = CCMenuItemToggler::create(off, on, target, callback);
    m_button->setTag(tag);
    m_button->setUserData(var);

    if (var && *var) m_button->toggle(true);

    m_label = CCLabelBMFont::create(text, "BoonBoldItalic.fnt");
    m_label->setAnchorPoint(ccp(0.5, 0.5));

    auto menu = CCMenu::create(m_button, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    this->addChild(m_label);
    m_label->setPosition(ccp(50, 0));

    return true;
}