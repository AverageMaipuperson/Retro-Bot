#include "../vendor/robtop/NoclipOptionsLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/NoclipTintOptionsLayer.hpp"
#include <cocos2d.h>

CCSprite* getToggleSpritee(CCSprite* on, CCSprite* off, bool state) { return (state) ? on : off; }
CCMenuItemSprite* getMenuToggleSpritee(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; }

class ToggleHack {
    public:
    void toggleFlash() {
      ExtraLayer::m_flash = !ExtraLayer::m_flash;
    }
};

 NoclipOptionsLayer* NoclipOptionsLayer::create(CCLayer* referrer) {
    auto ret = new NoclipOptionsLayer();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void NoclipOptionsLayer::onNoclipTintOptions(CCObject* sender) {
    NoclipTintOptionsLayer::create(this);
}

bool NoclipOptionsLayer::init(CCLayer* referrer) {
    if (!CCBlockLayer::init()) {
        return false;
    }

    cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();
  ExtraLayer::m_flash = def->getBoolForKey("noclipflash", false);

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
            CCString::createWithFormat("No-Clip Options")->getCString(), 
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
    menu_selector(NoclipOptionsLayer::onClose)
  );

  noclipButton->setScale(0.8f);
  menu1->addChild(noclipButton);
  menu1->setPosition({win_size.width / 2, 20});

  auto exportBtnSprite = ButtonSprite::create(
    "Set Tint Color", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto exportBtn = CCMenuItemSpriteExtra::create(
    exportBtnSprite,
    exportBtnSprite,
    this,
    menu_selector(NoclipOptionsLayer::onNoclipTintOptions)
  );

auto exportMenu = CCMenu::create();
exportMenu->addChild(exportBtn);
exportMenu->setAnchorPoint({0, 0.5f});
exportMenu->setPosition(ccp(win_size.width / 4 + 20, win_size.height / 2));

  ExtraLayer* refExtra = dynamic_cast<ExtraLayer*>(referrer);
  auto flashBtn = refExtra->optionToggler("Noclip Tint", &ExtraLayer::m_flash, true, ExtraLayerInfo::InfoType::FLASH);
  flashBtn->setPosition(win_size.width / 4 + 20, win_size.height - 100);

  mainLayoutLayer->addChild(menu1);
  mainLayoutLayer->addChild(flashBtn);
  // mainLayoutLayer->addChild(exportMenu);
  this->addChild(mainLayoutLayer);
  this->setKeypadEnabled(true);

    return true;
}

 void NoclipOptionsLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();
    def->setBoolForKey("noclipflash", ExtraLayer::m_flash);
    def->flush();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
  }

  void NoclipOptionsLayer::keyBackClicked() {
    onClose(nullptr);
  }