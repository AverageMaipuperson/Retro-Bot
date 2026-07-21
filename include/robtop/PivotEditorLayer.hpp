#pragma once

#include <cocos2d.h>
#include "EditorUI.hpp"
using namespace cocos2d;

class PivotEditorLayer : public CCLayer {
public:
	EditorUI* m_ui;
	static PivotEditorLayer* self;
    static PivotEditorLayer* create(EditorUI*);
protected:
	bool init(EditorUI*);
	virtual void update(float) override;
};
