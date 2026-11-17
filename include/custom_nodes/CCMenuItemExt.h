#ifndef __CCMENUITEMEXT_H__
#define __CCMENUITEMEXT_H__

#include <cocos2d.h>
#include <functional>
#include "CCMenuItemToggler.hpp"
#include "CCMenuItemSpriteExtra.hpp"

class CCMenuItemExt : public CCNode {
public:
    using Callback = std::function<void(CCObject*)>;

    static CCMenuItem* create(CCMenuItem* wrapper, const Callback& callback);
    static CCMenuItemSprite* createWithSprite(CCNode*, CCNode*, CCNode*, const Callback&);
    static CCMenuItemSprite* createWithSprite(CCNode*, CCNode*, const Callback&);
    static CCMenuItemSprite* createWithSprite(CCNode*, const Callback&);
    static CCMenuItemLabel* createWithLabel(CCNode*, const Callback&);
    static CCMenuItemSpriteExtra* createWithSpriteExtra(CCNode*, CCNode*, const Callback&);
    static CCMenuItemSpriteExtra* createWithSpriteExtra(CCNode*, const Callback&);
    static CCMenuItemToggler* createWithToggler(CCNode*, CCNode*, const Callback&);

    static Callback getTarget(CCMenuItem* wrapper);
    static void setTarget(CCMenuItem* wrapper, const Callback& callback);

    void callback(CCObject*);

private:
    Callback _callback = nullptr;
};

#endif