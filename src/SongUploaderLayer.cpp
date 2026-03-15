#include "../vendor/robtop/CustomSongLayer.hpp"
#include "../vendor/robtop/SongUploaderLayer.hpp"
#include "../vendor/robtop/PauseLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
// #include "../vendor/robtop/LevelSettingsObject.hpp"
#include "../vendor/robtop/LevelSettingsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/CCTextInputNode.hpp"
#include "../vendor/robtop/FLAlertLayer.hpp"
#include "../vendor/robtop/GJGameLevel.hpp"
#include "../vendor/robtop/GameManager.hpp"
#include "../vendor/cocos/CocosDenshion/include/SimpleAudioEngine.h"
#include "../vendor/robtop/UploadSongLayer.hpp"
#include <cocos2d.h>
#include <cstdlib> 
#include <cstdio>
#include <map>
#include "cocos2dExt.h"
#include <string>
#include <sstream>
#include <vector>
#include <dlfcn.h>
#include <thread>
#include <sys/stat.h> 
#include <cerrno>
#include <cstring>
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

CCLabelBMFont* SongUploaderLayer::iLabel = nullptr;
CCLayer* SongUploaderLayer::m_sharedLayer = nullptr;
CCTextInputNode* SongUploaderLayer::m_input = nullptr;
CCTextInputNode* SongUploaderLayer::m_nameInput = nullptr;
// bool ExtraLayer::uploadNotAllowed_ = false;

SongUploadData uploadData;

using namespace cocos2d;
using namespace cocos2d::extension;

#define OPT_URL 10002
#define OPT_WRITEFUNCTION 20011
#define OPT_WRITEDATA 10001
#define OPT_SSL_VERIFYPEER 64
#define OPT_POST 47
#define OPT_POSTFIELDS 10015
#define SONGCELL_ID_PREFIX 1383110
#define SONGCELL_CONTAINER_PREFIX 2057415
#define SONG_PROGRESS_TAG 96066355
#define SONGCELL_BUTTON_PREFIX 807705

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

    #include "platform/android/jni/JniHelper.h"

void openMp3Selector() {
    cocos2d::JniMethodInfo methodInfo;
    if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, 
        "org/cocos2dx/lib/Cocos2dxActivity", 
        "openMp3Selector", 
        "()V")) 
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

extern "C" JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxActivity_nativeOnMp3Selected(JNIEnv* env, jobject thiz, jbyteArray data, jlong duration, jlong size) {
        cocos2d::CCLog("VIOLETPS: NATIVEONMP3SELECTED CALL");
    if (!data) return;
    jsize arrayLen = env->GetArrayLength(data);
    if (arrayLen <= 0) return;
    jbyte* bufferPtr = env->GetByteArrayElements(data, NULL);
    if (!bufferPtr) return;
    uploadData.rawData.assign(reinterpret_cast<char*>(bufferPtr), static_cast<size_t>(arrayLen));
    uploadData.size = static_cast<int>(arrayLen);
    uploadData.duration = static_cast<int>(duration / 1000);
    env->ReleaseByteArrayElements(data, bufferPtr, JNI_ABORT);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
            schedule_selector(SongUploaderLayer::updateInfoLabel), 
            SongUploaderLayer::m_sharedLayer, 0, 0, 0, false
        );
    }

void SongUploaderLayer::showUploadErrorLength() {
FLAlertLayer::create(
  nullptr,
  "Error",
  "<cr>Your song is too short!</c>\nMinimum required length: 30 seconds",
  "OK",
  nullptr,
  300.f
)->show();
}

void SongUploaderLayer::showUploadErrorData() {
FLAlertLayer::create(
  nullptr,
  "Error",
  "<cr>You need to add a song before uploading!</c>\n(This error could happen when your .mp3 file is empty)",
  "OK",
  nullptr,
  300.f
)->show();
}

void SongUploaderLayer::showUploadErrorEmpty() {
FLAlertLayer::create(
  nullptr,
  "Error",
  "<cr>Song name or username parameters are empty.</c>",
  "OK",
  nullptr,
  300.f
)->show();
}

void SongUploaderLayer::showUploadErrorAllow() {
FLAlertLayer::create(
  nullptr,
  "Error",
  "<cr>You are currently not allowed to upload songs</c> This could be caused by using an outdated version.",
  "OK",
  nullptr,
  300.f
)->show();
}

void SongUploaderLayer::updateInfoLabel() {
  SongUploaderLayer::iLabel->setString(
            CCString::createWithFormat("Length: %ds\nSize: %dB", uploadData.duration, uploadData.size)->getCString()
        );
}

void SongUploaderLayer::submit() {
  if(uploadData.rawData.empty() || !uploadData.size) {
    this->showUploadErrorData();
    return;
  }
  uploadData.songName = std::string(MEMBER_BY_OFFSET(CCTextFieldTTF*, SongUploaderLayer::m_input, 0x168)->getString());
  uploadData.author = std::string(MEMBER_BY_OFFSET(CCTextFieldTTF*, SongUploaderLayer::m_nameInput, 0x168)->getString());
  if(uploadData.songName.empty() || uploadData.author.empty()) {
    this->showUploadErrorEmpty();
    return;
  }
  if(ExtraLayer::uploadNotAllowed_) {
    this->showUploadErrorAllow();
    return;
  }
  auto upload = UploadSongLayer::create(this, uploadData);
  this->addChild(upload, 999999);
}

    // helper to create a string in the format the game expects
void* to_fake_str3(const char* text) {
    size_t len = strlen(text);
    auto* totalBuffer = (uint8_t*)malloc(12 + len + 1);

    int* meta = (int*)totalBuffer;
    meta[0] = len;
    meta[1] = len;
    meta[2] = -1;
    
    char* dataPtr = (char*)(totalBuffer + 12);
    strcpy(dataPtr, text);
    
    return dataPtr; // the game expects a pointer to the TEXT, not the header
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

SongUploaderLayer* SongUploaderLayer::create(CCLayer* referrer) {
    auto ret = new SongUploaderLayer();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void SongUploaderLayer::onBrowse(CCObject*) {
  openMp3Selector();
}

static void setMenusEnabledRecursively(CCNode* node, bool enabled) {
    if (!node) return;
    CCObject* pObj = nullptr;
    CCArray* children = node->getChildren();
    if (children) {
        CCARRAY_FOREACH(children, pObj) {
            CCNode* child = dynamic_cast<CCNode*>(pObj);
            if (!child) continue;
            CCMenu* menu = dynamic_cast<CCMenu*>(child);
            if (menu) menu->setEnabled(enabled);
            setMenusEnabledRecursively(child, enabled);
        }
    }
}

bool SongUploaderLayer::init(CCLayer* referrer) {
  if(!CCBlockLayer::init()) return false;
  this->m_referrer = referrer;
  if (m_referrer) {
      setMenusEnabledRecursively(m_referrer, false);
      m_referrer->setKeypadEnabled(false);
  }

  SongUploaderLayer::m_sharedLayer = this;
  auto win_size = CCDirector::sharedDirector()->getWinSize();

    CCNode* pivot = CCNode::create();
    pivot->setPosition(ccp(win_size.width/2, win_size.height/2));
    this->addChild(pivot);
    CCNode* contentHolder = CCNode::create();
    contentHolder->setPosition(ccp(-win_size.width/2, -win_size.height/2));
    pivot->addChild(contentHolder);
    pivot->setScale(0.1f);
    CCScaleTo* scaleUp = CCScaleTo::create(0.5f, 1.0f);
    CCEaseElasticOut* ease = CCEaseElasticOut::create(scaleUp, 0.5f);
    pivot->runAction(ease);

    CCNode* leftParent = CCNode::create();
     CCLayerColor *overlay = CCLayerColor::layerWithColor(ccc4(0, 0, 0, 127),
                                                win_size.width,
                                                 win_size.height);
    overlay->setPosition(0,0);
    contentHolder->addChild(overlay);
    contentHolder->addChild(leftParent);
    leftParent->setPosition(win_size.width / 2, win_size.height / 2);


    CCRect rect = CCRectMake(0, 0, 80, 80);
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("GJ_square02.png", rect);
    panel->setContentSize(CCSizeMake(win_size.width - 75, win_size.height - 35));
    leftParent->addChild(panel);
    panel->setPosition({0.f, 0.f});

    auto buttonMenu = CCMenu::create();
    auto button = CCSprite::create("GJ_backBtn_001.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, this, menu_selector(SongUploaderLayer::keyBackClicked));
    menuBtn->setPosition({40,-35});
    buttonMenu->setPosition({0, win_size.height});
    buttonMenu->addChild(menuBtn);
    contentHolder->addChild(buttonMenu);

    auto titleLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Upload Song")->getCString(), 
            "bigFont-hd.fnt"
        );

  titleLabel->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel->setScale(1.0f);
  contentHolder->addChild(titleLabel);

  auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(240, 60));
    bg->_setZOrder(0);
    bg->setPosition({win_size.width / 2, win_size.height - 100});
    bg->setOpacity(127);
    contentHolder->addChild(bg);
    bg->setScale(0.9);

  CCTextInputNode* textInput = CCTextInputNode::create(100.0, 40.0, "Song Name", "Thonburi", 12, "chatFont.fnt");
    textInput->setString("Unnamed");
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldDelegate*, textInput, 0x16c) = this;
    textInput->setPosition(ccp(win_size.width / 2, win_size.height - 100));
    textInput->setMaxLabelScale(0.7);
    textInput->setLabelPlaceholderScale(0.7);
    SongUploaderLayer::m_input = textInput;
    MEMBER_BY_OFFSET(int, textInput, 0x170) = 20; // char limit
    MEMBER_BY_OFFSET(void*, textInput, 0x150) = to_fake_str3("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    // textInput->setAllowedChars("1234567890."); // set allowed chars
    // textInput->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,+-/!?");
    textInput->setAnchorPoint({0, 0.5});
    contentHolder->addChild(textInput, 1);

    bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(240, 60));
    bg->_setZOrder(0);
    bg->setPosition({win_size.width / 2, win_size.height - 175});
    bg->setOpacity(127);
    contentHolder->addChild(bg);
    bg->setScale(0.9);

    textInput = CCTextInputNode::create(100.0, 40.0, "User Name", "Thonburi", 12, "chatFont.fnt");
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldDelegate*, textInput, 0x16c) = this;
    textInput->setPosition(ccp(win_size.width / 2, win_size.height - 175));
    textInput->setMaxLabelScale(0.7);
    textInput->setLabelPlaceholderScale(0.7);
    SongUploaderLayer::m_nameInput = textInput;
    MEMBER_BY_OFFSET(int, textInput, 0x170) = 20; // char limit
    MEMBER_BY_OFFSET(void*, textInput, 0x150) = to_fake_str3("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    // textInput->setAllowedChars("1234567890."); // set allowed chars
    // textInput->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,+-/!?");
    textInput->setAnchorPoint({0, 0.5});
    contentHolder->addChild(textInput, 1);

    buttonMenu = CCMenu::create();
    button = CCSprite::create("GJ_browseBtn_001.png");
    menuBtn = CCMenuItemSpriteExtra::create(button, button, this, menu_selector(SongUploaderLayer::onBrowse));
    menuBtn->setPosition({60, 0});
    buttonMenu->setPosition({0, win_size.height / 2});
    buttonMenu->addChild(menuBtn);
    contentHolder->addChild(buttonMenu);

    auto infoLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Select a file first!")->getCString(), 
            "chatFont.fnt"
        );
        infoLabel->setScale(0.5f);
        infoLabel->setAnchorPoint(CCPointMake(1, 0));
        infoLabel->setColor(ccc3(0,0,0));
        infoLabel->setPosition(win_size.width - 75, 25);

    auto sprite = ButtonSprite::create(
    "Upload", 200.f, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto menu = CCMenu::create();
  auto btn = CCMenuItemSpriteExtra::create(
    sprite,
    sprite,
    this,
    menu_selector(SongUploaderLayer::submit)
  );

  menu->addChild(btn);
  menu->setPosition(CCPointZero);
  menu->setPosition(win_size.width / 2, 40);
  contentHolder->addChild(menu, 99);

        SongUploaderLayer::iLabel = infoLabel;
        contentHolder->addChild(infoLabel);
  this->setTouchEnabled(true);
  this->setKeypadEnabled(true);

  return true;
}


void SongUploaderLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this,
        kCCMenuHandlerPriority + 1,
        true
    );
}

bool SongUploaderLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true;
}

void SongUploaderLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    if (this->m_referrer) {
        setMenusEnabledRecursively(this->m_referrer, true);
        this->m_referrer->setKeypadEnabled(true);
        this->m_referrer = nullptr;
    }
    this->removeFromParentAndCleanup(true);
}

void SongUploaderLayer::keyBackClicked() {
    onClose(nullptr);
}