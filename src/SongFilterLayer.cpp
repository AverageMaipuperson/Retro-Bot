#include "../vendor/robtop/CCDropdown.hpp"
#include "../vendor/robtop/SongFilterLayer.hpp"
#include "../vendor/robtop/CustomSongLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/CCBlockLayer.h"
#include <string>
#include <vector>
#include <cocos2d.h>
using namespace cocos2d;

int selectedOption = 0;
bool orderType = false;

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

SongFilterLayer* SongFilterLayer::create(CCLayer* referrer) {
    auto ret = new SongFilterLayer();
    if(ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

SongFilterLayer::~SongFilterLayer() {
}

void SongFilterLayer::dummy() {
}

CCMenuItemSprite* getMenuToggleSprite_idkanymore(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; }

void SongFilterLayer::toggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    bool* toggleVar = static_cast<bool*>(btn->getUserData());
    if(toggleVar) *toggleVar = !*toggleVar;
}

CCNode* SongFilterLayer::optionToggler(const char* display, bool* toggleVar, bool addinfo, ExtraLayerInfo::InfoType infoType) {
  auto toggleOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
  auto toggleOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
  auto itemOff = CCMenuItemSprite::create(toggleOffSprite, toggleOffSprite, nullptr, nullptr);
  auto itemOn  = CCMenuItemSprite::create(toggleOnSprite,  toggleOnSprite,  nullptr, nullptr);

auto btn = CCMenuItemToggler::create(
    getMenuToggleSprite_idkanymore(itemOn, itemOff, *toggleVar), 
    getMenuToggleSprite_idkanymore(itemOff, itemOn, *toggleVar),  
    this, 
    menu_selector(SongFilterLayer::toggle) 
);

btn->setUserData(toggleVar);
btn->setAnchorPoint(ccp(0, 0.5f));
btn->setScale(0.8f);

   auto counterLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%s", display)->getCString(), 
            "bigFont.fnt"
        );
  counterLabel->setScale(0.5f);
  counterLabel->setAnchorPoint(ccp(0, 0.5f));
  btn->addChild(counterLabel);

  if(addinfo) {
    auto infobutton = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(infobutton, infobutton, this, menu_selector(ExtraLayer::showInfo));
    menuBtn->setUserData((void*)infoType);
    auto btnMenu = CCMenu::create(menuBtn, NULL);
    btnMenu->ignoreAnchorPointForPosition(false);
    btnMenu->setContentSize(CCSizeZero);
    btnMenu->setPosition(ccp(0, 0));
    menuBtn->setPosition(ccp(-6.0f, (btn->getContentSize().height * 2) + 6));
    btnMenu->setScale(0.5f);
    btn->addChild(btnMenu, 10);
  }
  auto menu = CCMenu::create(btn, NULL);
  menu->setPosition(CCPointZero);

  float padding = 5.0f;
  counterLabel->setPosition(ccp(btn->getContentSize().width + padding, btn->getContentSize().height / 2));
  float totalWidth = btn->getContentSize().width + padding + (counterLabel->getContentSize().width * counterLabel->getScaleX());
  float totalHeight = MAX(btn->getContentSize().height, counterLabel->getContentSize().height);

  auto container = CCNode::create();
  float totalWidth2 = padding + counterLabel->getContentSize().width;
  container->setContentSize(CCSizeMake(totalWidth2, btn->getContentSize().height));
  container->addChild(menu);

  return container;
}

bool SongFilterLayer::init(CCLayer* referrer) {
    if(!CCBlockLayer::init()) return false;
    this->setTouchEnabled(true); 
    this->m_referrer = referrer;
    if (m_referrer) {
        setMenusEnabledRecursively(m_referrer, false);
        m_referrer->setKeypadEnabled(false);
    }

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
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, this, menu_selector(SongFilterLayer::onClose));
    menuBtn->setPosition({40,-35});
    buttonMenu->setPosition({0, win_size.height});
    buttonMenu->addChild(menuBtn);
    contentHolder->addChild(buttonMenu);

    m_optionStrings = {"Downloads", "Age", "Length", "Size"};
    m_dropdown = DropdownWidget::create(m_optionStrings, this, menu_selector(SongFilterLayer::onItemSelected), selectedOption);
    m_dropdown->setPosition(ccp(win_size.width/2, win_size.height/1.5 + 50));
    contentHolder->addChild(m_dropdown);
    m_dropdown->changeDisplay(m_optionStrings[selectedOption].c_str());

    auto orderBtn = this->optionToggler("Ascending Order", &orderType);
    orderBtn->setPosition(70, 40);
    contentHolder->addChild(orderBtn);

    this->setKeypadEnabled(true);
    return true;
}

void SongFilterLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

bool SongFilterLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; 
}

void SongFilterLayer::onItemSelected(CCObject* pSender) {
    auto item = (CCMenuItem*)pSender;
    int index = item->getTag();
    m_dropdown->updateLabel(m_optionStrings[index].c_str());
    m_dropdown->m_selectedOption = index;
    selectedOption = index;
    CustomSongLayer::m_filter = index;
}
void SongFilterLayer::onClose(CCObject*) {
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    if (this->m_referrer) {
        setMenusEnabledRecursively(this->m_referrer, true);
        this->m_referrer->setKeypadEnabled(true);
        this->m_referrer = nullptr;
    }
    CustomSongLayer::m_orderType = orderType;
    CustomSongLayer::m_filter = selectedOption;
    this->removeFromParentAndCleanup(true);
}

void SongFilterLayer::keyBackClicked() {
    this->onClose(nullptr);
}