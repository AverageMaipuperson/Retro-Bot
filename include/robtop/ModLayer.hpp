#pragma once

#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "GameStatsManager.hpp"
#include "CCDropdown.hpp"
#include "CCTextInputNode.hpp"
#include <map>
#include "FLAlertLayer.hpp"
#include "ButtonSprite.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "PlayLayer.hpp"

static std::string currentSpecificVersion = "16.01"; // IMPORTANT
#define CURRENT_VERSION 16
inline PlayLayer* g_playLayer = nullptr;

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
		SPEED,
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
		GDS,
		ZB,
        UNKNOWN
    };
};

namespace Modules {
	enum class Gameplay {
		// bool
		NOCLIP,
		NOCLIP_TINT,
		SPEEDHACK,
		SPEEDHACK_MUSIC,
		DISPLAY_PERCENTAGE,
		NO_DEATH_EFFECT,
		HIDE_ATTEMPTS,
		STARTPOS_SWITCHER,
		HIDE_PAUSE_MENU,
		NO_ROTATION,
		NO_RESPAWN_TIME,
		PRACTICE_MUSIC_HACK,
		NO_MIRROR_TRANSITION,
		SHOW_HITBOXES,
		HITBOXES_ON_DEATH,
		LAYOUT_MODE,
		LAYOUT_NO_COLOR,
		HIDE_PLAYER,
		PRACTICE_FIX,

		// float

		SPEEDHACK_VAL
	};

	enum class Editor {
		// bool
		SELECT_FILTER = 19,
		DELETE_ALL,
		SHOW_LEVEL_LENGTH,
		EDITOR_EXTENSION,
		MORE_EDITOR_BUTTONS,
		EDITOR_HITBOXES,
		AUDIO_PLAYBACK,
		TRIGGER_LENGTH,
		ZOOM_BYPASS,
		FREE_MOVE,
		UNLIMITED_OBJECTS,
		COPY_AND_PASTE,
		GRID_SNAPPING
	};

	enum class Universal {
		// bool
		DISPLAY_EPIC_ICONS = 32,
		DISPLAY_AUDIO_TRACK,
		DISABLE_SONG_ALERT,
		ADD_QUIT_BUTTON,
		HIDE_VAULT,
		GD_SHARE,
		ICON_HACK,
		NO_SCENE_TRANSITION,
		CONTROL_START_MENU_ICONS,
		MY_LEVEL_STATS
	};

	enum class Labels {
		// bool
		FPS = 42,
		DEATHS,
		TIME,
		CUSTOM,
		SPEEDHACK,
		CLICKS,
		CPS,
		CHEAT_INDICATOR,
		CLOCK,

		// string
		CUSTOM_STR
	};

	enum class Internal {
		// int
		SAVED_THEME = 52,
		COUNT
	};
};

struct Module {
    enum ValueType { B, I, F, S };
    ValueType type; 
    bool b;
    int i;
    float f;
    std::string s;

    Module(bool val) : type(B), b(val), i(0), f(0.0f) {}
    Module(int val) : type(I), b(false), i(val), f(0.0f) {}
    Module(float val) : type(F), b(false), i(0), f(val) {}
    Module(std::string val) : type(S), b(false), i(0), f(0.0f), s(val) {}
    Module() : type(B), b(false), i(0), f(0.0f) {}
};


typedef int ModuleKey;

class ExtraLayer : public CCBlockLayer, FLAlertLayerProtocol {
private:
    static inline std::map<ModuleKey, Module> module_memory;
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
	CCLayer* m_page1;
	CCLayer* m_page2;
	CCLayer* m_page3;
	CCLayer* m_page4;
	CCLayer* m_page5;
	CCLayer* m_page6;
	CCLayer* m_page7;
	static CCMenu* m_right;
	static CCMenu* m_left;
	static CCLayer* m_self;
	static CCLayer* m_lel;
	static cocos2d::ccColor3B m_flashColor;
	static bool uploadNotAllowed_;
	DropdownWidget* m_pDropdown;
	static int m_uSelectedOption;
	CCSprite* m_sCloseBtn;
	cocos2d::extension::CCScale9Sprite* m_sPanel;
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
	void new_module(const char* display, 
		int moduleID, 
		CCLayer* page, 
		CCPoint pos,
		bool safeMode = false,
		char const* info = "", 
		bool addSettings = false, 
		SEL_MenuHandler settingSelector = menu_selector(ExtraLayer::dummy));
	CCMenu* createOptionsMenu(const char* display, int length, SEL_MenuHandler func);
	static void onLoadSettings();
	void onNoclipOptions();
	void onPEEOptions();
	void onCustomLabelOptions();
	void onHitboxOptions();
	void onLayoutOptions();
	void onFreeMoveOptions();
	void nextPage();
	void prevPage();
	void volumeSliderCallback(cocos2d::CCObject* pSender, cocos2d::extension::CCControlEvent controlEvent);
	void exportSaveFile();
	void manualSave(CCObject*);
	void showUID(CCObject*);
	bool extractFile(char const*);
	void onGameplay(CCObject*);
	void onEditor(CCObject*);
	void onUniversal(CCObject*);
	void onLabels(CCObject*);
	void onTools(CCObject*);
	void onInterface(CCObject*);
	void onAbout(CCObject*);
	void disableAllPages();
	void onDropdownSelected(CCObject*);
	void updateAllTextures();
	void moduleInit();
	void crashGame(CCObject*);
	CCMenuItemSpriteExtra* createToolsBtn(char const* contents, SEL_MenuHandler func, char const* info = "");
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) override;
	static void setup_defaults();

	static void save_module(ModuleKey id, Module val) {
    	module_memory[id] = val;
	}

	static Module& module_by_id(int id) {
    	return module_memory[id]; 
	}
protected:
    bool init(CCLayer* referrer);
    void keyBackClicked();
};

#define MODULE_BY_ID(member, id) ExtraLayer::module_by_id(static_cast<int>(id)).member
#define MODULE_TO_ID(id) static_cast<int>(id)
#define SAVE_MODULE(id, val) ExtraLayer::save_module(MODULE_TO_ID(id), val)
#define GAMEPLAY m_page1
#define EDITOR m_page2
#define UNIVERSAL m_page3
#define LABELS m_page4
#define TOOLS m_page5
#define INTERFACE m_page6
#define MOD_CHEATING MODULE_BY_ID(b, Gameplay::NOCLIP) || MODULE_BY_ID(b, Gameplay::SPEEDHACK) || (MODULE_BY_ID(b, Gameplay::SHOW_HITBOXES) && !MODULE_BY_ID(b, Gameplay::HITBOXES_ON_DEATH)) || MODULE_BY_ID(b, Gameplay::LAYOUT_MODE) || MODULE_BY_ID(b, Gameplay::HIDE_PLAYER)
#define NEW_TOOLS_BTN(c, f) toolsArray->addObject(this->createToolsBtn(c, f))
#define NEW_TOOLS_INFO(c, f, i) toolsArray->addObject(this->createToolsBtn(c, f, i))
#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))
