#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "rbot.hpp"
#include "Toggler.hpp"
#include "robtop/CCTextInputNode.hpp"
#include "robtop/FLAlertLayer.hpp"
#include "RBotLayer.hpp"
#include "RBotAlertLayer.hpp"
using namespace cocos2d;

class ClickLoader : public RBotAlertLayer, public FLAlertLayerProtocol {
public:
    RBotLayer* m_parent;
    CCTextInputNode* m_textInput;
    CCTextInputNode* m_textInput2;
    std::vector<File>(m_files);
    std::vector<CCNode*> m_musicSprites;
    std::vector<CCNode*> m_useSprites;
    std::vector<CCMenuItemSpriteExtra*> m_useBtns;
    bool m_isRelease;
    int m_page;

    static ClickLoader* create(RBotLayer*, bool);
    bool init(bool);
    void keyBackClicked();
    void loadFile(char const *);
    void FLAlert_Clicked(FLAlertLayer*, bool);
    void changePage(int);
    std::vector<Frame> readJson(char const*);
    std::vector<File> getFiles();
};