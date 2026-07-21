#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "rbot/RBot.h"
#include "rbot/Toggler.h"
#include "robtop/CCTextInputNode.hpp"
#include "CCBlockLayer.h"
using namespace cocos2d;

class MacroSaver : public CCLayer, public CCTextFieldDelegate {
public:
    RBotLayer* m_parent;
    CCTextInputNode* m_textInput;
    CCTextInputNode* m_textInput2;
    static MacroSaver* create(RBotLayer*);
    virtual bool init();
    virtual void keyBackClicked() override;
    void toggle(CCObject*);
    void saveFile();
    virtual ~MacroSaver() {}
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* sender, const char* text, int nLen) override { return true; }
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* sender) override { return true; }
};