#pragma once

#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "cocos2dExt.h"
#include <string>
#include <vector>
#include <mutex>
#include "CCTextInputNode.hpp"
#include "SongUploaderLayer.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;

class UploadSongLayer : public CCBlockLayer{
public:
SongUploadData uploadData_;
volatile int responseCode_;
volatile int phpResponse_;
CCSprite* m_loadingCircle;
CCSprite* check_;
CCSprite* ex_;
CCMenu* m_closeBtn;
static UploadSongLayer* create(CCLayer* referrer, SongUploadData data);
void onClose(CCObject*);
bool sendDataToServer();
void onUploadFinished(float);
void showSuccess();
void showFail();
protected:
bool init(CCLayer* referrer, SongUploadData data);
void keyBackClicked() override;
};