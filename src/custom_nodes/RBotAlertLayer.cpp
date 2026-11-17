#include "RBotAlertLayer.hpp"
#include "mod.hpp"

RBotAlertLayer* RBotAlertLayer::create(CCRect rect)
{
    auto ret = new RBotAlertLayer;

    if (ret->init(rect))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool RBotAlertLayer::init(CCRect rect)
{
    CCLayerColor::initWithColor(ccc4(0,0,0,105));
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto director = CCDirector::sharedDirector();
    auto dispatcher = director->getTouchDispatcher();
    dispatcher->setForcePrio(true);
    dispatcher->setTargetPrio(0x80000002);

    m_mainLayer = CCLayer::create();
    addChild(m_mainLayer);

    auto panel = extension::CCScale9Sprite::create("rbot_square.png", {0,0,80,80});
    panel->setContentSize(rect.size);
    panel->setPosition(ccp(winSize.width / 2, winSize.height / 2)); 
    m_mainLayer->addChild(panel);

    m_mainMenu = CCMenu::create();
    m_mainMenu->setPosition(ccp(0,0));
    m_mainLayer->addChild(m_mainMenu);

    auto spr = CCSprite::create("rbot_close.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(RBotAlertLayer::keyBackClicked));
    btn->setPosition(ccp((winSize.width / 2) + (rect.size.width / 2) - 10, (winSize.height / 2) + (rect.size.height / 2) - 10));
    m_mainMenu->addChild(btn);

    if (!mod::module_by_id<bool>(id::fast_menu))
    {
        m_mainLayer->setScale(.1f);
        auto scaleUp = CCScaleTo::create(.5f, 1.f);
        auto ease = CCEaseElasticOut::create(scaleUp, .5f);
        m_mainLayer->runAction(ease);
    }

    return true;
}

void RBotAlertLayer::onClose()
{
    removeFromParentAndCleanup(true);
}

void RBotAlertLayer::keyBackClicked()
{
    onClose();
}