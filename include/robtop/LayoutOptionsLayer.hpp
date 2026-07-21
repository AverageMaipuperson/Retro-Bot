#pragma once
#include <cocos2d.h>
#include "CCBlockLayer.h"

class LayoutOptionsLayer : public CCBlockLayer {

public:
    static LayoutOptionsLayer* create(CCLayer* referrer);
	void onClose(CCObject*);
	bool init(CCLayer* referrer);
    void keyBackClicked();
};
