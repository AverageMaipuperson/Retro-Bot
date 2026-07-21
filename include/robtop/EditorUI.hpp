#pragma once

#include <cocos2d.h>
#include "FLAlertLayer.hpp"
#include "GameObject.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include <utility>

class CreateMenuItem : public CCMenuItemSpriteExtra {};

class EditorUI : public cocos2d::CCLayer, public cocos2d::CCTargetedTouchDelegate
{
public:
    static EditorUI* create();
    bool init();

    void onPause();
    void deselectAll();
    void selectObject(GameObject*);
    void onDeleteSelected();
    void updateCreateMenu();
    CreateMenuItem* getCreateBtn(const char*, int);
	void showDeleteConfirmation();
	void toggleDuplicateButton();
	void toggleFilter();
	void createFilterBtn();
	void updateSlider();
	void resetupTheMenu();
	void onDuplicate();
	void onCopy();
	void onPaste();
	void onCopy2();
	void onPaste2();
	void updateButtons();
	void updateButtons2();
	static EditorUI* get();

    CCMenuItemSpriteExtra* getSpriteButton(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale);
    CCMenuItemSpriteExtra* getSpriteButton2(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale);
    CCMenuItemSpriteExtra* getSpriteButton3(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale, float scale2);
    void moveObjectCall(CCNode* sender);
    void transformObjectCall(CCNode* sender);
	void getObjectInfoCall(CCNode* sender);
    void moveObject(GameObject* obj, CCPoint transform);
    void editObject();
	void deleteAll();
	CCPoint getTouchPoint(CCTouch*, CCEvent*);

	void onPlay(CCObject*);
    void stopMusic(CCObject*);
	void playtest(CCObject*);

    void moveObjectCall2(CCNode* sender);
    void transformObjectCall2(CCNode* sender);
};