#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "rbot/RBot.h"
#include "rbot/Toggler.h"
#include "robtop/CCTextInputNode.hpp"
#include "robtop/FLAlertLayer.hpp"
using namespace cocos2d;

struct MacroFile
{
    std::string path;
    std::string name;
    long long size;
    bool operator==(const MacroFile& other) const
    {
        return name == other.name;
    }
};

class MacroLoader : public CCLayer, public FLAlertLayerProtocol {
public:
    RBotLayer* m_parent;
    CCTextInputNode* m_textInput;
    CCTextInputNode* m_textInput2;
    std::vector<MacroFile>(m_files);
    CCLayer* m_mainLayer;
    int m_page;
    static MacroLoader* create(RBotLayer*);
    virtual bool init();
    virtual void keyBackClicked() override;
    void toggle(CCObject*);
    void loadFile(char const *);
    void FLAlert_Clicked(FLAlertLayer*, bool) final;
    void changePage(int);
    std::vector<MacroFile> getMacros();
};