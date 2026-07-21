#pragma once

#include <cocos2d.h>
#include "PlayLayer.hpp"

class GameRateDelegate {
	virtual void updateRate();
};

class GameManager : public cocos2d::CCNode {
public:
    static GameManager* sharedState();

    void reportAchievementWithID(const char* ach, int percent, bool);
    PlayLayer* getPlayLayer() const;
};