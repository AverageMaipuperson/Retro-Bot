#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"
#include "FLAlertLayer.hpp"

class LevelInfoLayer : public CCLayer, public FLAlertLayerProtocol {
	public:
	static LevelInfoLayer* create(GJGameLevel*);
	void onPlay(LevelInfoLayer*);
	void onClone();
};