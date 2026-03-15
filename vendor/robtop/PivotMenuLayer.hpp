#pragma once

#include <cocos2d.h>
using namespace cocos2d;

class PivotMenuLayer : public CCLayer {
	public:
	static void* m_menuGame;
	static PivotMenuLayer* create() {
        PivotMenuLayer *pRet = new PivotMenuLayer();
        if (pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
	bool init();
	void onFinished(CCObject*);
	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
};