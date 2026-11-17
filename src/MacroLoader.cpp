#include "rbot.hpp"
#include "MacroLoader.hpp"
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
#include "rapidjson/document.h"
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

std::vector<Frame> MacroLoader::readJson(char const* jsonContent)
{
    std::vector<Frame> frames;
    rapidjson::Document document;

    document.Parse<rapidjson::kParseFullPrecisionFlag>(jsonContent);
    if (!document.IsObject()) return frames;

    for (auto it = document.MemberBegin(); it != document.MemberEnd(); ++it)
    {
        rapidjson::Value& obj = it->value;

        Frame frame;
        char* ptr;
        
        frame.frame = std::strtod(it->name.GetString(), &ptr);
        frame.click = static_cast<bool>(obj["click"].GetBool());
        frame.release = static_cast<bool>(obj["release"].GetBool());

        frames.push_back(frame);
    }

    return frames;
}

void MacroLoader::loadFile(char const* macroName) {
    std::ifstream inputFile{ std::string(macroName), std::ios::in };
    if (!inputFile.is_open()) {
        FLAlertLayer::create(
            nullptr, 
            "Error", 
            "Could not load the macro file.", 
            "OK", 
            nullptr, 
            300.f
        )->show();
        return;
    }
    std::string json((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    auto& actions = rbot::getFrameData();
    actions = readJson(json.c_str());

    if(actions.empty()) {
        FLAlertLayer::create(
            nullptr, 
            "Error", 
            "Macro data was empty or invalid.", 
            "OK", 
            nullptr, 
            300.f
        )->show();
        return;
    }

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

std::vector<File> MacroLoader::getMacros()
{
    std::vector<File> vector;

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
    RBotAlertLayer::init({0, 0, winSize.width - 75, winSize.height - 25});

    auto y = 0;
    auto i = 0;
    m_files = getMacros();
    m_container = CCLayer::create();
    m_mainLayer->addChild(m_container);

    for (const auto& file : m_files)
    {
        if(i > 5) break;
        auto panel = cocos2d::extension::CCScale9Sprite::create("rbot_square.png", {0,0,80,80});
        panel->setContentSize(CCSizeMake(winSize.width / 2, 40));
        panel->setPosition(ccp(winSize.width / 2, winSize.height - 50 - y));

        auto label = CCLabelBMFont::create(file.name.c_str(), "BoonBoldItalic.fnt");
        limitLabelWidth(label, 200);
        label->setPosition(ccp(20, 20));
        label->setAnchorPoint(ccp(0, .5f));
        label->setScale(.9f);
        panel->addChild(label);

        auto spr = ButtonSprite::create(
            "Load", 100, 0, 1, false, "BoonBoldItalic.fnt", "rbot_btn01.png"
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
        m_container->addChild(panel);
        y += 45;
        i++;
    }

    auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    spr->setFlipX(true);
    auto btn_ext = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*)
        {
            this->changePage(m_page + 1);
        }
    );
    btn_ext->setPosition(ccp(winSize.width / 2 - 70, 0));
    auto menu = CCMenu::create(btn_ext, NULL);

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
    m_mainLayer->addChild(menu);

    return true;
}

void MacroLoader::changePage(int page)
{
    int pages = std::ceil(static_cast<double>(m_files.size()) / 6);
    if (m_mainLayer && page > -1 && page < pages)
    {
        m_page = page;
        m_container->removeAllChildrenWithCleanup(true);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto y = 0;

        // std::reverse(m_files.begin(), m_files.end());
        auto start_index = page * 6;
        auto end_index = std::min(start_index + 6, static_cast<int>(m_files.size()));

        for (int i = page * 6 ; i < end_index ; i++)
        {
            if(i > m_files.size()) break;
            auto file = m_files[i];

            auto panel = cocos2d::extension::CCScale9Sprite::create("rbot_square.png", {0,0,80,80});
            panel->setContentSize(CCSizeMake(winSize.width / 2, 40));
            panel->setPosition(ccp(winSize.width / 2, winSize.height - 50 - y));

            auto label = CCLabelBMFont::create(file.name.c_str(), "BoonBoldItalic.fnt");
            limitLabelWidth(label, 200);
            label->setPosition(ccp(20, 20));
            label->setAnchorPoint(ccp(0, .5f));
            label->setScale(.9f);
            panel->addChild(label);

            auto spr = ButtonSprite::create(
                "Load", 100, 0, 1, false, "BoonBoldItalic.fnt", "rbot_btn01.png"
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
            m_container->addChild(panel);
            y += 45;
        }
    }
}

void MacroLoader::FLAlert_Clicked(FLAlertLayer* alert, bool btn2)
{
    if(btn2) this->loadFile(m_files[alert->getTag()].path.c_str());
}