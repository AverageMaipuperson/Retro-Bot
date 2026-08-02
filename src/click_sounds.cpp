#include <cocos2dcpp.h>
#include "ClickLoader.hpp"
#include "RBot.h"
#include <vector>
#include <string>
#include <jni.h>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include "tools.hpp"
#include "fmt/format.h"

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

ClickLoader* ClickLoader::create(RBotLayer* parent) {
    auto ret = new ClickLoader();
    ret->m_parent = parent;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

std::vector<File> ClickLoader::getFiles()
{
    std::vector<File> vector;

    create_directories("/sdcard/rbot/sounds/");

    auto* b_dir = opendir("/sdcard/rbot/sounds/");
    if(b_dir == nullptr) return vector;

    struct dirent* entry;
    std::string dir = "/sdcard/rbot/sounds/";
    while ((entry = readdir(b_dir)) != nullptr)
    {
        std::string name = entry->d_name;

        if (name.size() >= 4 && (name.compare(name.size() - 4, 4, ".mp3") == 0 || name.compare(name.size() - 4, 4, ".ogg") == 0))
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

bool ClickLoader::init() 
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
    auto btn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(ClickLoader::keyBackClicked));
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

    m_files = getFiles();
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

        auto container = CCNode::create();
        container->setContentSize(CCSizeMake(40, 40));

        auto spr = CCSprite::create("GJ_playMusicBtn_001.png");
        auto spr2 = CCSprite::create("GJ_stopMusicBtn_001.png");

        spr->setPosition(ccp(20, 20));
        spr2->setPosition(ccp(20, 20));
        spr2->setVisible(false);

        container->addChild(spr);
        container->addChild(spr2);

        auto btn = CCMenuItemExt::createWithSpriteExtra(
            container,
            [this, file, spr, spr2](CCObject*)
            {
                auto audio = CocosDenshion::SimpleAudioEngine::sharedEngine();
                
                if (spr2->isVisible()) 
                {
                    audio->stopBackgroundMusic();
                    spr->setVisible(true);
                    spr2->setVisible(false);
                    return;
                }

                for (const auto& container : m_musicSprites)
                {
                    if (container->getChildrenCount() >= 2)
                    {
                        auto s1 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(0));
                        auto s2 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(1));
                        
                        s1->setVisible(true);
                        s2->setVisible(false);
                    }
                }

                audio->playBackgroundMusic(file.path.c_str());
                spr->setVisible(false);
                spr2->setVisible(true);
            }
        );
        m_musicSprites.push_back(container);


        auto menu = CCMenu::create();
        menu->addChild(btn);

        // use buttons below

        container = CCNode::create();
        container->setContentSize(CCSizeMake(40, 40));

        spr = CCSprite::create("GJ_selectSongBtn_001.png");
        spr2 = CCSprite::create("GJ_selectSongOnBtn_001.png");

        spr->setPosition(ccp(20, 20));
        spr2->setPosition(ccp(20, 20));
        spr2->setVisible(false);

        container->addChild(spr);
        container->addChild(spr2);

        btn = CCMenuItemExt::createWithSpriteExtra(
            container,
            [this, file, spr, spr2](CCObject*)
            {
                RBot::getModules().soundPath = file.path;
                spr->setVisible(false);
                spr2->setVisible(true);
            }
        );

        auto callback = btn->getTarget();
        btn->setTarget(
            [btn, callback, this, file](CCObject* arg)
            {
                for (const auto& container : m_useSprites)
                {
                    if (container->getChildrenCount() >= 2)
                    {
                        auto s1 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(0));
                        auto s2 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(1));
                        
                        s1->setVisible(true);
                        s2->setVisible(false);
                    }
                }

                for (const auto& useBtn : m_useBtns)
                {
                    useBtn->setEnabled(true);
                }
                callback(arg);
                btn->setEnabled(true);
            }
        );
        m_useSprites.push_back(container);
        m_useBtns.push_back(btn);

        if (file.path == RBot::getModules().soundPath) {
            spr->setVisible(false);
            spr2->setVisible(true);
            btn->setEnabled(false);
        }

        menu->addChild(btn);
        menu->alignItemsHorizontallyWithPadding(5);
        menu->setPosition(ccp(panel->getContentSize().width - 50, 20));
        
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

    spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    spr->setScale(1.25f);

    auto infoBtn = CCMenuItemExt::createWithSpriteExtra(
        spr,
        [this](CCObject*){
            FLAlertLayer::create(
                nullptr,
                "Info",
                "To include click sounds, add them onto the <cl>/storage/emulated/0/rbot/sounds/</c> folder. \n \n Formats supported are: <cg>.mp3</c> and <cy>.ogg</c>.",
                "OK",
                nullptr,
                300
            )->show();
        }
    );
    infoBtn->setPosition(ccp(winSize.width - 45, 25));
    menu = CCMenu::create(infoBtn, NULL);
    menu->setPosition(CCPointZero);
    contentHolder->addChild(menu);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    setAllMenusEnabled(m_parent, false);
    return true;
}

void ClickLoader::changePage(int page)
{
    int pages = std::ceil(static_cast<double>(m_files.size()) / 6);
    if (m_mainLayer && page > -1 && page < pages)
    {
        m_page = page;
        m_mainLayer->removeAllChildrenWithCleanup(true);
        m_musicSprites.clear();

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

            auto container = CCNode::create();
            container->setContentSize(CCSizeMake(40, 40));

            auto spr = CCSprite::create("GJ_playMusicBtn_001.png");
            auto spr2 = CCSprite::create("GJ_stopMusicBtn_001.png");

            spr->setPosition(ccp(20, 20));
            spr2->setPosition(ccp(20, 20));
            spr2->setVisible(false);

            container->addChild(spr);
            container->addChild(spr2);

            auto btn = CCMenuItemExt::createWithSpriteExtra(
                container,
                [this, file, spr, spr2](CCObject*)
                {
                    auto audio = CocosDenshion::SimpleAudioEngine::sharedEngine();
                    
                    if (spr2->isVisible()) 
                    {
                        audio->stopBackgroundMusic();
                        spr->setVisible(true);
                        spr2->setVisible(false);
                        return;
                    }

                    for (const auto& container : m_musicSprites)
                    {
                        if (container->getChildrenCount() >= 2)
                        {
                            auto s1 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(0));
                            auto s2 = static_cast<CCSprite*>(container->getChildren()->objectAtIndex(1));
                            
                            s1->setVisible(true);
                            s2->setVisible(false);
                        }
                    }

                    audio->playBackgroundMusic(file.path.c_str());
                    spr->setVisible(false);
                    spr2->setVisible(true);
                }
            );
            m_musicSprites.push_back(container);

            auto menu = CCMenu::create(btn, NULL);
            menu->setPosition(ccp(panel->getContentSize().width - 40, 20));
        
            panel->addChild(menu);
            m_mainLayer->addChild(panel);
            y += 45;
        }
    }
}

void ClickLoader::FLAlert_Clicked(FLAlertLayer* alert, bool btn2)
{
    //if(btn2) this->loadFile(m_files[alert->getTag()].path.c_str());
}

void ClickLoader::keyBackClicked() {
    setAllMenusEnabled(m_parent, true);
    this->removeFromParentAndCleanup(true);
}