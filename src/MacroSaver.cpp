#include "rbot/RBot.h"
#include "rbot/MacroSaver.h"
#include "tools.hpp"
#include "robtop/ButtonSprite.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <jni.h>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include "fmt/format.h"
#include "PlayLayer.hpp"
#include "GameManager.hpp"
#include "tools.hpp"
#include "CCMenuItemExt.h"
using namespace cocos2d;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

MacroSaver* MacroSaver::create(RBotLayer* parent)
{
    auto ret = new MacroSaver();
    ret->m_parent = parent;

    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MacroSaver::saveFile()
{
    if(RBot::getFrameData().empty())
    {
        FLAlertLayer::create(
            nullptr,
            "Error",
            "Your macro is <cr>empty</c>",
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }
    #if GAME_VERSION < V1P7
    auto name = std::string(MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->getString());
    #else
    auto res = MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->m_pInputText;
    auto name = (res != nullptr) ? *res : "level"; 
    #endif
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    create_directories("/sdcard/rbot/macros/");
    std::ofstream outputFile("/sdcard/rbot/macros/" + std::string(name) + ".gdr");
    if (!outputFile.is_open())
    {
        FLAlertLayer::create(
            nullptr,
            "Error",
            "Failed to save the macro file.",
            "OK",
            nullptr,
            300.f
        )->show();
        return;
    }

    outputFile.write("rbot", 4);
    const auto& actions = RBot::getFrameData();
    auto size = actions.size();
    outputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
    outputFile.write(reinterpret_cast<const char*>(actions.data()), size * sizeof(Action));
    
    outputFile.close();
    FLAlertLayer::create(
        nullptr,
        "Success",
        CCString::createWithFormat("Macro saved to <cg>/sdcard/gdr/macros/%s.gdr</c>", name.c_str())->getCString(),
        "OK",
        nullptr,
        300.f
    )->show();
}

bool MacroSaver::init()
{
    CCArray* childrenself = this->m_parent->getChildren(); 

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(childrenself, pObj)
    {
        CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
        if (menu) menu->setEnabled(false); 
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();

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
    panel->setContentSize(CCSizeMake(winSize.width - 75, winSize.height / 2));
    leftParent->addChild(panel);

    this->m_textInput = CCTextInputNode::create(panel->getContentSize().width, 40.0, "Macro Name", "Thonburi", 15, "chatFont.fnt");
    MEMBER_BY_OFFSET(void*, m_textInput, 0x16c) = nullptr; 
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_textInput, CCTextInputNode__m_textField)->setOpacity(200);
    this->m_textInput->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
    this->m_textInput->setMaxLabelScale(1);
    this->m_textInput->setLabelPlaceholderScale(1);
    this->m_textInput->setScale(0.5f);
    setCharLimit(this->m_textInput, 20);
    setAllowedChars(this->m_textInput, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789. ");
    this->m_textInput->setAnchorPoint({0, 0});
    contentHolder->addChild(this->m_textInput, 100);
    this->m_textInput->setString(fmt::format("{}", GameManager::sharedState()->getPlayLayer()->getLevel()->m_sLevelName).c_str());

    auto label = CCLabelBMFont::create(".gdr", "chatFont.fnt");
    label->setPosition(ccp(winSize.width / 1.5 + 40, m_textInput->getPosition().y));
    label->setScale(.5f);
    contentHolder->addChild(label);

    auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(m_textInput->getContentSize().width, 60));
    bg->_setZOrder(m_textInput->getZOrder() - 1);
    bg->setPosition(m_textInput->getPosition());
    bg->setOpacity(127);
    contentHolder->addChild(bg);
    bg->setScale(0.5f);

    auto toolsMenu = CCMenu::create();

    auto saveBtnSprite = ButtonSprite::create(
        "Save Macro", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
    );
  
    auto saveBtn = CCMenuItemSpriteExtra::create(
        saveBtnSprite,
        saveBtnSprite,
        this,
        menu_selector(MacroSaver::saveFile)
    );
    saveBtn->setPosition(CCPointZero); 
    auto cancelBtnSprite = ButtonSprite::create(
        "Cancel", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
    );
  
    auto cancelBtn = CCMenuItemSpriteExtra::create(
        cancelBtnSprite,
        cancelBtnSprite,
        this,
        menu_selector(MacroSaver::keyBackClicked)
    );
    toolsMenu->addChild(cancelBtn);
    toolsMenu->addChild(saveBtn);
    toolsMenu->setPosition(CCPointZero);
    toolsMenu->alignItemsHorizontallyWithPadding(5);
    toolsMenu->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 60));

    contentHolder->addChild(toolsMenu, 100);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto btn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [](CCObject*)
        {
            FLAlertLayer::create(
                nullptr,
                "Info",
                "All macro files are saved in <cl>/storage/emulated/0/rbot/macros/</c>.\nIn the input you <cr>must</c> insert the name of the file only, without the .gdr",
                "OK",
                nullptr,
                300
            )->show();
        }
    );
    auto menu = CCMenu::create(btn, NULL);
    menu->setPosition(ccp(winSize.width - 40, panel->getContentSize().height + 75));
    contentHolder->addChild(menu);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    setAllMenusEnabled(m_parent, false);
    return true;
}

void MacroSaver::keyBackClicked()
{
    setAllMenusEnabled(m_parent, true);
    this->removeFromParentAndCleanup(true);
}