#pragma once
#include "rbot.hpp"
#include "Toggler.hpp"
#include "mod.hpp"
#include "RBotAlertLayer.hpp"

class RBotLayer : public RBotAlertLayer, public CCTextFieldDelegate, public FLAlertLayerProtocol {
public:
    PauseLayer* m_parent;
    CCMenuItemToggler* m_recordToggler;
    CCMenuItemToggler* m_playToggler;
    CCTextInputNode* m_textInput;
    CCLayer* m_aboutPage;
    CCLayer* m_macroPage;
    CCLayer* m_settingsPage;
    CCLayer* m_settingsPage1;
    CCLayer* m_settingsPage2;
    CCMenuItemToggler* m_macroToggler;
    CCMenuItemToggler* m_settingsToggler;
    CCMenuItemToggler* m_aboutToggler;
    CCLabelBMFont* m_label;
    CCMenu* m_soundsMenu;
    CCMenuItemSpriteExtra* m_nextBtn;
    CCMenuItemSpriteExtra* m_prevBtn;

    static RBotLayer* create(PauseLayer*);
    void updateLabel();
protected:
    bool init() override;
    void initAbout();
    void initMacro();
    void initSettings();
    void initSideButtons();
    void toggle(CCObject*);

    void clearMacro(CCObject*);
    void saveFile();
    void loadFile();

    void turnOffSideButtons();

    CCNode* togglerFromModule(const Module&);
    CCNode* createToggler(const char* text, bool* var, CCObject* target, SEL_MenuHandler callback, int tag);
    CCMenuItemToggler* createSideButton(char const* text, const CCMenuItemExt::Callback& callback);

    virtual void keyBackClicked() override;
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) override;
};