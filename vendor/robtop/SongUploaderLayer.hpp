#pragma once

#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "cocos2dExt.h"
#include <string>
#include <vector>
#include <mutex>
#include "CCTextInputNode.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;

struct SongUploadData {
	std::string songName;
	std::string author;
	std::string rawData;
	int duration;
	int size;
};

class SongUploaderLayer : public CCBlockLayer, public CCTextFieldDelegate {
public:
static const char* songName;
static CCLabelBMFont* iLabel;
static CCLayer* m_sharedLayer;
static CCTextInputNode* m_input;
static CCTextInputNode* m_nameInput;
CCLayer* m_referrer = nullptr;
static SongUploaderLayer* create(CCLayer* referrer);
void onClose(CCObject*);
void submit();
void onBrowse(CCObject*);
void updateInfoLabel();
void showUploadErrorLength();
void showUploadErrorData();
void showUploadErrorEmpty();
void showUploadErrorAllow();
void registerWithTouchDispatcher() override;
bool ccTouchBegan(CCTouch*, CCEvent*) override;
protected:
bool init(CCLayer* referrer);
void keyBackClicked() override;
};