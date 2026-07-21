#pragma once

#include <cocos2d.h>
#include "CCMenuItemSpriteExtra.hpp"

class CCMenuItemToggler : public cocos2d::CCMenuItem
{
public:
    CCMenuItemSpriteExtra* m_offButton;
    CCMenuItemSpriteExtra* m_onButton;
    bool m_toggled;
    
    static CCMenuItemToggler* create(cocos2d::CCNode* off, cocos2d::CCNode* on,
        cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    
    void setSizeMult(float);
    void toggle(bool);
};