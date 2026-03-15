#pragma once

#include <cocos2d.h>
#include <vector>
#include <string>
#include "CCDropdown.hpp"
#include "CCBlockLayer.h"
#include "EditorConfigurationsLayer.hpp"
using namespace cocos2d;

class SongFilterLayer : public DropdownDelegate, public CCBlockLayer {
public:
DropdownWidget* m_dropdown;
std::vector<std::string> m_optionStrings;
CCLayer* m_referrer = nullptr;
bool m_orderType;
static SongFilterLayer* create(CCLayer*);
virtual ~SongFilterLayer();
void onClose(CCObject*);
void dummy();
virtual void onItemSelected(CCObject* pSender) override;
void dropdownCallback(CCObject* pSender);
void registerWithTouchDispatcher() override;
bool ccTouchBegan(CCTouch*, CCEvent*) override;
void toggle(CCObject*);
CCNode* optionToggler(const char* display, bool* toggleVar, bool addinfo = false, ExtraLayerInfo::InfoType infoType = ExtraLayerInfo::InfoType::PEE);
protected:
virtual bool init(CCLayer*);
void keyBackClicked() override;
};