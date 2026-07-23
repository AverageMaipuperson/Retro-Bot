#pragma once

#include <cocos2dcpp.h>
#include "FLAlertLayer.hpp"
using namespace cocos2d;

#if GAME_VERSION == 18
class PauseLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol {
#else
class PauseLayer : public cocos2d::CCLayerColor, public FLAlertLayerProtocol {
#endif
public:
    static PauseLayer* create(void*);
    void onRBot(cocos2d::CCObject* sender);
};