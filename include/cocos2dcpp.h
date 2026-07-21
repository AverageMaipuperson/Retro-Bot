#ifndef __COCOS2DCPP_H__
#define __COCOS2DCPP_H__

#define V1P0 10
#define V1P1 11
#define V1P2 12
#define V1P3 13
#define V1P4 14
#define V1P5 15
#define V1P6 16
#define V1P7 17
#define V1P8 18

#if GAME_VERSION > V1P6
    #include "cocos2d-2.2.3/cocos2dx/include/cocos2d.h"
    #include "cocos2d-2.2.3/CocosDenshion/include/SimpleAudioEngine.h"
#else
    #include "cocos2d-2.0.1/cocos2dx/include/cocos2d.h"
    #include "cocos2d-2.0.1/CocosDenshion/include/SimpleAudioEngine.h"
#endif

#include "offsets.hpp"

#include "CCMenuItemExt.h"
#include "ButtonSprite.hpp"
#include "CCBlockLayer.h"
#include "CCMenuItemSpriteExtra.hpp"
#include "CCMenuItemToggler.hpp"
#include "UILayer.hpp"
#include "PlayLayer.hpp"
#include "GameManager.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "FLAlertLayer.hpp"

#endif