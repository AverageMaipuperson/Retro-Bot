#pragma once

#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "GameStatsManager.hpp"
#include "CCTextInputNode.hpp"

namespace ExtraLayerInfo {
	enum class InfoType {
        PMH,
        HA,
        SEI,
		SF,
		FPS,
		NP,
		NDE,
		NOCLIP,
		DEATHS,
		FLASH,
		PEE,
		NRT,
		HPM,
		DA,
		LL,
		MEB,
		DAT,
		SPEEDM,
		OPPS,
		EX,
		QB,
		HV,
		EH,
		CTP,
        UNKNOWN
    };
}

class ExtraLayer : public CCBlockLayer {

public:
	static bool m_deaths;
	static bool m_flash;
	static float m_speedhack;
	static std::string m_customLabelValue;
	static bool m_speedhackEnabled;
	static CCTextInputNode* m_speedHackInput;
	static CCTextInputNode* m_customLabelInput;
	static bool m_speedhackMusic;
	static int m_currentPage;
	static CCLayer* m_page1;
	static CCLayer* m_page2;
	static CCLayer* m_page3;
	static CCLayer* m_page4;
	static CCLayer* m_page5;
	static CCMenu* m_right;
	static CCMenu* m_left;
	static CCLayer* m_self;
	static CCLayer* m_lel;
	static cocos2d::ccColor3B m_flashColor;
	static bool uploadNotAllowed_;
    static ExtraLayer* create(CCLayer* referrer);
	void onClose(CCObject*);
	void dummy(CCObject* pSender);
	void readSettings();
	void showHA();
	void showEI();
	void showPMH();
	void showSF();
	void showFPS();
	void showNP();
	void showNDE();
	void showNOCLIP();
	void showDEATHS();
	void showFLASH();
	void showPEE();
	void showInfo(CCObject* sender);
	static void incrementStat(GameStatsManager* self, char* type, int amount);
	static int getStat(GameStatsManager* self, char* type);
	static bool saveSettingsToFile();
	static bool loadSettingFromFile(const char* settingname);
	void toggle(CCObject* sender);
	CCNode* optionToggler(const char* display, bool* toggleVar, bool addinfo = false, ExtraLayerInfo::InfoType type = ExtraLayerInfo::InfoType::UNKNOWN);
	CCMenu* createOptionsMenu(const char* display, int length, SEL_MenuHandler func);
	static void onLoadSettings();
	void onNoclipOptions();
	void onPEEOptions();
	void onCustomLabelOptions();
	void nextPage();
	void prevPage();
	void volumeSliderCallback(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
	void exportSaveFile();
	void manualSave(CCObject*);
	void showUID(CCObject*);
	bool extractFile(char const*);
protected:
    bool init(CCLayer* referrer);
    void keyBackClicked();
};