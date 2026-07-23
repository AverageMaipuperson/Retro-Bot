#include "CCMenuItemExt.h"

CCMenuItemExt* CCMenuItemExt::create(CCMenuItem* item, const Callback& callback)
{
    CCMenuItemExt* ret = new CCMenuItemExt();
    if (ret && ret->init(item, callback))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CCMenuItemExt::init(CCMenuItem* item, const Callback& callback)
{
    if (!CCMenuItem::initWithTarget(this, menu_selector(CCMenuItemExt::callback))) return false;

    _wrap = item;
    _callback = callback;

    if(_wrap)
    {
        this->addChild(_wrap);
        this->setContentSize(_wrap->getContentSize());
        _wrap->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        _wrap->setTarget(this, menu_selector(CCMenuItemExt::callback));
    }

    return true;
}

CCMenuItemExt* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, CCNode* disabled, const Callback& callback)
{
    return create(CCMenuItemSprite::create(normal, selected, disabled), callback);
}

CCMenuItemExt* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, const Callback& callback)
{
    return createWithSprite(normal, selected, nullptr, callback);
}

CCMenuItemExt* CCMenuItemExt::createWithSprite(CCNode* normal, const Callback& callback)
{
    return createWithSprite(normal, nullptr, nullptr, callback);
}

CCMenuItemExt* CCMenuItemExt::createWithLabel(CCNode* label, const Callback& callback)
{
    return create(CCMenuItemLabel::create(label), callback);
}

CCMenuItemExt* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, CCNode* selected, const Callback& callback)
{
    CCMenuItemExt* ret = new CCMenuItemExt();
    
    CCMenuItemSpriteExtra* item = CCMenuItemSpriteExtra::create(
        normal, 
        selected, 
        ret, 
        menu_selector(CCMenuItemExt::callback)
    );

    if (ret && ret->init(item, callback))
    {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

CCMenuItemExt* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, const Callback& callback)
{
    return createWithSpriteExtra(normal, nullptr, callback);
}

CCMenuItemExt* CCMenuItemExt::createWithToggler(CCNode* off, CCNode* on, const Callback& callback)
{
    CCMenuItemExt* ret = new CCMenuItemExt();
    
    CCMenuItemToggler* item = CCMenuItemToggler::create(
        off, 
        on, 
        ret, 
        menu_selector(CCMenuItemExt::callback)
    );

    if (ret && ret->init(item, callback))
    {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

void CCMenuItemExt::selected()
{
    CCMenuItem::selected();
    if(_wrap) _wrap->selected();
}

void CCMenuItemExt::unselected()
{
    CCMenuItem::unselected();
    if(_wrap) _wrap->unselected();
}

void CCMenuItemExt::setEnabled(bool enabled)
{
    CCMenuItem::setEnabled(enabled);
    if(_wrap) _wrap->setEnabled(enabled);
}

void CCMenuItemExt::setTag(int tag)
{
    CCMenuItem::setTag(tag);
    if(_wrap) _wrap->setTag(tag);
}

int CCMenuItemExt::getTag() const
{
    if(_wrap) return _wrap->getTag();
    else return 0;
}

CCMenuItem* CCMenuItemExt::getWrapper() const
{
    return _wrap;
}

void CCMenuItemExt::setData(void* data)
{
    _data = data;
}

void* CCMenuItemExt::getData() const
{
    return _data;
}

void CCMenuItemExt::callback(CCObject* sender)
{
    if (_callback) _callback(sender);
    if(auto toggler = dynamic_cast<CCMenuItemToggler*>(_wrap)) toggler->toggle(!toggler->m_toggled);
}

CCMenuItemExt::Callback& CCMenuItemExt::getTarget()
{
    return _callback;
}

void CCMenuItemExt::setTarget(const Callback& callback)
{
    _callback = callback;
}