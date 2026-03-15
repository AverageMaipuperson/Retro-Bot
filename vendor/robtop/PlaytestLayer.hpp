#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"
#include "PlayerObject.hpp"
#include "GameManager.hpp"
#include "EditorUI.hpp"
using namespace cocos2d;

class PlaytestLayer : public CCLayer, public CCTouchDelegate {
public:
	PlayerObject* m_player;
	GameManager* m_manager;
	int m_playerPos;
	EditorUI* m_ui;
	static PlaytestLayer* self;
	bool m_jumping;
    static PlaytestLayer* create(EditorUI*, PlayerObject*);
	static PlaytestLayer* get();
protected:
	bool init(EditorUI*, PlayerObject*);
	virtual void update(float) override;
};
