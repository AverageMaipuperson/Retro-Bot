// actions are deprecated and unused, maybe I'll use them in the future
#include "rbot.hpp"
#include "Toggler.hpp"
#include "MacroSaver.hpp"
#include "MacroLoader.hpp"
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
#include "ClickLoader.hpp"

#define MACRO_TAG 679999
#define SETTINGS_TAG 4804376
using namespace cocos2d;

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

void RBotLayer::toggle(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    bool* toggleVar = static_cast<bool*>(btn->getUserData());
    if(toggleVar) *toggleVar = !*toggleVar;
    switch(btn->getTag())
    {
        case 1000:
            m_playToggler->toggle(false);
            break;
        case 1001:
            m_recordToggler->toggle(false);
            break;
    }
}

void RBotLayer::clearMacro(CCObject* sender)
{
    rbot::getCheckpointData().clear();
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
    auto on = CCSprite::create("rbot_toggleOn.png");
    on->setScale(.5f);
    auto off = CCSprite::create("rbot_toggleOff.png");
    off->setScale(.5f);
    auto container = CCNode::create();

    auto val = mod::module_by_id<bool>(m.id);
    auto btn = CCMenuItemExt::createWithToggler(
        off,
        on,
        [id = m.id](CCObject* toggler)
        {
            bool val = mod::module_by_id<bool>(id);
            mod_map[id].value = !val;
            
            if (mod_map[id].callback) mod_map[id].callback();
        }
    );

    if(val) static_cast<CCMenuItemToggler*>(btn->getWrapper())->toggle(true);
    btn->setAnchorPoint(ccp(0, 0.5f));
    btn->setScale(0.8f);

    auto label = CCLabelBMFont::create(
        m.name.c_str(), 
        "BoonBoldItalic.fnt"
    );
    label->setAnchorPoint(ccp(0, 0.5f));
    limitLabelWidth(label, 200);
    label->setPosition(ccp(30,0));
    container->addChild(label);

    // if(m.isCheat) label->setColor(ccc3(255, 66, 66));

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

CCMenuItemExt* RBotLayer::createSideButton(char const* text, const CCMenuItemExt::Callback& callback)
{
    auto on = ButtonSprite::create(
        text, 
        120, 
        60, 
        1, 
        false, 
        "BoonBoldItalic.fnt", 
        "rbot_btn02.png"
    );

    auto off = ButtonSprite::create(
        text, 
        120, 
        60, 
        1, 
        false, 
        "BoonBoldItalic.fnt", 
        "rbot_btn01.png"
    );

    auto btn = CCMenuItemExt::createWithToggler(
        off,
        on,
        callback
    );

    return btn;
}

void RBotLayer::turnOffSideButtons()
{
    for(auto btn : std::vector<CCMenuItemExt*>{m_macroToggler, m_settingsToggler, m_aboutToggler})
    {
        static_cast<CCMenuItemToggler*>(btn->getWrapper())->toggle(false);
        static_cast<CCMenuItemToggler*>(btn->getWrapper())->setEnabled(true);
    }

    for(auto layer : std::vector<CCLayer*>{m_macroPage, m_settingsPage, m_aboutPage})
    {
        layer->setVisible(false);
        setChildrenEnabled(layer, false);
    }
}

bool RBotLayer::init()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    rbot::getModules().rbotOpened = true;

    m_aboutPage = CCLayer::create();
    m_macroPage = CCLayer::create();
    m_settingsPage = CCLayer::create();
    m_settingsPage1 = CCLayer::create();
    m_settingsPage2 = CCLayer::create();

    m_settingsPage->setVisible(false);
    m_settingsPage2->setVisible(false);
    m_aboutPage->setVisible(false);

    auto pivot = CCNode::create();
    pivot->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(pivot);

    auto contentHolder = CCLayer::create();
    contentHolder->setPosition(ccp(-winSize.width/2, -winSize.height/2));
    m_mainLayer = contentHolder;

    pivot->addChild(contentHolder);
    pivot->setScale(0.1f);

    auto scaleUp = CCScaleTo::create(0.5f, 1.0f);
    auto ease = CCEaseElasticOut::create(scaleUp, 0.5f);
    pivot->runAction(ease);

    auto leftParent = CCNode::create();
    leftParent->setPosition(winSize.width / 2, winSize.height / 2);

    auto overlay = CCLayerColor::create(
        ccc4(0, 0, 0, 127),
        winSize.width,
        winSize.height
    );
    overlay->setPosition(ccp(0,0));

    contentHolder->addChild(overlay);
    contentHolder->addChild(leftParent);
    contentHolder->addChild(m_macroPage);
    contentHolder->addChild(m_settingsPage);
    contentHolder->addChild(m_aboutPage);

    initAbout();
    initMacro();
    initSettings();
    initSideButtons();

    CCRect rect = CCRectMake(0, 0, 80, 80);
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("rbot_square.png", rect);
    panel->setContentSize(CCSizeMake(winSize.width - 75, winSize.height - 25));
    leftParent->addChild(panel);
    panel->setPosition(ccp(0,0));

    auto menu = CCMenu::create();
    auto spr = CCSprite::create("rbot_close.png");
    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(RBotLayer::keyBackClicked));
    btn->setPosition({-50,-25});
    menu->setPosition({winSize.width, winSize.height});
    menu->addChild(btn);
    contentHolder->addChild(menu, 100);

    CCLog("1");

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

void RBotLayer::initAbout()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto btnspr = ButtonSprite::create(
        "Credits", 
        120, 
        60, 
        1, 
        false, 
        "BoonBoldItalic.fnt", 
        "rbot_btn01.png"
    );

    auto infoBtn = CCMenuItemExt::createWithSpriteExtra(
        btnspr,
        [this](CCObject*){
            FLAlertLayer::create(
                nullptr,
                "Credits",
                fmt::format(
                fmt::format("<cg>Version: {}</c> \n <cy>Special Thanks:</c> AntiMatter, Mysph, Elektrick, Hris, Aloaf \n <cb>Libraries used:</c> rapidjson, Dobby, linbany, fmt, KittyMemory  \n \n <co>Thank you for playing!</c>", rbot::getVersion()),
                rbot::getVersion()
                ).c_str(),
                "OK",
                nullptr,
                300
            )->show();
        }
    );
    infoBtn->setPosition(ccp(80, 30));

    auto menu = CCMenu::create(infoBtn, NULL);
    menu->setPosition(CCPointZero);
    m_aboutPage->addChild(menu);

    auto label = CCLabelBMFont::create("Socials:", "BoonBoldItalic.fnt");
    label->setPosition(winSize.width / 2, 60);
    m_aboutPage->addChild(label);

    auto spr = CCSprite::create("discordIcon.png");
    auto discordBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [](CCObject*)
        {
        openURL("https://discord.gg/bMpBqMMF2w");
        }
    );
    discordBtn->setPosition(ccp(winSize.width / 2, 35));
    menu->addChild(discordBtn);
}

void RBotLayer::initMacro()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto macroMenu = CCMenu::create();
    macroMenu->setPosition(CCPointZero);
    m_macroPage->addChild(macroMenu);

    auto recordToggler = Toggler::create("Record", &rbot::getModules().recording, this, menu_selector(RBotLayer::toggle), 1000);
    m_recordToggler = recordToggler;
    recordToggler->setPosition(ccp(winSize.width / 2 - 90, winSize.height / 2));
    m_macroPage->addChild(recordToggler, 100000);

    auto playToggler = Toggler::create("Play", &rbot::getModules().playing, this, menu_selector(RBotLayer::toggle), 1001);
    m_playToggler = playToggler;
    playToggler->setPosition(ccp(winSize.width / 2 + 90, winSize.height / 2));
    m_macroPage->addChild(playToggler, 100000);

    auto spr = ButtonSprite::create(
        "Clear Macro", 
        50, 
        0, 
        1.2f, 
        false, 
        "BoonBoldItalic.fnt", 
        "rbot_btn01.png"
    );

    if (rbot::getFrameData().empty()) static_cast<CCSprite*>(spr)->setOpacity(127);

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
        "BoonBoldItalic.fnt", 
        "rbot_btn01.png"
    );

    if (rbot::getFrameData().empty()) static_cast<CCSprite*>(spr)->setOpacity(127);

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
        "BoonBoldItalic.fnt", 
        "rbot_btn01.png"
    );

    auto loadBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            auto layer = MacroLoader::create(this);
            this->addChild(layer, 10000);
        }
    );
    auto menu = CCMenu::create(clearBtn, NULL);
    menu->addChild(saveBtn);
    menu->addChild(loadBtn);
    menu->setPosition(ccp(winSize.width / 2 + 20, 40));
    menu->alignItemsHorizontallyWithPadding(5);
    m_macroPage->addChild(menu);

    m_label = CCLabelBMFont::create(fmt::format("Size: {}", rbot::getFrameData().size()).c_str(), "BoonBoldItalic.fnt");
    m_label->setOpacity(127);
    m_label->setPosition(ccp(winSize.width / 2 + 20, 65));
    m_label->setScale(.9f);
    m_macroPage->addChild(m_label);
}

void RBotLayer::initSettings()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto y = winSize.height / 2 + 20;
    auto y2 = winSize.height / 2 + 20;
    int mike = 0;

    for(const auto mod : mod_map)
    {
        mike++;
        if(mod.second.category != category::menu || mike > 10) continue;

        auto toggle = togglerFromModule(mod.second);
        toggle->setPosition(winSize.width / 4 - 10, winSize.height / 2 + 60 - y);
        y += -30;
        m_settingsPage1->addChild(toggle);
    }

    for(const auto mod : mod_map)
    {
        if(mod.second.category == category::menu_page_2)
        {
            auto toggle = togglerFromModule(mod.second);
            toggle->setPosition(winSize.width / 4 - 10, winSize.height / 2 + 60 - y2);
            y2 += -30;
            m_settingsPage2->addChild(toggle);
        }
    }

    this->m_textInput = CCTextInputNode::create(120, 40, "Speedhack", "Thonburi", 15, "chatFont.fnt");
    MEMBER_BY_OFFSET(void*, m_textInput, 0x16c) = nullptr; 
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->setOpacity(200);
    this->m_textInput->setPosition(ccp(winSize.width / 2 - 20, winSize.height / 4 + 20));
    this->m_textInput->setMaxLabelScale(1);
    this->m_textInput->setLabelPlaceholderScale(1);
    this->m_textInput->setScale(0.5f);
    setCharLimit(this->m_textInput, 3);
    setAllowedChars(this->m_textInput, "0123456789.");
    this->m_textInput->setAnchorPoint({0, 0});
    this->m_textInput->setString(fmt::format("{}", mod::module_by_id<float>(id::speedhack_val)).c_str());
    m_settingsPage1->addChild(this->m_textInput, 100);

    auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(m_textInput->getContentSize().width + 20, 60));
    bg->_setZOrder(m_textInput->getZOrder() - 1);
    bg->setPosition(ccp(m_textInput->getPositionX() - 10, m_textInput->getPositionY()));
    bg->setOpacity(67);
    m_settingsPage1->addChild(bg);
    bg->setScale(0.5f);
    bg->setColor(ccWHITE);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
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

    spr = ButtonSprite::create(
        "Load Sound", 100, 0, 1, false, "BoonBoldItalic.fnt", "rbot_btn01.png"
    );
    spr->setScale(.75f);

    auto soundBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            auto layer = ClickLoader::create(this);
            this->addChild(layer, 10000);
        }
    );
    soundBtn->setPosition(ccp(winSize.width / 2 - 60, winSize.height - 40));
    soundBtn->setAnchorPoint(ccp(0, .5));

    m_soundsMenu = CCMenu::create(soundBtn, NULL);
    m_soundsMenu->setPosition(CCPointZero);
    m_settingsPage1->addChild(m_soundsMenu);

    if(mod::module_by_id<bool>(id::click_sounds))
    {
        this->m_soundsMenu->setVisible(true);
        this->m_soundsMenu->setEnabled(true);
    } else {
        this->m_soundsMenu->setVisible(false);
        this->m_soundsMenu->setEnabled(false);
    }

    mod::set_target(
        id::click_sounds,
        [this, bg]()
        {
            if(mod::module_by_id<bool>(id::click_sounds))
            {
                this->m_soundsMenu->setVisible(true);
                this->m_soundsMenu->setEnabled(true);
            } else {
                this->m_soundsMenu->setVisible(false);
                this->m_soundsMenu->setEnabled(false);
            }
        }
    );

    auto menu = CCMenu::create();
    menu->setPosition(ccp(0,0));

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");

    m_prevBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            m_nextBtn->setEnabled(true);
            m_nextBtn->setVisible(true);
            m_prevBtn->setEnabled(false);
            m_prevBtn->setVisible(false);

            m_settingsPage1->setVisible(true);
            setChildrenEnabled(m_settingsPage1, true);

            m_settingsPage2->setVisible(false);
            setChildrenEnabled(m_settingsPage2, false);
        }
    );
    m_prevBtn->setPosition(ccp(80, winSize.height / 2));
    m_prevBtn->setEnabled(false);
    m_prevBtn->setVisible(false);
    menu->addChild(m_prevBtn);

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    spr->setFlipX(true);

    m_nextBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            m_nextBtn->setEnabled(false);
            m_nextBtn->setVisible(false);
            m_prevBtn->setEnabled(true);
            m_prevBtn->setVisible(true);

            m_settingsPage1->setVisible(false);
            setChildrenEnabled(m_settingsPage1, false);

            m_settingsPage2->setVisible(true);
            setChildrenEnabled(m_settingsPage2, true);
        }
    );
    m_nextBtn->setPosition(ccp(winSize.width - 80, winSize.height / 2));
    menu->addChild(m_nextBtn);

    m_settingsPage->addChild(menu);
    m_settingsPage->addChild(m_settingsPage1);
    m_settingsPage->addChild(m_settingsPage2);
}

void RBotLayer::initSideButtons()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_macroToggler = this->createSideButton(
        "Macro",
        [this](CCObject*)
        {
            this->turnOffSideButtons();
            static_cast<CCMenuItemToggler*>(m_macroToggler->getWrapper())->toggle(true);
            static_cast<CCMenuItemToggler*>(m_macroToggler->getWrapper())->setEnabled(false);

            m_macroPage->setVisible(true);
            setChildrenEnabled(m_macroPage, true);
        }
    );

    m_settingsToggler = this->createSideButton(
        "Settings",
        [this](CCObject*)
        {
            this->turnOffSideButtons();
            static_cast<CCMenuItemToggler*>(m_settingsToggler->getWrapper())->toggle(true);
            static_cast<CCMenuItemToggler*>(m_settingsToggler->getWrapper())->setEnabled(false);

            m_settingsPage->setVisible(true);
            setChildrenEnabled(m_settingsPage, true);
        }
    );

    m_aboutToggler = this->createSideButton(
        "About",
        [this](CCObject*)
        {
            this->turnOffSideButtons();
            static_cast<CCMenuItemToggler*>(m_aboutToggler->getWrapper())->toggle(true);
            static_cast<CCMenuItemToggler*>(m_aboutToggler->getWrapper())->setEnabled(false);

            m_aboutPage->setVisible(true);
            setChildrenEnabled(m_aboutPage, true);
        }
    );

    static_cast<CCMenuItemToggler*>(m_macroToggler->getWrapper())->toggle(true);
    static_cast<CCMenuItemToggler*>(m_macroToggler->getWrapper())->setEnabled(false);

    auto menu = CCMenu::create(m_macroToggler, m_settingsToggler, m_aboutToggler, NULL);
    menu->alignItemsVerticallyWithPadding(5);
    menu->setPosition(ccp(80, winSize.height - 85));
    m_mainLayer->addChild(menu);

    auto spr = CCSprite::create("rbot_logo.png");
    spr->setPosition(ccp(80, winSize.height - 30));
    spr->setScale(.8f);
    m_mainLayer->addChild(spr);
}

void RBotLayer::updateLabel()
{
    m_label->setString(fmt::format("Size: {}", rbot::getFrameData().size()).c_str());
}

void RBotLayer::FLAlert_Clicked(FLAlertLayer*, bool btn2)
{
    if(btn2)
    {
        rbot::getFrameData().clear();
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

    if(f < 0.1 || f > 999) f = 1;

    mod_map[id::speedhack_val] = f;

    if(rbot::getModules().playing) rbot::getModules().mode = Modes::kModePlaying;
    else if(rbot::getModules().recording) rbot::getModules().mode = Modes::kModeRecording;
    rbot::getModules().rbotOpened = false;
    this->removeFromParentAndCleanup(true);
    mod::save();
}