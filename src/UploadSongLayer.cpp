#include "../vendor/robtop/UploadSongLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/SongUploaderLayer.hpp"
#include <cocos2d.h>
#include <thread>
#include <dlfcn.h>
#include "../vendor/other/fallback.h"
#include "../vendor/cpp-base64-2.rc.08/base64.h"

static std::string base64_url_encode(const std::string &input) {
    static const char b64_table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-_";

    const unsigned char* data = reinterpret_cast<const unsigned char*>(input.data());
    size_t len = input.size();
    std::string out;
    out.reserve(((len + 2) / 3) * 4);

    size_t i = 0;
    while (i + 2 < len) {
        unsigned int triple = (data[i] << 16) | (data[i + 1] << 8) | data[i + 2];
        out.push_back(b64_table[(triple >> 18) & 0x3F]);
        out.push_back(b64_table[(triple >> 12) & 0x3F]);
        out.push_back(b64_table[(triple >> 6) & 0x3F]);
        out.push_back(b64_table[triple & 0x3F]);
        i += 3;
    }

    if (i < len) {
        if (len - i == 1) {
            unsigned int val = data[i] << 16;
            out.push_back(b64_table[(val >> 18) & 0x3F]);
            out.push_back(b64_table[(val >> 12) & 0x3F]);
        } else {
            unsigned int val = (data[i] << 16) | (data[i + 1] << 8);
            out.push_back(b64_table[(val >> 18) & 0x3F]);
            out.push_back(b64_table[(val >> 12) & 0x3F]);
            out.push_back(b64_table[(val >> 6) & 0x3F]);
        }
    }
    while (out.size() % 4 != 0) out.push_back('=');

    return out;
}

 UploadSongLayer* UploadSongLayer::create(CCLayer* referrer, SongUploadData data) {
    auto ret = new UploadSongLayer();
    if (ret->init(referrer, data)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool UploadSongLayer::sendDataToServer() {
    this->retain(); 
    
    auto data = this->uploadData_;
    std::thread([=]() {
        void* handle = dlopen("libcurl.so", RTLD_LAZY);
        if (!handle) handle = dlopen("libgame.so", RTLD_LAZY);
        if (!handle) {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                schedule_selector(UploadSongLayer::onUploadFinished), this, 0, 0, 0, false);
            return;
        }

        auto curl_init = (void*(*)())dlsym(handle, "curl_easy_init");
        auto curl_setopt = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_setopt");
        auto curl_perform = (int(*)(void*))dlsym(handle, "curl_easy_perform");
        auto curl_cleanup = (void(*)(void*))dlsym(handle, "curl_easy_cleanup");
        auto curl_getinfo = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_getinfo");

        void* curl = curl_init();
        if (curl) {
            std::string responseString;
            /* char* encodedRaw = curl_escape(curl, uploadData_.rawData.data(), (int)uploadData_.rawData.size());
    std::string encodedSongData = encodedRaw ? encodedRaw : "";
    if (encodedRaw) curl_free_sym ? curl_free_sym(encodedRaw) : free(encodedRaw); */
    std::string postData = "songName=" + uploadData_.songName + 
                           "&songUploader=" + uploadData_.author +
                           "&songData=" + base64_url_encode(uploadData_.rawData) +
                           "&length=" + patch::to_string(uploadData_.duration) + 
                           "&byteSize=" + patch::to_string(uploadData_.size);
            std::string phpUrl = "pok.ps.fhgdps.com/uploadGJSong16.php";

            curl_setopt(curl, 10002L, phpUrl.c_str());    // CURLOPT_URL
            curl_setopt(curl, 10165L, postData.c_str());  // CURLOPT_COPYPOSTFIELDS
            curl_setopt(curl, 43L, (long)postData.length());
            
            curl_setopt(curl, 20011L, WriteCallback);     // CURLOPT_WRITEFUNCTION
            curl_setopt(curl, 10001L, &responseString);   // CURLOPT_WRITEDATA

            int res = curl_perform(curl);
            long http_code = 0;
            curl_getinfo(curl, 2097154L, &http_code);     // CURLINFO_RESPONSE_CODE

            if (res == 0) {
                CCLog("PHP Response: %s", responseString.c_str());
            }
            // CCLog("POST Params: %s", postData.c_str());

            this->responseCode_ = res;
            this->phpResponse_ = atoi(responseString.c_str());
curl_cleanup(curl);
CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
    schedule_selector(UploadSongLayer::onUploadFinished), 
    this, 0, 0, 0, false
);
        }
        dlclose(handle);
    }).detach();

    return true;
}

void UploadSongLayer::showSuccess() {
check_->setVisible(true);
}

void UploadSongLayer::showFail() {
ex_->setVisible(true);
}

void UploadSongLayer::onUploadFinished(float dt) {
     CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
        schedule_selector(UploadSongLayer::onUploadFinished), this);
    m_loadingCircle->setVisible(false);
    m_closeBtn->setVisible(true);
    m_closeBtn->setEnabled(true);

    if (responseCode_ == 0 && phpResponse_ == 1) {
        showSuccess();
    } else {
        showFail();
    }
    this->release();
}


bool UploadSongLayer::init(CCLayer* referrer, SongUploadData data) {
    if (!CCBlockLayer::init()) {
        return false;
    }

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
    panel->setOpacity(255);
    panel->setContentSize(CCSizeMake(win_size.width - 25, win_size.height - 25));
    leftParent->addChild(panel);
    panel->setPosition({0.f, 0.f});

    CCLayer* mainLayoutLayer = CCLayer::create();
mainLayoutLayer->setAnchorPoint(ccp(0.5f, 0.5f));

this->setTouchEnabled(true);

CCSprite* loadingCircle = CCSprite::create("loadingCircle.png");
    mainLayoutLayer->addChild(loadingCircle);
    loadingCircle->setOpacity(127);
    loadingCircle->setColor(ccc3(255, 255, 255));
    CCRotateBy* rotateAction = CCRotateBy::create(1.0f, 360.0f);
    CCRepeatForever* spinForever = CCRepeatForever::create(rotateAction);
    loadingCircle->runAction(spinForever);
    loadingCircle->setPosition({win_size.width / 2, win_size.height / 2});
    m_loadingCircle = loadingCircle;

CCSprite* check = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
check->setVisible(false);
check->setScale(2.f);
check->setPosition({win_size.width / 2, win_size.height / 2});
check_ = check;
mainLayoutLayer->addChild(check, 1000);

CCSprite* ex = CCSprite::createWithSpriteFrameName("exMark_001.png");
ex->setVisible(false);
ex->setScale(2.f);
ex->setPosition({win_size.width / 2, win_size.height / 2});
ex_ = ex;
mainLayoutLayer->addChild(ex, 1000);

auto sprite = ButtonSprite::create(
    "Close", 200.f, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto menu = CCMenu::create();
  auto btn = CCMenuItemSpriteExtra::create(
    sprite,
    sprite,
    this,
    menu_selector(UploadSongLayer::keyBackClicked)
  );

  menu->addChild(btn);
  menu->setPosition(CCPointZero);
  menu->setPosition(win_size.width / 2, 40);
  menu->setEnabled(false);
  menu->setVisible(false);
  this->addChild(menu, 1000);

  m_closeBtn = menu;

auto titleLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%s", data.songName.c_str())->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel->setScale(1.0f);
  this->addChild(titleLabel);
  this->addChild(mainLayoutLayer);
  this->setKeypadEnabled(true);
  this->uploadData_ = data;
  this->sendDataToServer();

    return true;
}

 void UploadSongLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
  }

  void UploadSongLayer::keyBackClicked() {
    onClose(nullptr);
  }