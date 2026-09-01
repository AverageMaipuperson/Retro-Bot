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
    void toggleLayoutMode(bool);
    void tintBackground(cocos2d::_ccColor3B, float);
    void tintGround(cocos2d::_ccColor3B, float);
    void tintLine(cocos2d::_ccColor3B, float);
    void tintObjects(cocos2d::_ccColor3B, float);
    void tintColorObjects(cocos2d::_ccColor3B, float);
};
