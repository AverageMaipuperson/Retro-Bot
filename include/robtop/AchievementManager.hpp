#pragma once

#include <cocos2d.h>
// #include "PlayLayer.hpp"

class AchievementManager : public cocos2d::CCNode {
public:
    static AchievementManager* sharedState();

    void notifyAchievementWithID(const char* ach);
};