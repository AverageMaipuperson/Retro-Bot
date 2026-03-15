#include "../vendor/robtop/CustomLabelOptionsLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/CCTextInputNode.hpp"
#include <cocos2d.h>
#include <cstdlib> 
#include <cstdio>

CCTextInputNode* ExtraLayer::m_customLabelInput = nullptr;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))


CCSprite* getToggleSpriteeeeeeeeeeeeeeeeeeeeeeeee(CCSprite* on, CCSprite* off, bool state) { return (state) ? on : off; }
CCMenuItemSprite* getMenuToggleSpriteeee(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; }

struct LegacyString {
    struct Metadata {
        int capacity;
        int length;
        int refCount;
    };
    char* data;
};

// helper to create a string in the format the game expects
void* to_fake_str4(const char* text) {
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

 CustomLabelOptionsLayer* CustomLabelOptionsLayer::create(CCLayer* referrer) {
    auto ret = new CustomLabelOptionsLayer();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomLabelOptionsLayer::init(CCLayer* referrer) {
    if (!CCBlockLayer::init()) {
        return false;
    }

    cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();

    CCNode* leftParent = CCNode::create();
    auto win_size = CCDirector::sharedDirector()->getWinSize();
     CCLayerColor *overlay = CCLayerColor::layerWithColor(ccc4(0, 0, 0, 127),
                                                win_size.width,
                                                 win_size.height);
    overlay->setPosition(0,0);
    this->addChild(overlay);
    this->addChild(leftParent);
    leftParent->setPosition(win_size.width / 2, win_size.height / 2);

    CCRect rect = CCRectMake(0, 0, 80, 80);
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("square02_001.png", rect);
    panel->setOpacity(127);
    panel->setContentSize(CCSizeMake(win_size.width / 2, win_size.height - 25));
    leftParent->addChild(panel);
    panel->setPosition({0.f, 0.f});

    CCLayer* mainLayoutLayer = CCLayer::create();
mainLayoutLayer->setAnchorPoint(ccp(0.5f, 0.5f));

this->setTouchEnabled(true);

auto titleLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Edit Custom Label")->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel->setScale(1.0f);
  this->addChild(titleLabel);

    auto menu1 = CCMenu::create();
  auto noclipBtnSprite = ButtonSprite::create(
    "OK", 50, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto noclipButton = CCMenuItemSpriteExtra::create(
    noclipBtnSprite,
    noclipBtnSprite,
    this,
    menu_selector(CustomLabelOptionsLayer::onClose)
  );

  noclipButton->setScale(0.8f);
  menu1->addChild(noclipButton);

  auto bg = extension::CCScale9Sprite::create("square02_001.png", CCRectMake(0,0,80,80));
    bg->setContentSize(CCSizeMake(240, 60));
    bg->_setZOrder(-1);
    bg->setPosition({win_size.width / 2, win_size.height - 100});
    bg->setOpacity(127);
    this->addChild(bg);
    bg->setScale(0.9);

  CCTextInputNode* textInput = CCTextInputNode::create(100.0, 40.0, "Custom!", "Thonburi", 12, "bigFont.fnt");
    ExtraLayer::m_customLabelInput = textInput;
    if(ExtraLayer::m_customLabelInput) ExtraLayer::m_customLabelInput->setString(ExtraLayer::m_customLabelValue.c_str());
    MEMBER_BY_OFFSET(cocos2d::CCTextFieldDelegate*, textInput, 0x16c) = this;
    textInput->setPosition(ccp(win_size.width / 2, win_size.height - 100));
    textInput->setMaxLabelScale(0.7);
    textInput->setLabelPlaceholderScale(0.7);
    MEMBER_BY_OFFSET(int, textInput, 0x170) = 20; // char limit
    MEMBER_BY_OFFSET(void*, textInput, 0x150) = to_fake_str4("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*.:?!\"/;+-%$(){}[]^& "); // set allowed chars
    textInput->setAnchorPoint({0, 0.5});

  this->addChild(textInput);

  menu1->setPosition({win_size.width / 2, 20});

  mainLayoutLayer->addChild(menu1);
  this->addChild(mainLayoutLayer);
  this->setKeypadEnabled(true);

    return true;
}

  bool CustomLabelOptionsLayer::onTextFieldReturn(cocos2d::CCTextFieldTTF* pSender, const char * text, int nLen) {
    ExtraLayer::m_customLabelValue = std::string(MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, ExtraLayer::m_customLabelInput, 0x168)->getString());
    return true;
  }

 void CustomLabelOptionsLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    const char* str = MEMBER_BY_OFFSET(cocos2d::CCTextFieldTTF*, ExtraLayer::m_customLabelInput, 0x168)->getString();
    std::string cppString = str;
    ExtraLayer::m_customLabelValue = cppString;
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
  }

  void CustomLabelOptionsLayer::keyBackClicked() {
    onClose(nullptr);
  }