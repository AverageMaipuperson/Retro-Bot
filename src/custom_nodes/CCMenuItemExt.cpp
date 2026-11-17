#include "CCMenuItemExt.h"

CCMenuItem* CCMenuItemExt::create(CCMenuItem* wrapper, const Callback& callback)
{
    auto listener = new CCMenuItemExt();
    listener->_callback = callback;
    listener->setTag(1);

    wrapper->setTarget(listener, menu_selector(CCMenuItemExt::callback));
    wrapper->addChild(listener); 
    listener->release(); 

    return wrapper;
}

CCMenuItemSprite* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, CCNode* disabled, const Callback& callback)
{
    return static_cast<CCMenuItemSprite*>(create(CCMenuItemSprite::create(normal, selected, disabled), callback));
}

CCMenuItemSprite* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, const Callback& callback)
{
    return createWithSprite(normal, selected, nullptr, callback);
}

CCMenuItemSprite* CCMenuItemExt::createWithSprite(CCNode* normal, const Callback& callback)
{
    return createWithSprite(normal, nullptr, nullptr, callback);
}

CCMenuItemLabel* CCMenuItemExt::createWithLabel(CCNode* label, const Callback& callback)
{
    return static_cast<CCMenuItemLabel*>(create(CCMenuItemLabel::create(label), callback));
}

CCMenuItemSpriteExtra* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, CCNode* selected, const Callback& callback)
{
    auto item = CCMenuItemSpriteExtra::create(normal, selected, nullptr, nullptr);
    return static_cast<CCMenuItemSpriteExtra*>(create(item, callback));
}

CCMenuItemSpriteExtra* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, const Callback& callback)
{
    return createWithSpriteExtra(normal, nullptr, callback);
}

CCMenuItemToggler* CCMenuItemExt::createWithToggler(CCNode* off, CCNode* on, const Callback& callback)
{
    auto item = CCMenuItemToggler::create(off, on, nullptr, nullptr);
    return static_cast<CCMenuItemToggler*>(create(item, callback));
}

void CCMenuItemExt::callback(CCObject* sender)
{
    if (_callback) _callback(sender);
}

CCMenuItemExt::Callback CCMenuItemExt::getTarget(CCMenuItem* wrapper)
{
    if (auto listener = dynamic_cast<CCMenuItemExt*>(wrapper->getChildByTag(1)))
    {
        return listener->_callback;
    }
    return nullptr;
}

void CCMenuItemExt::setTarget(CCMenuItem* wrapper, const Callback& callback)
{
    if (auto listener = dynamic_cast<CCMenuItemExt*>(wrapper->getChildByTag(1))) listener->_callback = callback;
    else create(wrapper, callback);
}