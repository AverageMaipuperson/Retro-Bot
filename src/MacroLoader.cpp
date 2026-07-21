#include "rbot/RBot.h"
#include "rbot/MacroLoader.h"
#include "tools.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <jni.h>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include "fmt/format.h"
using namespace cocos2d;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

MacroLoader* MacroLoader::create(RBotLayer* parent) {
    auto ret = new MacroLoader();
    ret->m_parent = parent;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MacroLoader::loadFile(char const* macroName) {
    /* cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, 
        "com/robtopx/geometryjump/GeometryJump", 
        "openFolderSelector", 
        "()V")) 
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    } */
   std::ifstream inputFile{ std::string(macroName), std::ios::binary };
    if (!inputFile.is_open()) {
        FLAlertLayer::create(
                nullptr,
                "Error",
                "Could not load the macro file. <cg>Try checking if you gave the app storage permissions.</c>",
                "OK",
                nullptr,
                300.f
            )->show();
        return;
    }
    char header[4];
    inputFile.read(header, 4);

    if (!inputFile || std::memcmp(header, "rbot", 4) != 0) {
        FLAlertLayer::create(
                nullptr,
                "Error",
                "Could not load the macro file. <cr>fatal:</c> corrupted file",
                "OK",
                nullptr,
                300.f
            )->show();
        return;
    }
    size_t size;
    inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
    if(size > 99999)
    {
        FLAlertLayer::create(
                nullptr,
                "Error",
                "Could not load the macro file. <cr>fatal:</c> macro too big. will cause memory exhaustion",
                "OK",
                nullptr,
                300.f
            )->show();
        return;
    }
    auto& actions = RBot::getFrameData();
    actions.clear();
    actions.resize(size);
    inputFile.read(reinterpret_cast<char*>(actions.data()), size * sizeof(Frame));

    inputFile.close();

    // MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, this->m_parent->m_textInput, CCTextInputNode__m_textField)->setString(CCString::createWithFormat("%.1f", RBot::getModules().tps)->getCString());

    FLAlertLayer::create(
        nullptr, 
        "Success", 
        "Macro loaded successfully!", 
        "OK", 
        nullptr, 
        300.f
    )->show();

    m_parent->updateLabel();
}

std::vector<MacroFile> MacroLoader::getMacros()
{
    std::vector<MacroFile> vector;

    auto* b_dir = opendir("/sdcard/rbot/macros/");
    if(b_dir == nullptr) return vector;

    struct dirent* entry;
    std::string dir = "/sdcard/rbot/macros/";
    while ((entry = readdir(b_dir)) != nullptr)
    {
        std::string name = entry->d_name;

        if (name.size() >= 4 && name.compare(name.size() - 4, 4, ".gdr") == 0)
        {
            std::string path = dir + name;
            struct stat fileStat;
            long long size = 0;

            if (stat(path.c_str(), &fileStat) == 0) size = fileStat.st_size;

            vector.push_back({path, name, size});
        }
    }
    closedir(b_dir);
    return vector;
}

bool MacroLoader::init() 
{
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
    CCLayerColor *overlay = CCLayerColor::create(
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
    auto btn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(MacroLoader::keyBackClicked));
    btn->setPosition({-50,-25});
    menu->setPosition({winSize.width, winSize.height});
    menu->addChild(btn);
    contentHolder->addChild(menu, 100);

    CCArray* childrenself = this->m_parent->getChildren(); 

    CCObject* pObj = NULL;
    CCARRAY_FOREACH(childrenself, pObj) {
    CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
        if (menu) menu->setEnabled(false); 
    }

    int y = 0;
    int i = 0;
    m_mainLayer = CCLayer::create();

    m_files = getMacros();
    for (const auto& file : m_files)
    {
        if(i > 5) break;
        panel = cocos2d::extension::CCScale9Sprite::create("square02_small.png", {0,0,40,40});
        panel->setContentSize(CCSizeMake(winSize.width / 2, 40));
        panel->setPosition(ccp(winSize.width / 2, winSize.height - 50 - y));
        panel->setOpacity(127);

        auto label = CCLabelBMFont::create(file.name.c_str(), "goldFont.fnt");
        limitLabelWidth(label, 200);
        label->setPosition(ccp(20, 20));
        label->setAnchorPoint(ccp(0, .5f));
        label->setScale(.5f);
        panel->addChild(label);

        auto spr = ButtonSprite::create(
            "Load", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
        );
        spr->setScale(.75f);

        auto btn = CCMenuItemExt::createWithSpriteExtra(
            spr,
            [this, file](CCObject*)
            {
                auto alert = FLAlertLayer::create(
                    this,
                    "Warning",
                    fmt::format("Are you sure you want to load <cg>{}</c>? <cr>Your current macro will be overwritten</c>.", file.name).c_str(),
                    "Cancel",
                    "OK",
                    300
                );
                auto it = std::find(m_files.begin(), m_files.end(), file);
                if (it != m_files.end()) alert->setTag(std::distance(m_files.begin(), it));
                alert->show();
            }
        );

        auto menu = CCMenu::create(btn, NULL);
        menu->setPosition(ccp(panel->getContentSize().width - 40, 20));
        
        panel->addChild(menu);
        m_mainLayer->addChild(panel);
        y += 45;
        i++;
    }

    contentHolder->addChild(m_mainLayer);

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    spr->setFlipX(true);
    auto btn_ext = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            this->changePage(m_page + 1);
        }
    );
    btn_ext->setPosition(ccp(winSize.width / 2 - 70, 0));
    menu = CCMenu::create(btn_ext, NULL);

    spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    btn_ext = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            this->changePage(m_page - 1);
        }
    );
    btn_ext->setPosition(ccp(-winSize.width / 2 + 70, 0));
    menu->addChild(btn_ext);
    contentHolder->addChild(menu);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    setAllMenusEnabled(m_parent, false);
    return true;
}

void MacroLoader::changePage(int page)
{
    int pages = std::ceil(static_cast<double>(m_files.size()) / 6);
    if (m_mainLayer && page > -1 && page < pages)
    {
        m_page = page;
        m_mainLayer->removeAllChildrenWithCleanup(true);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        int y = 0;

        // std::reverse(m_files.begin(), m_files.end());
        int start_index = page * 6;
        int end_index = std::min(start_index + 6, static_cast<int>(m_files.size()));

        for (int i = page * 6 ; i < end_index ; i++)
        {
            if(i > m_files.size()) break;
            auto file = m_files[i];

            auto panel = cocos2d::extension::CCScale9Sprite::create("square02_small.png", {0,0,40,40});
            panel->setContentSize(CCSizeMake(winSize.width / 2, 40));
            panel->setPosition(ccp(winSize.width / 2, winSize.height - 50 - y));
            panel->setOpacity(127);

            auto label = CCLabelBMFont::create(file.name.c_str(), "goldFont.fnt");
            limitLabelWidth(label, 200);
            label->setPosition(ccp(20, 20));
            label->setAnchorPoint(ccp(0, .5f));
            label->setScale(.5f);
            panel->addChild(label);

            auto spr = ButtonSprite::create(
                "Load", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
            );
            spr->setScale(.75f);

            auto btn = CCMenuItemExt::createWithSpriteExtra(
                spr,
                [this, file](CCObject*)
                {
                    auto alert = FLAlertLayer::create(
                        this,
                        "Warning",
                        fmt::format("Are you sure you want to load <cg>{}</c>? <cr>Your current macro will be overwritten</c>.", file.name).c_str(),
                        "Cancel",
                        "OK",
                        300
                    );
                    auto it = std::find(m_files.begin(), m_files.end(), file);
                    if (it != m_files.end()) alert->setTag(std::distance(m_files.begin(), it));
                    alert->show();
                }
            );

            auto menu = CCMenu::create(btn, NULL);
            menu->setPosition(ccp(panel->getContentSize().width - 40, 20));
        
            panel->addChild(menu);
            m_mainLayer->addChild(panel);
            y += 45;
        }
    }
}

void MacroLoader::FLAlert_Clicked(FLAlertLayer* alert, bool btn2)
{
    if(btn2) this->loadFile(m_files[alert->getTag()].path.c_str());
}

void MacroLoader::keyBackClicked() {
    setAllMenusEnabled(m_parent, true);
    this->removeFromParentAndCleanup(true);
}