#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCMenuItemToggler.hpp"
using namespace cocos2d;

class Toggler : public CCNode {
public:
CCMenuItemToggler* m_button;
    CCLabelBMFont* m_label;
    bool* m_opt;
    int m_tag;
    static Toggler* create(const char* text, bool* var, CCObject* target, SEL_MenuHandler selector, int tag) {
        Toggler* pRet = new Toggler();
        if (pRet && pRet->init(text, var, target, selector, tag)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
void modifyText(const char* text) {
        m_label->setString(text);
    }
void toggle(bool state) {
    *m_opt = state;
  m_button->toggle(state);  
}
void setSelected(bool on) {
    if (m_button) {
        m_button->toggle(on);
    }
    if (m_opt) {
        *m_opt = on;
    }
}

private:
    bool init(const char* text, bool* var, CCObject* target, SEL_MenuHandler selector, int tag);
};