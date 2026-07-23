/*
1.8 CANNOT release because of this specific file

I spent about 5 days trying to fix a fatal crash on CCNode::setActionManager

Anything that i tried didn't work

And i need CCMenuItemExt at the same time because cocos2d is dumb sometimes

At the end i gave up because there was no reason to waste my time on fixing a bug i could not fix

so yea... rbot does not support 1.8


Also, CCMenuItemToggler breaks for versions 1.0-1.4 if you're using rbot...
visual state doesn't change even if you force it to the hard way

And if you really try to force it to it just crashes

CCMenuItemToggler::toggle crashes the game when i try calling it in a lambda
*/

#ifndef __CCMENUITEMEXT_H__
#define __CCMENUITEMEXT_H__

#include <cocos2dcpp.h>
using namespace cocos2d;

// CCMenuItemExt allows for lambdas to be called, and combines most CCMenuItem types so you can create them just using this
class CCMenuItemExt : public CCMenuItem {
public:

    using Callback = std::function<void(CCObject*)>;

    /* #if defined(__arm__) && GAME_VERSION == V1P8
    virtual void vtable_pad() {} 
    #endif */

    /* CCMenuItemExt::create requires a CCMenuItem* to work as a wrapper so you can do the callback.
    @param wrapper the wrapper
    @param callback your lambda callback
    */
    static CCMenuItemExt* create(CCMenuItem* wrapper, const Callback& callback);

    static CCMenuItemExt* createWithSprite(CCNode*, const Callback&);

    static CCMenuItemExt* createWithSprite(CCNode*, CCNode*, const Callback&);

    static CCMenuItemExt* createWithSprite(CCNode*, CCNode*, CCNode*, const Callback&);

    static CCMenuItemExt* createWithLabel(CCNode*, const Callback&);

    static CCMenuItemExt* createWithSpriteExtra(CCNode*, CCNode*, const Callback&);

    static CCMenuItemExt* createWithSpriteExtra(CCNode*, const Callback&);

    static CCMenuItemExt* createWithToggler(CCNode*, CCNode*, const Callback&);
    
    void setEnabled(bool);

    void setTag(int);
    
    int getTag() const;

    void setData(void*);

    void* getData() const;

    CCMenuItem* getWrapper() const;

    void setTarget(const Callback& callback);

    Callback& getTarget();

protected:
    bool init(CCMenuItem*, const Callback&);

    void selected();
    
    void unselected();

    void callback(CCObject*);
private:
    CCMenuItem* _wrap = nullptr;
    Callback _callback = nullptr;
    void* _data = nullptr;
};

#define GET_EXT(item) static_cast<CCMenuItemExt*>(item->getParent())
#endif