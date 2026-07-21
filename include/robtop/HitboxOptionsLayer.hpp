#pragma once
#include <cocos2d.h>
#include "CCBlockLayer.h"

class HitboxOptionsLayer : public CCBlockLayer {

public:
    static HitboxOptionsLayer* create(CCLayer* referrer);
	void onClose(CCObject*);
	bool init(CCLayer* referrer);
    void keyBackClicked();
};
