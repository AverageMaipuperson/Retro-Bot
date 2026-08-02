#pragma once

#include "../cocos2dcpp.h"
#include "PlayerObject.hpp"
#include "GJGameLevel.hpp"

class PlayLayer : public cocos2d::CCLayer {
public:
    static PlayLayer* create(GJGameLevel*);
	static PlayLayer* get();
    bool init(void*);

    void resetLevel();
    void levelComplete();
    void removeLastCheckpoint();
    CCNode* getLastCheckpoint();
    GJGameLevel* getLevel() const;
    void onStepper(CCObject*);
};
