#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "rbot/RBot.h"
#include "rbot/Toggler.h"
#include "robtop/CCTextInputNode.hpp"
#include "robtop/FLAlertLayer.hpp"
using namespace cocos2d;

class ClickLoader : public CCLayer, public FLAlertLayerProtocol {
public:
    RBotLayer* m_parent;
    CCTextInputNode* m_textInput;
    CCTextInputNode* m_textInput2;
    std::vector<File>(m_files);
    CCLayer* m_mainLayer;
    std::vector<CCNode*> m_musicSprites;
    std::vector<CCNode*> m_useSprites;
    std::vector<CCMenuItemExt*> m_useBtns;
    int m_page;
    static ClickLoader* create(RBotLayer*);
    virtual bool init();
    virtual void keyBackClicked() override;
    void loadFile(char const *);
    void FLAlert_Clicked(FLAlertLayer*, bool) final;
    void changePage(int);
    std::vector<Frame> readJson(char const*);
    std::vector<File> getFiles();
};