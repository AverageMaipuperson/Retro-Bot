#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "rbot.hpp"
#include "RBotLayer.hpp"
#include "Toggler.hpp"
#include "robtop/CCTextInputNode.hpp"
#include "robtop/FLAlertLayer.hpp"
#include "RBotAlertLayer.hpp"
using namespace cocos2d;

class MacroLoader : public RBotAlertLayer, public FLAlertLayerProtocol {
public:
    RBotLayer* m_parent;
    CCTextInputNode* m_textInput;
    CCTextInputNode* m_textInput2;
    std::vector<File>(m_files);
    CCLayer* m_container;
    int m_page;

    static MacroLoader* create(RBotLayer*);

    bool init();
    void toggle(CCObject*);
    void loadFile(char const *);
    void FLAlert_Clicked(FLAlertLayer*, bool);
    void changePage(int);
    std::vector<Frame> readJson(char const*);
    std::vector<File> getMacros();
};