#pragma once
#include <cocos2d.h>
#include "CCBlockLayer.h"

class FreeMoveOptionsLayer : public CCBlockLayer {

public:
    static FreeMoveOptionsLayer* create(CCLayer* referrer);
	void onClose(CCObject*);
	bool init(CCLayer* referrer);
    void keyBackClicked();
};
