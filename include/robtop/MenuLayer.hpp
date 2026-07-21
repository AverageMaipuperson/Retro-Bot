#pragma once

#include "FLAlertLayer.hpp"
#include <atomic>
#include <string>

class MenuLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol {
public:
static MenuLayer* sharedLayer;
static std::atomic<char*> s_newVersion;
static std::atomic<bool> s_hasVersionData;
    static MenuLayer* node();
    static cocos2d::CCScene* scene() ;

    void onMenuInfo();
    void onOpenMenu();
	void onOptions();
	void checkVersionIsOutdated();
	void showUpdateAlert(float dt);
	void onVersionReceived(CCObject*);
	void endGame();
	void versionCheckPoll(float dt) ;
	void onSpecialThanks(CCObject*);

    virtual bool init();
    virtual void keyBackClicked();
    virtual void googlePlaySignedIn();
    virtual void FLAlert_Clicked(FLAlertLayer* alert, bool btn2);
};