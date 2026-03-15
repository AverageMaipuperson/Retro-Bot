#include "../vendor/robtop/CustomSongLayer.hpp"
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
#include "../vendor/robtop/SongUploaderLayer.hpp"
#include "../vendor/cocos/CocosDenshion/include/SimpleAudioEngine.h"
#include "../vendor/robtop/SongFilterLayer.hpp"
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
#include <atomic>
#include "../vendor/other/fallback.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class VerticalScissorNode : public cocos2d::CCNode {
public:
    float m_minY_points;
    float m_maxY_points;

    static VerticalScissorNode* create(float minY_points, float maxY_points) {
        auto pRet = new VerticalScissorNode();
        pRet->m_minY_points = minY_points;
        pRet->m_maxY_points = maxY_points;
        pRet->autorelease();
        return pRet;
    }

    virtual void visit() override {
    if (!this->isVisible()) return;

    CCEGLView* view = &CCEGLView::sharedOpenGLView();
    if (!view) {
        CCNode::visit();
        return;
    }


    CCSize frameSize = view->getFrameSize();
    CCSize winSize   = CCDirector::sharedDirector()->getWinSize();

    float sx = frameSize.width  / winSize.width;
    float sy = frameSize.height / winSize.height;

    CCPoint worldPos = this->getParent()->convertToWorldSpace(this->getPosition());

    float bottom_in_points = worldPos.y + m_minY_points;
    float top_in_points    = worldPos.y + m_maxY_points;
    float height_in_points = top_in_points - bottom_in_points;
    if (height_in_points <= 0.0f) {
        CCNode::visit();
        return;
    }

    GLint  scissor_x = static_cast<GLint>(floorf(worldPos.x * sx));
    GLint  scissor_y = static_cast<GLint>(floorf(bottom_in_points * sy));
    GLsizei scissor_w = static_cast<GLsizei>(ceilf(frameSize.width)); 
    GLsizei scissor_h = static_cast<GLsizei>(ceilf(height_in_points * sy));

  
    GLint prev_scissor[4] = {0,0,0,0};
    glGetIntegerv(GL_SCISSOR_BOX, prev_scissor);
    GLboolean scissor_was_enabled = glIsEnabled(GL_SCISSOR_TEST);

    glEnable(GL_SCISSOR_TEST);
    glScissor(scissor_x, scissor_y, scissor_w, scissor_h);
    CCNode::visit();
    glScissor(prev_scissor[0], prev_scissor[1], prev_scissor[2], prev_scissor[3]);
    if (!scissor_was_enabled) {
        glDisable(GL_SCISSOR_TEST);
    }
}
};


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

CCSprite* CustomSongLayer::m_loadingCircle = nullptr;
CCNode* CustomSongLayer::m_contentHolder = nullptr;
std::vector<std::string> CustomSongLayer::songList;
std::vector<std::string> CustomSongLayer::songIDs;
std::string CustomSongLayer::s_currentSongName = "";
std::string CustomSongLayer::s_currentSongID = "";
volatile bool CustomSongLayer::s_hasVersionData = false;
volatile bool CustomSongLayer::s_failed = false;
volatile int CustomSongLayer::i_dispID = 0;
volatile int CustomSongLayer::s_errorType = 0;
std::string CustomSongLayer::c_fullPath = "";
std::string CustomSongLayer::c_fullLink = "";
volatile int CustomSongLayer::i_res = 0;
volatile int CustomSongLayer::i_fperrno = 0;
volatile int CustomSongLayer::s_httpError = 0;
float CustomSongLayer::s_currentProgress = 0.f;
int CustomSongLayer::i_currentUsedID = 0;
std::vector<SongInfo> CustomSongLayer::songData;
std::mutex CustomSongLayer::songDataMutex;
CCNode* CustomSongLayer::m_container = nullptr;
CCMenu* CustomSongLayer::s_currentActivePlayBtn = nullptr;
CCMenu* CustomSongLayer::s_currentActiveStopBtn = nullptr;
CCMenu* CustomSongLayer::s_currentActiveUseBtn = nullptr;
CCMenu* CustomSongLayer::s_currentActiveUseOnBtn = nullptr;
std::map<int, float> CustomSongLayer::m_progresses;
std::mutex CustomSongLayer::m_progressMutex;
LevelSettingsObject* CustomSongLayer::m_lso = nullptr;
CCMenu* CustomSongLayer::m_refreshBtn = nullptr;
CCScrollView* CustomSongLayer::m_scroll = nullptr;
std::atomic<int> CustomSongLayer::m_filter{0};
std::atomic<int> CustomSongLayer::m_orderType{0};

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


  LevelSettingsLayer* LevelSettingsLayer::create() {
    auto ret = new LevelSettingsLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CustomSongLayer::~CustomSongLayer() {
    
}

void CustomSongLayer::setAllMenusEnabled(CCNode* parent, bool enabled) {
    if (!parent) return;

    CCObject* pObj = nullptr;
    CCArray* children = parent->getChildren();
    
    if (children) {
        CCARRAY_FOREACH(children, pObj) {
            CCNode* child = dynamic_cast<CCNode*>(pObj);
            if (child) {
                CCMenu* menu = dynamic_cast<CCMenu*>(child);
                if (menu && child != this) {
                    menu->setEnabled(enabled);
                }
                setAllMenusEnabled(child, enabled);
            }
        }
    }
}

void CustomSongLayer::onEnter() {
    CCBlockLayer::onEnter();
}

CustomSongLayer* CustomSongLayer::create(CCLayer* referrer, LevelSettingsObject* object) {
    auto ret = new CustomSongLayer();
    if (ret->init(referrer, object)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void CustomSongLayer::networkTimeout(float dt) {
  CustomSongLayer::s_hasVersionData = true;
}

// Helper function to write binary data to file
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void CustomSongLayer::changeSong(int songID) {
  auto lvl = CustomSongLayer::m_lso;
  MEMBER_BY_OFFSET(int, lvl, 0x130) = songID - 1;
}

int getCurrentSongID() {
  auto lvl = CustomSongLayer::m_lso;
  return MEMBER_BY_OFFSET(int, lvl, 0x130) + 1;
}

void* getPlayerName() {
  auto game = GameManager::sharedState();
  if(game) return MEMBER_BY_OFFSET(void*, game, 0x168);
}

void CustomSongLayer::showDownloadError(int songID) {
  if(CustomSongLayer::m_container) {
    CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
    CCLabelBMFont* label = (CCLabelBMFont*)static_cast<CCNode*>(CustomSongLayer::m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID))->getChildByTag(SONGCELL_ID_PREFIX + songID);
     CCProgressTimer* timer = (CCProgressTimer*)songCell->getChildByTag(SONG_PROGRESS_TAG + songID);
        CCProgressTimer* bg = (CCProgressTimer*)songCell->getChildByTag((SONG_PROGRESS_TAG + songID) * 2);
        auto downloadBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 1);
        if(downloadBtn) {
          downloadBtn->setEnabled(true);
          downloadBtn->setVisible(true);
        }
        if(timer) timer->setVisible(false);
        if(bg) bg->setVisible(false);
        this->unschedule(schedule_selector(CustomSongLayer::updateProgressBar));
    if (label) {
        label->setVisible(true);
        this->error(songID, label, CustomSongLayer::s_errorType);
    }
  }
}

void CustomSongLayer::error(int songID, CCLabelBMFont* label, int errorType) {
  CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    CCSize viewSize = CCSizeMake(win_size.width - 75, win_size.height - 25);
if(!label) return;
  switch(errorType) {
    case 0:
    label->setString(CCString::createWithFormat("libcurl error, exited with code: %i", i_res)->getCString());
    break;
    case 1:
    label->setString(CCString::createWithFormat("fopen error, exited with errno: %i | trying to open path: %s", CustomSongLayer::i_fperrno, CustomSongLayer::c_fullPath.c_str())->getCString());
    label->setPosition(ccp(viewSize.width / 4, 10));
    break;
    case 2:
    label->setString(CCString::createWithFormat("libcurl error, null pointer dereference")->getCString());
    break;
    case 3:
    switch(CustomSongLayer::s_httpError) {
      case 404:
      label->setString(CCString::createWithFormat("Not found. | trying to open link %s", CustomSongLayer::c_fullLink.c_str())->getCString());
      break;
      default:
      label->setString(CCString::createWithFormat("libcurl error %i | trying to open link %s", CustomSongLayer::s_httpError, CustomSongLayer::c_fullLink.c_str())->getCString());
    }
  }
}

void CustomSongLayer::updateProgressBar(float dt) {
    int songID = CustomSongLayer::i_dispID;
    std::lock_guard<std::mutex> lock(m_progressMutex);
    float progress = CustomSongLayer::m_progresses[songID];
    CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
    if (songCell) {
        CCProgressTimer* timer = (CCProgressTimer*)songCell->getChildByTag(SONG_PROGRESS_TAG + songID);
        CCProgressTimer* bg = (CCProgressTimer*)songCell->getChildByTag((SONG_PROGRESS_TAG + songID) * 2);
        if (timer) {
            timer->setPercentage(progress);
        }
        if (progress >= 100.0f) {
        this->onDownloadFinished();
        if(timer) timer->setVisible(false);
        if(bg) bg->setVisible(false);
        this->unschedule(schedule_selector(CustomSongLayer::updateProgressBar));
    }
    }
}


static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    if (dltotal <= 0) return 0;
    double progress = (dlnow / dltotal) * 100.0;
    std::lock_guard<std::mutex> lock(CustomSongLayer::m_progressMutex);
    int songID = CustomSongLayer::i_dispID;
    CustomSongLayer::m_progresses[songID] = (float)progress;
    return 0;
}

void CustomSongLayer::onDownloadFinished() {
int songID = CustomSongLayer::i_dispID;
CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
    if (songCell) {
        CCLabelBMFont* label = (CCLabelBMFont*)songCell->getChildByTag((SONGCELL_ID_PREFIX + songID) * 2);
        label->setVisible(true);
    }
CCUserDefault* def = CCUserDefault::sharedUserDefault();
def->setBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), true);
auto downloadBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 1);
auto playMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 2);
auto stopMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 3);
auto deleteBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 4);
auto useBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 5);
if(playMusicBtn) {
  playMusicBtn->setEnabled(true);
  playMusicBtn->setVisible(true);
}
if(stopMusicBtn) {
  stopMusicBtn->setEnabled(false);
  stopMusicBtn->setVisible(false);
}

if(downloadBtn) {
  downloadBtn->setEnabled(false);
  downloadBtn->setVisible(false);
}
if(deleteBtn) {
  deleteBtn->setEnabled(true);
  deleteBtn->setVisible(true);
}
if(useBtn) {
  useBtn->setEnabled(true);
  useBtn->setVisible(true);
}
}

void CustomSongLayer::incrementDownloadCount(int songID) {
    std::thread([songID]() {
        void* handle = dlopen("libcurl.so", RTLD_LAZY);
        if (!handle) handle = dlopen("libgame.so", RTLD_LAZY);
        if (!handle) return;

        auto curl_init = (void*(*)())dlsym(handle, "curl_easy_init");
        auto curl_setopt = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_setopt");
        auto curl_perform = (int(*)(void*))dlsym(handle, "curl_easy_perform");
        auto curl_cleanup = (void(*)(void*))dlsym(handle, "curl_easy_cleanup");
        auto curl_getinfo = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_getinfo");

        void* curl = curl_init();
        if (curl) {
            std::string phpUrl = "pok.ps.fhgdps.com/downloadGJSong16.php";
            std::string postData = "songID=" + patch::to_string(songID);

            curl_setopt(curl, 10002L, phpUrl.c_str());   // CURLOPT_URL
            curl_setopt(curl, 47L, 1L);                  // CURLOPT_POST
            curl_setopt(curl, 10165L, postData.c_str()); // CURLOPT_COPYPOSTFIELDS
            curl_setopt(curl, 10018L, "Mozilla/5.0");    // CURLOPT_USERAGENT
            curl_setopt(curl, 13L, 30L);                   // CURLOPT_TIMEOUT
            curl_setopt(curl, 64L, 0L); // CURLOPT_SSL_VERIFYPEER
            curl_setopt(curl, 81L, 0L); // CURLOPT_SSL_VERIFYHOST

            int res = curl_perform(curl);

            long http_code = 0;
            curl_setopt(curl, 20011L, +[](void* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
            return size * nmemb; 
            }); // CURLINFO_RESPONSE_CODE

            CCLog("PHP Counter Result Code: %d", res);
            CCLog("PHP HTTP Status: %ld", http_code);
            CCLog("HTTP request parameters: %s, %s", phpUrl.c_str(), postData.c_str());

            curl_cleanup(curl);
        }
        dlclose(handle);
    }).detach();
}


void CustomSongLayer::downloadFile(int songID) {
    std::thread([=]() {
    CustomSongLayer::i_dispID = songID;
    std::string path = "/storage/emulated/0/Music/saved_songs/";
     std::string dirPath = path;
    if (dirPath.back() == '/') dirPath.pop_back();
    
    mkdir(dirPath.c_str(), 0775);
    char fullPath[512];
    sprintf(fullPath, "%s%i.mp3", path.c_str(), songID);
    void* handle = dlopen("libcurl.so", RTLD_LAZY);
    if (!handle) handle = dlopen("libgame.so", RTLD_LAZY);
    if (!handle) {
      CustomSongLayer::s_failed = true;
      return;
    }

    auto curl_init = (void*(*)())dlsym(handle, "curl_easy_init");
    auto curl_setopt = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_setopt");
    auto curl_perform = (int(*)(void*))dlsym(handle, "curl_easy_perform");
    auto curl_cleanup = (void(*)(void*))dlsym(handle, "curl_easy_cleanup");
    auto curl_getinfo = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_getinfo");


    void* curl = curl_init();
    if (curl) {
        FILE* fp = fopen(fullPath, "wb");
        if (fp) {
            CustomSongLayer::c_fullLink = std::string(CCString::createWithFormat("pok.ps.fhgdps.com/songs/%i.mp3", songID)->getCString());
            curl_setopt(curl, 10002, CCString::createWithFormat("pok.ps.fhgdps.com/songs/%i.mp3", songID)->getCString());
            curl_setopt(curl, 20011, write_data);
            curl_setopt(curl, 10001, fp);
            curl_setopt(curl, 64, 0L);
            curl_setopt(curl, 156, 1L);
            curl_setopt(curl, 10018, "Mozilla/5.0"); 
            curl_setopt(curl, 13, 60L);
            curl_setopt(curl, 78, 20L);
            curl_setopt(curl, 43, 0L);
            curl_setopt(curl, 20056, progress_callback);

            int res = curl_perform(curl);
            long http_code = 0;
            curl_getinfo(curl, 20022, &http_code); 
            fclose(fp);

            if (res == 0) {
    CCLog("Song %d downloaded successfully", songID);
} else {
    CustomSongLayer::s_errorType = 3;
    
    if (http_code == 404) {
        CustomSongLayer::s_httpError = 404; 
    } else {
        CustomSongLayer::i_res = res;
        CustomSongLayer::s_httpError = http_code; 
    }
        CustomSongLayer::s_failed = true;
}
        } else {
          CustomSongLayer::c_fullPath = fullPath;
          CustomSongLayer::s_errorType = 1;
          CustomSongLayer::i_fperrno = errno;
          CustomSongLayer::s_failed = true;
        }
        if (curl_cleanup) curl_cleanup(curl);
    } else {
      CustomSongLayer::s_failed = true;
      CustomSongLayer::s_errorType = 2;
    }
    dlclose(handle);
        }).detach();
}

void CustomSongLayer::onDownload(CCObject* pSender) {
    this->schedule(schedule_selector(CustomSongLayer::errorCheck), 0.1f);
    int songID = static_cast<CCNode*>(pSender)->getTag();
    CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
    if (songCell) {
        CCProgressTimer* timer = (CCProgressTimer*)songCell->getChildByTag(SONG_PROGRESS_TAG + songID);
        CCProgressTimer* bg = (CCProgressTimer*)songCell->getChildByTag((SONG_PROGRESS_TAG + songID) * 2);

        auto downloadBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 1);

        if(downloadBtn) {
          downloadBtn->setEnabled(false);
          downloadBtn->setVisible(false);
        }

        if(timer) timer->setVisible(true);
        if(bg) bg->setVisible(true);
    }
    CustomSongLayer::i_dispID = songID;
    CustomSongLayer::s_currentProgress = 0.0f;
    this->schedule(schedule_selector(CustomSongLayer::updateProgressBar));
    this->downloadFile(songID);
    CCUserDefault* user = CCUserDefault::sharedUserDefault();
    if(!user->getBoolForKey(CCString::createWithFormat("downloadSONG%i", songID)->getCString(), false)) {
    user->setBoolForKey(CCString::createWithFormat("downloadSONG%i", songID)->getCString(), true);
    this->downloadFile(songID);
    }
}

void CustomSongLayer::getSongs() {
    std::thread([this]() {
        void* handle = dlopen("libcurl.so", RTLD_LAZY);
        if (!handle) handle = dlopen("libgame.so", RTLD_LAZY);
        if (!handle) return;

        auto curl_init = (void*(*)())dlsym(handle, "curl_easy_init");
        auto curl_setopt = (int(*)(void*, int, ...))dlsym(handle, "curl_easy_setopt");
        auto curl_perform = (int(*)(void*))dlsym(handle, "curl_easy_perform");
        auto curl_cleanup = (void(*)(void*))dlsym(handle, "curl_easy_cleanup");

        if (!curl_init || !curl_setopt || !curl_perform) {
            dlclose(handle);
            return;
        }

        void* curl = curl_init();
        int filter = CustomSongLayer::m_filter.load();
        int order = CustomSongLayer::m_orderType.load();
        if (curl) {
            std::string responseString;
            if(filter > 0 || order == 1) {
              std::string postData = "filter=" + patch::to_string(filter) + "&orderType=" + patch::to_string(order);
            std::string phpUrl = "pok.ps.fhgdps.com/downloadSongsWithFilter16.php";

            curl_setopt(curl, 10002L, phpUrl.c_str());    // CURLOPT_URL
            curl_setopt(curl, 10165L, postData.c_str());  // CURLOPT_COPYPOSTFIELDS
            curl_setopt(curl, 43L, (long)postData.length());
            
            curl_setopt(curl, 20011L, WriteCallback);     // CURLOPT_WRITEFUNCTION
            curl_setopt(curl, 10001L, &responseString);   // CURLOPT_WRITEDATA

            } else {
            curl_setopt(curl, 10002, "pok.ps.fhgdps.com/downloadGJSongs16.php");
            curl_setopt(curl, 20011, WriteCallback);
            curl_setopt(curl, 10001, &responseString);
            curl_setopt(curl, 47, 1L);
            curl_setopt(curl, 10015, "secret=Wmfd2893gb7");
            curl_setopt(curl, 10018, "");
            curl_setopt(curl, 64, 0L);
            }

            if (curl_perform(curl) == 0) {
                std::vector<SongInfo> localSongs;
                std::stringstream ss(responseString);
                std::string segment;
                std::string songSegment; 
                std::vector<SongInfo> parsedSongs;
                while (std::getline(ss, songSegment, '|')) {
    if (songSegment.empty()) continue;

    std::stringstream ssSong(songSegment);
    std::string attr;
    std::vector<std::string> tokens;

    while (std::getline(ssSong, attr, ':')) {
        tokens.push_back(attr);
    }
    if (tokens.size() >= 12) {
        SongInfo info;
        for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
            if (tokens[i] == "1") info.id = tokens[i+1];
            else if (tokens[i] == "2") info.name = tokens[i+1];
            else if (tokens[i] == "3") info.author = tokens[i+1];
            else if (tokens[i] == "4") info.size = tokens[i+1];
            else if (tokens[i] == "5") info.length = tokens[i+1];
            else if (tokens[i] == "6") info.verified = tokens[i+1];
            else if (tokens[i] == "7") info.downloads = tokens[i+1];
        }
        
        std::lock_guard<std::mutex> lock(CustomSongLayer::songDataMutex);
        parsedSongs.push_back(info);
    }
}
                std::lock_guard<std::mutex> lock(CustomSongLayer::songDataMutex);
                CustomSongLayer::songData = parsedSongs;
                CustomSongLayer::s_hasVersionData = true;
            }
            if (curl_cleanup) curl_cleanup(curl);
        }
        dlclose(handle);
    }).detach();
}

void CustomSongLayer::onUploadPage() {
  auto upload = SongUploaderLayer::create(this);
  this->addChild(upload, 1000);
}

void CustomSongLayer::onFilter() {
  auto filter = SongFilterLayer::create(this);
  this->addChild(filter, 1234);
}

bool CustomSongLayer::init(CCLayer* self, LevelSettingsObject* object) {

  CCBlockLayer::init();

  // this->setTouchEnabled(true);

auto win_size = CCDirector::sharedDirector()->getWinSize();

    CustomSongLayer::m_lso = object;

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

    m_contentHolder = contentHolder;

    CCNode* leftParent = CCNode::create();
     CCLayerColor *overlay = CCLayerColor::layerWithColor(ccc4(0, 0, 0, 127),
                                                win_size.width,
                                                 win_size.height);
    overlay->setPosition(0,0);
    contentHolder->addChild(overlay, -10);
    contentHolder->addChild(leftParent, -2);
    leftParent->setPosition(win_size.width / 2, win_size.height / 2);


    CCRect rect = CCRectMake(0, 0, 80, 80);
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("GJ_square02.png", rect);
    panel->setContentSize(CCSizeMake(win_size.width - 75, win_size.height - 35));
    leftParent->addChild(panel, -2);
    panel->setPosition({0.f, 0.f});

        this->m_bgSize = panel->getContentSize();
    CCPoint panelWorldPos = panel->getParent()->convertToWorldSpace(panel->getPosition());
    CCPoint panelPosInContentHolder = contentHolder->convertToNodeSpace(panelWorldPos);
    this->m_bgOrigin = ccp(
        panelPosInContentHolder.x - this->m_bgSize.width * panel->getAnchorPoint().x,
        panelPosInContentHolder.y - this->m_bgSize.height * panel->getAnchorPoint().y
    );
    auto scissor = VerticalScissorNode::create(0.0f, this->m_bgSize.height - 80);
    scissor->setPosition(ccp(this->m_bgOrigin.x, this->m_bgOrigin.y + 40));
    contentHolder->addChild(scissor);
    this->m_scissorNode = scissor;

    panel = cocos2d::extension::CCScale9Sprite::create("square02_001.png", rect);
    panel->setContentSize(CCSizeMake(win_size.width - 160, win_size.height - 80));
    contentHolder->addChild(panel, -1);
    panel->setPosition({win_size.width / 2 - 20, win_size.height / 2});
    panel->setOpacity(127);
    m_bgPanel = panel;
    panel->setVisible(false);

    CCSprite* loadingCircle = CCSprite::create("loadingCircle.png");
    contentHolder->addChild(loadingCircle);
    loadingCircle->setOpacity(127);
    CCRotateBy* rotateAction = CCRotateBy::create(1.0f, 360.0f);
    CCRepeatForever* spinForever = CCRepeatForever::create(rotateAction);
    loadingCircle->runAction(spinForever);
    loadingCircle->setPosition({win_size.width / 2, win_size.height / 2});
    CustomSongLayer::m_loadingCircle = loadingCircle;

    auto buttonMenu = CCMenu::create();
    auto button = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, this, menu_selector(CustomSongLayer::keyBackClicked));
    menuBtn->setPosition({40,-35});
    buttonMenu->setPosition({0, win_size.height});
    buttonMenu->addChild(menuBtn);
    contentHolder->addChild(buttonMenu);

    auto buttonMenu2 = CCMenu::create();
    auto button2 = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, this, menu_selector(CustomSongLayer::onRefresh));
    buttonMenu2->setPosition({win_size.width - 70, win_size.height / 2});
    buttonMenu2->addChild(menuBtn2);
    button2 = CCSprite::create("GJ_sFilterBtn_001.png");
    menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, this, menu_selector(CustomSongLayer::onFilter));
    buttonMenu2->addChild(menuBtn2);
    contentHolder->addChild(buttonMenu2);
    buttonMenu2->alignItemsVerticallyWithPadding(10.0f);
    CustomSongLayer::m_refreshBtn = buttonMenu2;
    buttonMenu2->setEnabled(false);
    buttonMenu2->setVisible(false);

    auto buttonMenu1 = CCMenu::create();
    auto button1 = CCSprite::create("GJ_plusBtn_001.png");
    CCMenuItemSpriteExtra* menuBtn1 = CCMenuItemSpriteExtra::create(button1, button1, this, menu_selector(CustomSongLayer::onUploadPage));
    menuBtn1->setPosition({-40,35});
    buttonMenu1->setPosition({win_size.width, 0});
    buttonMenu1->addChild(menuBtn1);
    contentHolder->addChild(buttonMenu1);
    this->schedule(schedule_selector(CustomSongLayer::updateLoadCheck), 0.1f);

    this->scheduleOnce(schedule_selector(CustomSongLayer::networkTimeout), 10.0f);
    this->getSongs();
    // pMenu->setHandlerPriority(kCCMenuHandlerPriority + 1); 
  this->setKeypadEnabled(true);
  this->_setZOrder(10000000); 

  m_self = self;
  return true;
  }

  void CustomSongLayer::playAudio(CCObject* pSender) {
    auto btn = static_cast<CCMenuItemToggler*>(pSender);
    int songID = static_cast<int>(reinterpret_cast<intptr_t>(btn->getUserData()));
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("/storage/emulated/0/Music/saved_songs/%i.mp3", songID)->getCString());

CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
auto playMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 2);
auto stopMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 3);
if(playMusicBtn) {
  playMusicBtn->setEnabled(false);
  playMusicBtn->setVisible(false);
}
if(stopMusicBtn) {
  stopMusicBtn->setEnabled(true);
  stopMusicBtn->setVisible(true);
}

if(CustomSongLayer::s_currentActiveStopBtn) {
  CustomSongLayer::s_currentActiveStopBtn->setEnabled(false);
  CustomSongLayer::s_currentActiveStopBtn->setVisible(false);
}
if(CustomSongLayer::s_currentActivePlayBtn) {
  CustomSongLayer::s_currentActivePlayBtn->setEnabled(true);
  CustomSongLayer::s_currentActivePlayBtn->setVisible(true);
}

CustomSongLayer::s_currentActivePlayBtn = playMusicBtn;
CustomSongLayer::s_currentActiveStopBtn = stopMusicBtn;
  }

  void CustomSongLayer::stopAudio(CCObject* pSender) {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

auto btn = static_cast<CCMenuItemToggler*>(pSender);
int songID = static_cast<int>(reinterpret_cast<intptr_t>(btn->getUserData()));
CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
auto playMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 2);
auto stopMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 3);
if(playMusicBtn) {
  playMusicBtn->setEnabled(true);
  playMusicBtn->setVisible(true);
}
if(stopMusicBtn) {
  stopMusicBtn->setEnabled(false);
  stopMusicBtn->setVisible(false);
}

CustomSongLayer::s_currentActivePlayBtn = nullptr;
CustomSongLayer::s_currentActiveStopBtn = nullptr;
  }

  void CustomSongLayer::deleteAudio(CCObject* pSender) {
    auto btn = static_cast<CCMenuItemToggler*>(pSender);
    int songID = static_cast<int>(reinterpret_cast<intptr_t>(btn->getUserData()));
    remove(CCString::createWithFormat("/storage/emulated/0/Music/saved_songs/%i.mp3", songID)->getCString());
    auto def = CCUserDefault::sharedUserDefault();
    def->setBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false);
    def->flush();

    if(CustomSongLayer::i_currentUsedID == songID) CustomSongLayer::i_currentUsedID = 0;
CCNode* songCell = m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
auto downloadBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 1);
auto playMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 2);
auto stopMusicBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 3);
auto deleteBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 4);
auto useBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 5);
auto useOnBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 6);
if(downloadBtn) {
  downloadBtn->setEnabled(true);
  downloadBtn->setVisible(true);
}
if(playMusicBtn) {
  playMusicBtn->setEnabled(false);
  playMusicBtn->setVisible(false);
}
if(stopMusicBtn) {
  stopMusicBtn->setEnabled(false);
  stopMusicBtn->setVisible(false);
}
if(deleteBtn) {
  deleteBtn->setEnabled(false);
  deleteBtn->setVisible(false);
}
if(useBtn) {
  useBtn->setEnabled(false);
  useBtn->setVisible(false);
}
if(useOnBtn) {
  useOnBtn->setEnabled(false);
  useOnBtn->setVisible(false);
}
CustomSongLayer::s_currentProgress = 0.f;
  }

  void CustomSongLayer::use(CCObject* pSender) {
    auto btn = static_cast<CCMenuItemToggler*>(pSender);
    int songID = static_cast<int>(reinterpret_cast<intptr_t>(btn->getUserData()));
    int uSongID = getCurrentSongID();
    this->changeSong(songID);

 if(CustomSongLayer::m_container) {
    CCNode* songCell = CustomSongLayer::m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + songID);
auto useBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 5);
auto useOnBtn = (CCMenu*)songCell->getChildByTag((SONGCELL_BUTTON_PREFIX + songID) * 6);

if(uSongID != 0) {
CCNode* oldSongCell = CustomSongLayer::m_container->getChildByTag(SONGCELL_CONTAINER_PREFIX + uSongID);
if(oldSongCell) {
auto usedBtn = (CCMenu*)oldSongCell->getChildByTag((SONGCELL_BUTTON_PREFIX + uSongID) * 5);
auto usedOnBtn = (CCMenu*)oldSongCell->getChildByTag((SONGCELL_BUTTON_PREFIX + uSongID) * 6);

if(usedOnBtn) {
  usedOnBtn->setVisible(false);
}

if(usedBtn) {
  usedBtn->setEnabled(true);
  usedBtn->setVisible(true);
}
}
}
if(useOnBtn) {
  useOnBtn->setVisible(true);
}

if(useBtn) {
  useBtn->setEnabled(false);
  useBtn->setVisible(false);
}

CustomSongLayer::s_currentActiveUseBtn = useBtn;
CustomSongLayer::s_currentActiveUseOnBtn = useOnBtn;
CustomSongLayer::i_currentUsedID = songID;
  }
  }

  /* void CustomSongLayer::scrollViewDidScroll(CCScrollView* view) {
    float h = view->getContentSize().height - view->getViewSize().height;
    float offset = view->getContentOffset().y;
    float percent = (-offset / h);
    if (m_scrollbarThumb) {
        float scrollRange = desiredHeight - m_scrollbarThumb->getContentSize().height;
        float newY = minY + (percent * scrollRange);
        m_scrollbarThumb->setPositionY(newY);
    }
} */

  void CustomSongLayer::showSongInfo(CCObject* pSender) {
    auto item = dynamic_cast<CCMenuItemSpriteExtra*>(pSender);
    if (!item) return;
    int songID = item->getTag();

    SongInfo* foundSong = nullptr;
    std::lock_guard<std::mutex> lock(CustomSongLayer::songDataMutex);
    for (auto& song : CustomSongLayer::songData) {
        if (atoi(song.id.c_str()) == songID) {
            foundSong = &song;
            break;
        }
    }
    if (!foundSong) return;
    FLAlertLayer::create(
      nullptr,
      "Info",
      CCString::createWithFormat("<cy>%s</c> \n <cg>Author: </c>%s \n <cl>SongID: </c>%i \n <cp>Downloads: </c>%i \n <co>Bytes:</c> %i \n <cr>Length: </c>%is", foundSong->name.c_str(), foundSong->author.c_str(), songID, atoi(foundSong->downloads.c_str()), atoi(foundSong->size.c_str()), atoi(foundSong->length.c_str()))->getCString(),
      "OK",
      nullptr,
      300.f
    )->show();
  }


  void CustomSongLayer::createSongList() {
    auto def = CCUserDefault::sharedUserDefault();
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    int uSongID = getCurrentSongID();
     CCSize viewSize = CCSizeMake(std::max(0.0f, this->m_bgSize.width - 12.f), this->m_bgSize.height);
    float cellHeight = 50.0f;
    int totalSongs = CustomSongLayer::songData.size();
    float padding = 10.0f;
    float effectiveCellHeight = cellHeight + padding; 
    float containerHeight = totalSongs * effectiveCellHeight;
    CCLayer* container = CCLayer::create();
    container->setContentSize(CCSizeMake(viewSize.width, containerHeight + padding + 123456789));
    for (int i = 0; i < totalSongs; i++) {
        auto songCell = CCNode::create();
        songCell->ignoreAnchorPointForPosition(false);
        songCell->setAnchorPoint(ccp(0.5f, 0.5f)); 
        songCell->setContentSize(CCSizeMake(viewSize.width, cellHeight));
        SongInfo& song = CustomSongLayer::songData[i];
        int songID = atoi(song.id.c_str());
        int downloads = atoi(song.downloads.c_str());
        std::string displayString = song.name + " by " + song.author;
        auto songLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%s", displayString.c_str())->getCString(), 
            "bigFont.fnt"
        );
        songCell->addChild(songLabel, 99);
        songLabel->setScale(0.5f);
        songLabel->setAnchorPoint(ccp(0, 1));
        songLabel->setPosition(ccp(55, cellHeight - 5));
        float currentWidth = songLabel->getContentSize().width * songLabel->getScale();
        if (currentWidth > 200.f) songLabel->setScale(200.f / songLabel->getContentSize().width);

        auto errorLabel = CCLabelBMFont::create(
            CCString::createWithFormat("An error has occured, please try again later.")->getCString(), 
            "chatFont.fnt"
        );
        songCell->addChild(errorLabel, 99);
        errorLabel->setScale(0.5f);
        errorLabel->setAnchorPoint(ccp(0, 1));
        errorLabel->setPosition(ccp(viewSize.width / 3, 10));
        errorLabel->setColor(ccc3(255, 0, 0));
        errorLabel->setVisible(false);
        errorLabel->setTag(SONGCELL_ID_PREFIX + atoi(song.id.c_str()));

        auto finishedLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Download finished")->getCString(), 
            "chatFont.fnt"
        );
        songCell->addChild(finishedLabel, 99);
        finishedLabel->setScale(0.5f);
        finishedLabel->setAnchorPoint(ccp(0, 1));
        finishedLabel->setPosition(ccp(viewSize.width / 3, 10));
        finishedLabel->setColor(ccc3(0, 255, 0));
        finishedLabel->setVisible(false);
        finishedLabel->setTag((SONGCELL_ID_PREFIX + atoi(song.id.c_str())) * 2);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", CCRectMake(0,0,80,80));
        bg->setContentSize(CCSizeMake(viewSize.width - 100, 60));
        bg->setPosition(ccp(viewSize.width / 2, cellHeight / 2));
        songCell->addChild(bg, 98);
        float yPos = containerHeight - (i * effectiveCellHeight) - (effectiveCellHeight / 2) - (padding * (i + 1));
        songCell->setPosition(ccp(viewSize.width / 3, yPos));
        auto downloadBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_downloadBtn_001.png"),
        CCSprite::create("GJ_downloadBtn_001.png"),
        this,
        menu_selector(CustomSongLayer::onDownload)
        );
        downloadBtn->setTag(songID);
        downloadBtn->setPosition(ccp(viewSize.width - 100, cellHeight / 2));
        auto cellMenu = CCMenu::create(downloadBtn, NULL);
        cellMenu->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu, 100);
        
        struct stat buffer;
        bool fileExists = (stat(CCString::createWithFormat("/storage/emulated/0/Music/saved_songs/%i.mp3", songID)->getCString(), &buffer) != 0);
        if(fileExists) {
          def->setBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false);
          def->flush();
        }
        
        if(def->getBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false)) {
        cellMenu->setEnabled(false);
        cellMenu->setVisible(false);
        }

        cellMenu->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 1);
        
        
        auto useBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_selectSongBtn_001.png"),
        CCSprite::create("GJ_selectSongBtn_001.png"),
        this,
        menu_selector(CustomSongLayer::use)
        ); 
        useBtn->setUserData((void*)atoi(song.id.c_str()));
        useBtn->setPosition(ccp(viewSize.width - 100, cellHeight / 2));
        auto cellMenu4 = CCMenu::create(useBtn, NULL);
        cellMenu4->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu4, 100);
        cellMenu4->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 5);

        if(!def->getBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false) || songID == uSongID) {
        cellMenu4->setEnabled(false);
        cellMenu4->setVisible(false);
        }

        auto useOnBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_selectSongOnBtn_001.png"),
        CCSprite::create("GJ_selectSongOnBtn_001.png"),
        this,
        menu_selector(ExtraLayer::dummy)
        ); 
        useOnBtn->setPosition(ccp(viewSize.width - 100, cellHeight / 2));
        auto cellMenu5 = CCMenu::create(useOnBtn, NULL);
        cellMenu5->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu5, 100);
        cellMenu5->setEnabled(false);
        cellMenu5->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 6);
        
        if(songID != uSongID || fileExists) cellMenu5->setVisible(false);

        auto playAudioBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_playMusicBtn_001.png"),
        CCSprite::create("GJ_playMusicBtn_001.png"),
        this,
        menu_selector(CustomSongLayer::playAudio)
        );
        playAudioBtn->setUserData((void*)atoi(song.id.c_str()));
        playAudioBtn->setPosition(ccp(viewSize.width - 150, cellHeight / 2));
        auto cellMenu1 = CCMenu::create(playAudioBtn, NULL);
        cellMenu1->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu1, 100);
        if(!def->getBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false)) {
        cellMenu1->setEnabled(false);
        cellMenu1->setVisible(false);
        }
        cellMenu1->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 2);

        auto stopAudioBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_stopMusicBtn_001.png"),
        CCSprite::create("GJ_stopMusicBtn_001.png"),
        this,
        menu_selector(CustomSongLayer::stopAudio)
        );
        stopAudioBtn->setUserData((void*)atoi(song.id.c_str()));
        stopAudioBtn->setPosition(ccp(viewSize.width - 150, cellHeight / 2));
        auto cellMenu2 = CCMenu::create(stopAudioBtn, NULL);
        cellMenu2->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu2, 100);
        cellMenu2->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 3);
        cellMenu2->setEnabled(false);
        cellMenu2->setVisible(false);

        auto deleteAudioBtn = CCMenuItemSpriteExtra::create(
        CCSprite::create("GJ_trashBtn_001.png"),
        CCSprite::create("GJ_trashBtn_001.png"),
        this,
        menu_selector(CustomSongLayer::deleteAudio)
        );
        deleteAudioBtn->setUserData((void*)atoi(song.id.c_str()));
        deleteAudioBtn->setPosition(ccp(viewSize.width - 200, cellHeight / 2));
        auto cellMenu3 = CCMenu::create(deleteAudioBtn, NULL);
        cellMenu3->setPosition(ccp(0, 0));
        songCell->addChild(cellMenu3, 100);
        if(!def->getBoolForKey(CCString::createWithFormat("SONG%i", songID)->getCString(), false)) {
        cellMenu3->setEnabled(false);
        cellMenu3->setVisible(false);
        }
        cellMenu3->setTag((SONGCELL_BUTTON_PREFIX + atoi(song.id.c_str())) * 4);

        CCSprite* barBG = CCSprite::create("slidergroove.png");
        barBG->setPosition(ccp(viewSize.width / 2, cellHeight / 4));
        barBG->setScaleX(1.f);
        songCell->addChild(barBG, 100);

        CCSprite* barFull = CCSprite::create("sliderBar.png");
        // barFull->setAnchorPoint(ccp(0, 0.5f)); 
        // barFull->setContentSize(barBG->getContentSize());
        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        barFull->getTexture()->setTexParameters(&params);
        barFull->setTextureRect(CCRectMake(0, 0, barBG->getContentSize().width, barFull->getTexture()->getContentSize().height));

        /* happy new year */
        CCProgressTimer* progressTimer = CCProgressTimer::create(barFull);
        progressTimer->setType(kCCProgressTimerTypeBar);
        // progressTimer->setColor(ccc3(0, 255, 0));
        progressTimer->setMidpoint(ccp(0, 0.5f)); 
        progressTimer->setBarChangeRate(ccp(1, 0));
        progressTimer->setPercentage(0);
        progressTimer->ignoreAnchorPointForPosition(false);
        progressTimer->setAnchorPoint(barBG->getAnchorPoint()); 
        progressTimer->setPosition(barBG->getPosition());
        // progressTimer->setAnchorPoint(ccp(0, 0.5f)); 
        // progressTimer->setContentSize(barBG->getContentSize());
        // float grooveLeft = barBG->getPositionX() - (barBG->getContentSize().width * barBG->getScaleX() / 2);
        // progressTimer->setPosition(ccp(grooveLeft, barBG->getPositionY()));
        // progressTimer->setScaleX(barBG->getScaleX()); 
        float scaleX = barBG->getContentSize().width / barFull->getContentSize().width;
        progressTimer->setScaleX(scaleX);
        progressTimer->setScaleY(barBG->getScaleY());
        progressTimer->setTag(SONG_PROGRESS_TAG + atoi(song.id.c_str()));
        barBG->setTag((SONG_PROGRESS_TAG + atoi(song.id.c_str())) * 2);
        progressTimer->setVisible(false);
        barBG->setVisible(false);
        songCell->addChild(progressTimer, 99);

        auto downloadSprite = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
        auto downloadLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%i", downloads)->getCString(), 
            "bigFont.fnt"
        );
        
        downloadSprite->setPosition({65,10});
        downloadSprite->setScale(0.75f);
        downloadLabel->setAnchorPoint({0, 0.5});
        downloadLabel->setScale(0.5f);
        downloadLabel->setPosition({downloadSprite->getPosition().x + downloadSprite->getContentSize().width - 10, downloadSprite->getPosition().y});
        songCell->addChild(downloadSprite, 99);
        songCell->addChild(downloadLabel, 99);
        
        auto buttonMenu3 = CCMenu::create();
        buttonMenu3->setPosition(CCPointZero);
auto button3 = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
CCMenuItemSpriteExtra* menuBtn3 = CCMenuItemSpriteExtra::create(button3, button3, this, menu_selector(CustomSongLayer::showSongInfo));
buttonMenu3->addChild(menuBtn3);
menuBtn3->setTag(songID);
songCell->addChild(buttonMenu3, 167);
buttonMenu3->setPosition(ccp(viewSize.width - 50, cellHeight));

        container->addChild(songCell);
        songCell->setTag(SONGCELL_CONTAINER_PREFIX + atoi(song.id.c_str()));

        std::lock_guard<std::mutex> lock(m_progressMutex);
        CustomSongLayer::m_progresses[songID] = 0.f;
    }
        CustomSongLayer::m_container = container;
    VerticalScissorNode* scissor = this->m_scissorNode;
    if (!scissor) {

        scissor = VerticalScissorNode::create(0.0f, this->m_bgSize.height - 40);
        scissor->setPosition({this->m_bgOrigin.x, this->m_bgOrigin.y + 20});
        if (m_contentHolder) m_contentHolder->addChild(scissor);
        this->m_scissorNode = scissor;
    }
    CCSize scrollViewSize = this->m_bgSize;
    CCScrollView* scrollView = CCScrollView::create(scrollViewSize, container);
    scrollView->setDirection(CCScrollViewDirectionVertical);
    scrollView->setPosition({m_bgOrigin.x + 30, 0});
    scrollView->setClippingToBounds(false);
    scissor->addChild(scrollView);
    float initialY = scrollViewSize.height - container->getContentSize().height; 
    CustomSongLayer::m_scroll = scrollView;

    auto songCountText = CCString::createWithFormat("%i total songs", totalSongs)->getCString();
    auto songCountLabel = CCLabelBMFont::create(songCountText, "goldFont.fnt");
    songCountLabel->setAnchorPoint({1, 1});
    songCountLabel->setPosition({win_size.width - 10, win_size.height - 5});
    songCountLabel->setScale(0.5f);
    this->addChild(songCountLabel);

    CustomSongLayer::m_refreshBtn->setVisible(true);
    CustomSongLayer::m_refreshBtn->setEnabled(true);
    m_bgPanel->setVisible(true);
  }

void CustomSongLayer::onRefresh() {
  if(!m_container) return;
    CustomSongLayer::m_refreshBtn->setVisible(false);
    CustomSongLayer::m_refreshBtn->setEnabled(false);
    CustomSongLayer::m_scroll->removeFromParentAndCleanup(true);
    m_bgPanel->setVisible(false);
    this->unscheduleAllSelectors();
    m_loadingCircle->setVisible(true);
    m_container->removeFromParentAndCleanup(true);
    this->schedule(schedule_selector(CustomSongLayer::updateLoadCheck), 0.1f);
    this->scheduleOnce(schedule_selector(CustomSongLayer::networkTimeout), 10.0f);
    this->getSongs();
}

void CustomSongLayer::errorCheck(float dt) {
  if(CustomSongLayer::s_failed && CustomSongLayer::i_dispID != 0) {
    this->showDownloadError(CustomSongLayer::i_dispID);
    CustomSongLayer::s_failed = false; 
  }
  this->unschedule(schedule_selector(CustomSongLayer::errorCheck));
}

  void CustomSongLayer::updateLoadCheck(float dt) {
    if (CustomSongLayer::s_hasVersionData) { 
        this->unschedule(schedule_selector(CustomSongLayer::updateLoadCheck));
        if (m_loadingCircle) {
            m_loadingCircle->setVisible(false);
        }
        this->createSongList();
    }
}

  void CustomSongLayer::onSongClick(CCObject* pSender) {
  CCNode* pNode = (CCNode*)pSender;
  if (!pNode) return;
  int id = pNode->getTag(); 
  }

  void CustomSongLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    this->removeFromParentAndCleanup(true);
CustomSongLayer::m_loadingCircle = nullptr;
CustomSongLayer::m_contentHolder = nullptr;
CustomSongLayer::s_currentSongName = "";
CustomSongLayer::s_currentSongID = "";
CustomSongLayer::s_hasVersionData = false;
CustomSongLayer::s_failed = false;
CustomSongLayer::i_dispID = 0;
CustomSongLayer::s_errorType = 0;
CustomSongLayer::c_fullPath = "";
CustomSongLayer::i_res = 0;
CustomSongLayer::i_fperrno = 0;
CustomSongLayer::s_currentProgress = 0.f;
CustomSongLayer::m_container = nullptr;
CustomSongLayer::s_currentActivePlayBtn = nullptr;
CustomSongLayer::s_currentActiveStopBtn = nullptr;
CustomSongLayer::s_currentActiveUseBtn = nullptr;
CustomSongLayer::s_currentActiveUseOnBtn = nullptr;
CustomSongLayer::m_lso = nullptr;
CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
  }

  void CustomSongLayer::keyBackClicked() {
    onClose(nullptr);
  }