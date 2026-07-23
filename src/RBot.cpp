// actions are deprecated and unused, maybe I'll use them in the future
#include "rbot/RBot.h"
#include "rbot/Toggler.h"
#include "rbot/MacroSaver.h"
#include "rbot/MacroLoader.h"
#include "tools.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <jni.h>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include "save.hpp"
#include <iostream>

#define MACRO_TAG 679999
#define SETTINGS_TAG 4804376
using namespace cocos2d;

std::string RBot::getVersion()
{
    return "0.911 beta";
}

bool compareFrames(Action a, Action b)
{
    return a.frame < b.frame;
}

Modules& RBot::getModules()
{
    static Modules data;
    return data;
}

std::vector<Action>& RBot::getActions()
{
    static std::vector<Action> data;
    return data;
}

std::vector<CheckpointData>& RBot::getCheckpointData()
{
    static std::vector<CheckpointData> data;
    return data;
}

std::vector<Frame>& RBot::getFrameData() {
    static std::vector<Frame> data;
    return data;
}

#define ADD_ACTION(f, s) RBot::getActions().push_back({f, s})
Action* RBot::getCurrentAction()
{
    auto& actions = getActions();
    if(actions.empty()) return nullptr;
    return &actions.front();
}

Action* RBot::getActionByNumber(int num)
{
    auto& actions = getActions();
    if(num < 0 || num >= actions.size()) return nullptr;
    return &actions[num];
}

void RBot::loadActionData()
{
    /* ADD_ACTION(50, true);
    ADD_ACTION(100, false);
    ADD_ACTION(150, true);
    ADD_ACTION(200, false);
    ADD_ACTION(210, true);
    ADD_ACTION(220, false);
    ADD_ACTION(230, true);
    ADD_ACTION(240, false);
    ADD_ACTION(500, true);
    ADD_ACTION(550, false); */
}

void RBot::reset()
{
    getModules().frame = 0;
    getModules().completed = false;
    RBot::getCheckpointData().clear();
}

void RBot::stopMacro()
{
    RBot::getModules().mode = kModeNone;
    RBot::getModules().recording = false;
    RBot::getModules().playing = false;
}

void RBot::addAction(int frame, bool push)
{
    ADD_ACTION(frame, push);
    std::sort(getActions().begin(), getActions().end(), compareFrames);
}

Action* RBot::getActionByFrame(int frame)
{
    auto& actions = getActions();
    for(auto& action : actions) {
        if(action.frame == frame) {
            return &action;
        }
    }
    return nullptr;
}

void RBot::removeAllActions()
{
    RBot::getActions().clear();
}

RBotLayer* RBotLayer::create(PauseLayer* parent)
{
    auto ret = new RBotLayer();
    ret->m_parent = parent;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCMenuItemToggler* getToggler(CCNode* node)
{
    return static_cast<CCMenuItemToggler*>(static_cast<CCMenu*>(node->getChildren()->objectAtIndex(0))->getChildren()->objectAtIndex(0));
}

void RBotLayer::toggle(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    bool* toggleVar = static_cast<bool*>(btn->getUserData());
    if(toggleVar) *toggleVar = !*toggleVar;
    switch(btn->getTag())
    {
        case 1000: 
            // getToggler(m_playToggler)->toggle(false);
            m_playToggler->toggle(false);
            break;
        case 1001:
            // getToggler(m_recordToggler)->toggle(false);
            m_recordToggler->toggle(false);
            break;
        case MACRO_TAG:
            m_settingsToggler->toggle(false);
            m_settingsToggler->setEnabled(true);
            m_macroToggler->setEnabled(false);
            m_macroToggler->toggle(true);
            setChildrenEnabled(m_macroPage, true);
            setChildrenEnabled(m_settingsPage, false);
            break;
        case SETTINGS_TAG:
            m_macroToggler->toggle(false);
            m_settingsToggler->setEnabled(false);
            m_macroToggler->setEnabled(true);
            m_settingsToggler->toggle(true);
            setChildrenEnabled(m_macroPage, false);
            setChildrenEnabled(m_settingsPage, true);
            break;
    }
}

void RBotLayer::clearMacro(CCObject* sender)
{
    RBot::removeAllActions();
    RBot::getCheckpointData().clear();
}

void RBotLayer::saveFile()
{
    auto macro = MacroSaver::create(this);
    this->addChild(macro, 1000);
}

void RBotLayer::loadFile()
{
    auto macro = MacroLoader::create(this);
    this->addChild(macro, 1000);
}

CCNode* RBotLayer::togglerFromModule(const Module& m)
{
    #if GAME_VERSION > V1P0
    auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    #else
    auto on = CCSprite::create("GJ_checkOn_001.png");
    auto off = CCSprite::create("GJ_checkOff_001.png");
    #endif

    auto val = mod::module_by_id<bool>(m.id);

    auto container = CCNode::create();

    // im too lazy to make a CCMenuItemTogglerExtExtra or whatever stay with an annoying bug at the moment
    auto btn = CCMenuItemExt::createWithToggler(
        off,
        on,
        [id = m.id](CCObject* toggler)
        {
            bool val = mod::module_by_id<bool>(id);
            mod_map[id].value = !val;
            
            if (mod_map[id].callback) mod_map[id].callback();
            // static_cast<CCMenuItemToggler*>()->toggle(val);
        }
    );
    if(val) static_cast<CCMenuItemToggler*>(btn->getWrapper())->toggle(true);

    auto target = btn->getTarget();
    auto wrapper = btn->getWrapper();
    btn->setTarget(
        [target, wrapper, id = m.id](CCObject* self)
        {
            target(self);
            static_cast<CCMenuItemToggler*>(wrapper)->toggle(mod::module_by_id<bool>(id));
        }
    );

    btn->setAnchorPoint(ccp(0, 0.5f));
    btn->setScale(0.8f);

    auto label = CCLabelBMFont::create(
        m.name.c_str(), 
        "bigFont.fnt"
    );
    label->setScale(0.5f);
    label->setAnchorPoint(ccp(0, 0.5f));
    limitLabelWidth(label, 200);
    label->setPosition(ccp(30,0));
    container->addChild(label);

    if(m.isCheat) label->setColor(ccc3(255, 66, 66));

    auto menu = CCMenu::create(btn, NULL);
    container->addChild(menu, 10);
    menu->ignoreAnchorPointForPosition(false);

    if(!m.description.empty()) {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        spr->setScale(0.4f);
        auto infoBtn = CCMenuItemExt::createWithSpriteExtra(
            spr, 
            [m, this](CCObject*)
            {
                FLAlertLayer::create(
                    this,
                    m.name.c_str(),
                    m.description.c_str(),
                    "OK",
                    nullptr,
                    300
                )->show();
            }
        );
        infoBtn->setPosition(ccp(-4.0f, btn->getContentSize().height / 2));
        menu->addChild(infoBtn);
    }
    return container;
}

CCNode* RBotLayer::createToggler(const char* text, bool* var, CCObject* target, SEL_MenuHandler callback, int tag)
{
    auto node = CCNode::create();

    #if GAME_VERSION > V1P0
    auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    #else
    auto on = CCSprite::create("GJ_checkOn_001.png");
    auto off = CCSprite::create("GJ_checkOff_001.png");
    #endif
    
    auto button = CCMenuItemToggler::create(off, on, target, callback);
    button->setTag(tag);
    button->setUserData(var);

    auto menu = CCMenu::create(button, NULL);
    menu->setPosition(CCPointZero);

    if (var && *var) button->toggle(true);

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    float width = label->getContentSize().width;
    label->setAnchorPoint(ccp(0.5, 0.5));
    if (width > 55.f) label->setScale(55.f / width);
    else label->setScale(0.25f);

    node->addChild(menu);
    node->addChild(label);
    
    label->setPosition(ccp(50, 0));

    return node;
}

bool RBotLayer::init()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_macroPage = CCLayer::create();
    m_settingsPage = CCLayer::create();

    m_settingsPage->setVisible(false);

    auto pivot = CCNode::create();
    pivot->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(pivot);

    auto contentHolder = CCNode::create();
    contentHolder->setPosition(ccp(-winSize.width/2, -winSize.height/2));

    pivot->addChild(contentHolder);
    pivot->setScale(0.1f);

    auto scaleUp = CCScaleTo::create(0.5f, 1.0f);
    auto ease = CCEaseElasticOut::create(scaleUp, 0.5f);
    pivot->runAction(ease);

    auto leftParent = CCNode::create();
    auto overlay = CCLayerColor::create(
        ccc4(0, 0, 0, 127),
        winSize.width,
        winSize.height
    );
    overlay->setPosition(0,0);
    contentHolder->addChild(overlay);
    contentHolder->addChild(leftParent);
    leftParent->setPosition(winSize.width / 2, winSize.height / 2);


    CCRect rect = CCRectMake(0, 0, 80, 80);
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("GJ_square01-hd.png", rect);
    panel->setContentSize(CCSizeMake(winSize.width - 75, winSize.height - 25));
    leftParent->addChild(panel);
    panel->setPosition({0.f, 0.f});

    auto menu = CCMenu::create();
    auto spr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(RBotLayer::keyBackClicked));
    btn->setPosition({-50,-25});
    menu->setPosition({winSize.width, winSize.height});
    menu->addChild(btn);
    contentHolder->addChild(menu, 100);

    auto macroMenu = CCMenu::create();
    macroMenu->setPosition(CCPointZero);
    m_macroPage->addChild(macroMenu);

    auto recordToggler = Toggler::create("Record", &RBot::getModules().recording, this, menu_selector(RBotLayer::toggle), 1000);
    m_recordToggler = recordToggler;
    recordToggler->setPosition(ccp(winSize.width / 2 - 90, winSize.height / 2));
    m_macroPage->addChild(recordToggler, 100000);

    auto playToggler = Toggler::create("Play", &RBot::getModules().playing, this, menu_selector(RBotLayer::toggle), 1001);
    m_playToggler = playToggler;
    playToggler->setPosition(ccp(winSize.width / 2 + 90, winSize.height / 2));
    m_macroPage->addChild(playToggler, 100000);

    auto sprOn = ButtonSprite::create(
        "Macro", 
        50, 
        0, 
        1, 
        false, 
        "bigFont.fnt", 
        "GJ_button_02.png"
    );

    auto sprOff = ButtonSprite::create(
        "Macro", 
        50, 
        0, 
        1, 
        false, 
        "bigFont.fnt", 
        "GJ_button_01.png"
    );

    m_macroToggler = CCMenuItemToggler::create(
        sprOff,
        sprOn,
        this,
        menu_selector(RBotLayer::toggle)
    );
    m_macroToggler->setTag(MACRO_TAG);
    m_macroToggler->toggle(true);

    sprOn = ButtonSprite::create(
        "Settings", 
        50, 
        0, 
        1, 
        false, 
        "bigFont.fnt", 
        "GJ_button_02.png"
    );

    sprOff = ButtonSprite::create(
        "Settings", 
        50, 
        0, 
        1, 
        false, 
        "bigFont.fnt", 
        "GJ_button_01.png"
    );

    m_settingsToggler = CCMenuItemToggler::create(
        sprOff,
        sprOn,
        this,
        menu_selector(RBotLayer::toggle)
    );
    m_settingsToggler->setTag(SETTINGS_TAG);

    menu = CCMenu::create(m_macroToggler, NULL);
    menu->addChild(m_settingsToggler);

    menu->alignItemsVerticallyWithPadding(5);
    menu->setPosition(ccp(80, winSize.height - 60));

    contentHolder->addChild(menu);
    contentHolder->addChild(m_macroPage);
    contentHolder->addChild(m_settingsPage);

    spr = ButtonSprite::create(
        "Clear Macro", 
        50, 
        0, 
        1.2f, 
        false, 
        "goldFont.fnt", 
        "GJ_button_01.png"
    );

    if (RBot::getFrameData().empty()) static_cast<CCSprite*>(spr)->setOpacity(127);

    auto clearBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            FLAlertLayer::create(
                this,
                "Warning",
                "Are you sure you want to <cr>clear</c> the macro?",
                "Cancel",
                "OK",
                300
            )->show();
        }
    );

    spr = ButtonSprite::create(
        "Save Macro", 
        50, 
        0, 
        1.2f, 
        false, 
        "goldFont.fnt", 
        "GJ_button_01.png"
    );

    if (RBot::getFrameData().empty()) static_cast<CCSprite*>(spr)->setOpacity(127);

    auto saveBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            auto layer = MacroSaver::create(this);
            this->addChild(layer, 10000);
        }
    );

    spr = ButtonSprite::create(
        "Load Macro", 
        50, 
        0, 
        1.2f, 
        false, 
        "goldFont.fnt", 
        "GJ_button_01.png"
    );

    auto loadBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            auto layer = MacroLoader::create(this);
            this->addChild(layer, 10000);
        }
    );
    menu = CCMenu::create(clearBtn, NULL);
    menu->addChild(saveBtn);
    menu->addChild(loadBtn);
    menu->setPosition(ccp(winSize.width / 2 + 20, 40));
    menu->alignItemsHorizontallyWithPadding(5);
    m_macroPage->addChild(menu);

    this->m_textInput = CCTextInputNode::create(120, 40, "Speedhack", "Thonburi", 15, "chatFont.fnt");
    MEMBER_BY_OFFSET(void*, m_textInput, 0x16c) = nullptr; 
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->setOpacity(200);
    this->m_textInput->setPosition(ccp(winSize.width / 2 + 20, winSize.height / 1.5 - 50));
    this->m_textInput->setMaxLabelScale(1);
    this->m_textInput->setLabelPlaceholderScale(1);
    this->m_textInput->setScale(0.5f);
    setCharLimit(this->m_textInput, 3);
    setAllowedChars(this->m_textInput, "0123456789.");
    this->m_textInput->setAnchorPoint({0, 0});
    this->m_textInput->setString(fmt::format("{}", mod::module_by_id<float>(id::speedhack_val)).c_str());
    m_settingsPage->addChild(this->m_textInput, 100);

    auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(m_textInput->getContentSize().width + 20, 60));
    bg->_setZOrder(m_textInput->getZOrder() - 1);
    bg->setPosition(ccp(m_textInput->getPositionX() - 10, m_textInput->getPositionY()));
    bg->setOpacity(67);
    m_settingsPage->addChild(bg);
    bg->setScale(0.5f);

    spr = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    spr->setPosition(ccp(30, 30));
    spr->setScale(1.5f);
    bg->addChild(spr);

    if(mod::module_by_id<bool>(id::speedhack))
    {
        this->m_textInput->setVisible(true);
        bg->setVisible(true);
    } else {
        this->m_textInput->setVisible(false);
        bg->setVisible(false);
    }

    mod::set_target(
        id::speedhack,
        [this, bg]()
        {
            if(mod::module_by_id<bool>(id::speedhack))
            {
                this->m_textInput->setVisible(true);
                bg->setVisible(true);
            } else {
                this->m_textInput->setVisible(false);
                bg->setVisible(false);
            }
        }
    );

    float y = 15 * mod_map.size();

    for(const auto mod : mod_map)
    {
        if(mod.second.category != category::menu) continue;
        auto toggle = togglerFromModule(mod.second);
        toggle->setPosition(winSize.width / 4 - 10, winSize.height / 2 + 60 - y);
        y += -30;
        m_settingsPage->addChild(toggle);
    }


    m_label = CCLabelBMFont::create(fmt::format("Size: {}", RBot::getFrameData().size()).c_str(), "chatFont.fnt");
    m_label->setColor(ccc3(0,0,0));
    m_label->setOpacity(67);
    m_label->setPosition(ccp(winSize.width / 2 + 20, 65));
    m_label->setScale(.5f);
    m_macroPage->addChild(m_label);

    spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*){
            FLAlertLayer::create(
                nullptr,
                "About",
                fmt::format(
                "<cy>Retro Bot</c> \n Developed by nano \n \n <cg>Version:</c> {} \n <cp>Special Thanks:</c> AntiMatter, aloaf, hris, Mysph",
                RBot::getVersion()
                ).c_str(),
                "OK",
                nullptr,
                300
            )->show();
        }
    );
    infoBtn->setPosition(ccp(winSize.width - 60, 30));
    menu = CCMenu::create(infoBtn, NULL);
    menu->setPosition(CCPointZero);
    contentHolder->addChild(menu);

    CCArray* childrenself = this->m_parent->getChildren(); 

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(childrenself, pObj)
    {
        CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
        if (menu) menu->setEnabled(false);
    }
    this->setKeypadEnabled(true);
    return true;
}

void RBotLayer::updateLabel()
{
    m_label->setString(fmt::format("Size: {}", RBot::getFrameData().size()).c_str());
}

void RBotLayer::FLAlert_Clicked(FLAlertLayer*, bool btn2)
{
    if(btn2)
    {
        RBot::getFrameData().clear();
        updateLabel();
    }
}

void RBotLayer::keyBackClicked() {
    CCArray* childrenself = this->m_parent->getChildren();

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(childrenself, pObj)
    {
        if (auto menu = dynamic_cast<CCMenu*>(pObj)) menu->setEnabled(true); 
    }
    #if GAME_VERSION < V1P7
    auto str = std::string(MEMBER_BY_OFFSET(CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->getString());
    #else
    auto res = MEMBER_BY_OFFSET(CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->m_pInputText;
    auto str = (res != nullptr) ? *res : "1.0"; 
    #endif
    std::stringstream ss(str);
    float f;
    ss >> f;

    mod_map[id::speedhack_val] = f;

    if(RBot::getModules().playing) RBot::getModules().mode = Modes::kModePlaying;
    else if(RBot::getModules().recording) RBot::getModules().mode = Modes::kModeRecording;
    this->removeFromParentAndCleanup(true);
    mod::save();
}