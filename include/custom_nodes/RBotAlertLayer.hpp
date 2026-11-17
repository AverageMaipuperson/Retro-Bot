#pragma once
#include "cocos2dcpp.h"

using namespace cocos2d;

class RBotAlertLayer : public CCLayerColor
{
public:
    CCNode* m_mainLayer;
    CCMenu* m_mainMenu;

    static RBotAlertLayer* create(CCRect);
    bool init(CCRect);
    void keyBackClicked();
    void onClose();
    bool ccTouchBegan(CCTouch*, CCEvent*) { return true; }
    void registerWithTouchDispatcher()
    {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        dispatcher->addTargetedDelegate(this, 0x80000003, true);
    }
};