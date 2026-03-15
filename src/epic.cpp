#include "../vendor/cocos/cocos2dx/include/cocos2d.h"
#include "../vendor/cocos/CocosDenshion/include/SimpleAudioEngine.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "../vendor/other/hooking.h"
#include "../vendor/cocos/cocos2dx/platform/CCPlatformMacros.h"
#include "../vendor/robtop/GJGameLevel.hpp"
#include "../vendor/robtop/EditButtonBar.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include "../vendor/robtop/FPSCounter.hpp"
#include "../vendor/robtop/OptionsLayer.hpp"
#include "../vendor/robtop/MyLevelsLayer.hpp"
#include "../vendor/robtop/FLAlertLayer.hpp"
#include "../vendor/robtop/GameManager.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/LevelSettingsLayer.hpp"
#include "../vendor/other/levels.h"
#include "../vendor/robtop/ObjectInfoHandler.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/PlaytestLayer.hpp"
#include "../vendor/robtop/AppDelegate.hpp"
#include "../vendor/robtop/VersionRequest.hpp"
#include "../vendor/robtop/HitboxLayer.hpp"
#include "../vendor/robtop/CustomSongLayer.hpp"
#include "../vendor/robtop/EditLevelLayer.hpp"
#include "../vendor/robtop/LocalLevelManager.hpp"
#include "../vendor/robtop/LevelBrowserLayer.hpp"
#include "../vendor/robtop/CustomLabelOptionsLayer.hpp"
#include "../vendor/robtop/MenuLayer.hpp"
#include "platform/android/jni/JniHelper.h"
#include "cocos2dExt.h"
#include "../vendor/cocos/cocos2dx/extensions/network/HttpClient.h"
#include "../vendor/cocos/cocos2dx/extensions/network/HttpRequest.h"
#include "../vendor/cocos/cocos2dx/extensions/network/HttpResponse.h"
#include <typeinfo>
#include "../vendor/robtop/CustomSongLayer.hpp"
#include <atomic>
#include <jni.h>
#include <cmath>
#include <map>
#include <cstdint>
#include <unordered_map>
#include <ctime>
#include <random>
#include <utility>
#include <new>
#include "../vendor/robtop/NoclipOptionsLayer.hpp"
#include "../vendor/robtop/SpeedhackOptionsLayer.hpp"
#include "../vendor/robtop/LevelInfoLayer.hpp"
#include "../vendor/robtop/DS_Dictionary.hpp"
// #include "../vendor/robtop/patch.h"
#include "../vendor/robtop/PlayLayer.hpp"
#include "../vendor/robtop/LevelEditorLayer.hpp"
#include "../vendor/robtop/CoderLayer.hpp"
#include "../vendor/robtop/CreatorLayer.hpp"
#include "../vendor/robtop/PivotMenuLayer.hpp"
// #include "../vendor/robtop/GJGameLevel.hpp"
#include "../vendor/robtop/EditorUI.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/GJComment.hpp"
#include "../vendor/robtop/PauseLayer.hpp"
#include "../vendor/cocos/cocos2dx/cocoa/CCDictionary.h"
#include "../vendor/cocos/cocos2dx/cocoa/CCDictionary.cpp"
#include "../vendor/cocos/cocos2dx/cocoa/CCString.h"
#include "../vendor/cocos/cocos2dx/cocoa/CCString.cpp"
#include "MemoryPatch.h"
#include <thread>
#include "../vendor/robtop/HidePauseLayer.hpp"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_android.h"
#include "minibpm/src/MiniBpm.h"
#include <sys/stat.h>
#include <signal.h>
#include <ctime>
#include <sstream>
#include <chrono>

#define PAGE_1 743276
#define PAGE_2 743277
#define CURRENT_VERSION 15
using namespace cocos2d;
using namespace cocos2d::extension;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

static GJGameLevel* g_currentExportLevel;
static GJGameLevel* g_importedLevel = nullptr;
static PlayerObject* g_player = nullptr;
bool noclip = false;
bool extraLayerCreated = false;
bool elc = false;
bool hasActivatedNoclip = false;
bool safeMode = false;
int deaths = 0;
int atts = 0;
int jumps = 0;
bool hideatts = false;
bool extrainfo = false;
GJGameLevel* gjlvl = nullptr;
int song = 0;
bool pmh = false;
const char* displayText;
const char* titleText;
bool doorClosed = false;
bool hitbox = true;
bool filter = false;
bool filterOption = false;
bool filterShowedMsg = false;
bool fps = false;
bool added = false;
bool noParticles = false;
bool noDeathEffect = false;
bool confirmExit = false;
bool iconHack = false;
bool speedhack = false;
int lastDeadFrame = 0;
int frameCount = 0;
bool noRotation = false;
bool noRespawn = false;
bool noRespawnReset = false;
bool pauseLayerVisible = true;
bool hidePauseMenu = false;
bool deleteAll = false;
bool levelLength = false;
float musicVolume = 1.0f;
CCLayer* menulayer = nullptr;
bool moreEditorButtons = false;
cocos2d::CCLabelBMFont* deathsLabel;
CCLabelBMFont* updateLabel;
bool warnedOutdated = false;
bool showSong = false;
bool showEpic = false;
int clicks = 0;
std::vector<float> clickTimestamps;
bool hasShowedAlert = false;
bool instantFlip = false;
bool dontShowAlert = false;
bool destroyAllOpps = false;
bool extendEditor = false;
bool exitButton = false;
bool hideVault = false;
bool noSceneTransition = false;
bool editorHitbox = false;
bool playerControl = false;
bool playback = false;

static JavaVM* g_vm = nullptr;
static bool g_playback = false;
static float g_lineX = 0.0f;
static CCNode* g_gameLayer = nullptr;
static CCMenuItemSpriteExtra* g_playBtn = nullptr;
static CCMenuItemSpriteExtra* g_stopBtn = nullptr;

bool ExtraLayer::m_flash = false;
float ExtraLayer::m_speedhack = 1.0f;
bool ExtraLayer::m_speedhackEnabled = false;
int ExtraLayer::m_currentPage = 1;
CCLayer* ExtraLayer::m_page1 = nullptr;
CCLayer* ExtraLayer::m_page2 = nullptr;
CCLayer* ExtraLayer::m_page3 = nullptr;
CCLayer* ExtraLayer::m_page4 = nullptr;
CCLayer* ExtraLayer::m_page5 = nullptr;
CCMenu* ExtraLayer::m_right = nullptr;
CCMenu* ExtraLayer::m_left = nullptr;
CCLayer* ExtraLayer::m_lel = nullptr;
CCLayer* HidePauseLayer::m_pauseLayer = nullptr;
MenuLayer* MenuLayer::sharedLayer = nullptr;
CCLayer* ExtraLayer::m_self = nullptr;
std::string ExtraLayer::m_customLabelValue = "Custom!";
bool ExtraLayer::m_speedhackMusic = false;
cocos2d::ccColor3B ExtraLayer::m_flashColor = {255, 0, 0};
int datedVersion = 0;
float lastSpeed = -1.0f; 
bool ExtraLayer::uploadNotAllowed_ = false;

bool fps_label = false;
bool deaths_label = false;
bool time_label = false;
bool speedhack_label = false;
bool clicks_label = false;
bool cheat_label = false;
bool cps_label = false;
bool custom_label = false;

std::string labelFPSDisplay = (char*)"";
std::string labelDeathsDisplay = (char*)"";
std::string labelTimeDisplay = (char*)"";
std::string labelSpeedhackDisplay = (char*)"";
std::string labelClicksDisplay = (char*)"";
std::string labelCPSDisplay = (char*)"";
std::string labelCustomDisplay = (char*)"";

CCLabelBMFont* labelInfo = nullptr;

static FMOD::System* fmodSystem = nullptr;
static FMOD::Channel* bgmChannel = nullptr;
static FMOD::Sound* bgmSound = nullptr;
static std::map<std::string, FMOD::Sound*> sfxCache;
static std::map<unsigned int, FMOD::Channel*> effectChannels;
static unsigned int effectIdCounter = 0;
static FMOD::Channel* sfxChannel = nullptr;
static FMOD::Sound* sfxSound = nullptr; 
static FMOD::Sound* cachedSound = nullptr;

std::string currentSpecificVersion = "15.14";

bool checked = false;
std::atomic<char*> MenuLayer::s_newVersion;
std::atomic<bool> MenuLayer::s_hasVersionData;

MemoryPatch flipPatch;

void* PivotMenuLayer::m_menuGame = nullptr;

class MenuGameLayer : public CCLayer {
public:
void tryJump();
};

MenuGameLayer* menuGame = nullptr;

bool (*MenuGameLayer_init)(MenuGameLayer*);
bool MenuGameLayer_init_H(MenuGameLayer* self) {
    MenuGameLayer_init(self);
    CCLog("VIOLETPS: MenuGameLayer::init()");
    menuGame = self;
    return true;
}

bool PivotMenuLayer::init() {
    this->setTouchEnabled(true); 
    CCLog("VIOLETPS: PivotMenuLayer::init()");
    return true;
}

/* class PlayerObject : public CCObject {
public:
void pushButton(int);
void releaseButton(int);
}; */

typedef void (*t_playerBtnFunc)(void* playerInstance, int buttonID);

void handlePlayerInput(void* player, int buttonID, bool isPush) {
    static t_playerBtnFunc pPushBtn = nullptr;
    static t_playerBtnFunc pReleaseBtn = nullptr;
    
    if (pPushBtn == nullptr || pReleaseBtn == nullptr) {
        void* handle = dlopen("libgame.so", RTLD_LAZY);
        if (handle) {
            pPushBtn = (t_playerBtnFunc)dlsym(handle, "_ZN12PlayerObject10pushButtonE12PlayerButton");
            pReleaseBtn = (t_playerBtnFunc)dlsym(handle, "_ZN12PlayerObject13releaseButtonE12PlayerButton");
        }
    }

    if (player) {
        if (isPush && pPushBtn) pPushBtn(player, buttonID);
        else if (!isPush && pReleaseBtn) pReleaseBtn(player, buttonID);
    }
}

typedef void (*t_playerDestroyed)(void* self);
t_playerDestroyed pPlayerDestroyed = nullptr;

void initDestroyedSymbol() {
    void* handle = dlopen("libgame.so", RTLD_LAZY);
    if (handle) {
        pPlayerDestroyed = (t_playerDestroyed)dlsym(handle, "_ZN12PlayerObject15playerDestroyedEv");
    }
}

void PivotMenuLayer::onFinished(CCObject* sender) {
    cocos2d::CCNode* player = (cocos2d::CCNode*)*(void**)((char*)menuGame + 0x15c);
    player->setPosition(ccp(0.0f, player->getPosition().y));
    player->setVisible(true);
}

    bool PivotMenuLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if(menuGame != nullptr) {
        auto player = MEMBER_BY_OFFSET(PlayerObject*, menuGame, 0x15c);
        if (player != nullptr) {
            handlePlayerInput(player, 1, true);
        }
    }
    return true;
        initDestroyedSymbol();
    if (menuGame != nullptr) {
        cocos2d::CCNode* player = (cocos2d::CCNode*)*(void**)((char*)menuGame + 0x15c);
        if (player != nullptr) {
            CCPoint worldPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
            CCPoint localPoint = player->convertToNodeSpace(worldPoint);
            CCSize s = player->getContentSize();
            float w = (s.width > 0) ? s.width : 30.0f;
            float h = (s.height > 0) ? s.height : 30.0f;
            CCRect localRect = CCRectMake(0, 0, w, h);

            if (CCRect::CCRectContainsPoint(localRect, localPoint)) {
                CCLog("VIOLETPS: Hit detected! Teleporting player to trigger respawn.");
                pPlayerDestroyed(player);
                CCPoint pos = player->getPosition();
                CCDelayTime* delay = CCDelayTime::create(2.0f);
    CCCallFuncO* teleport = CCCallFuncO::create(this, 
        callfuncO_selector(PivotMenuLayer::onFinished), player);
    auto seq = CCSequence::create(delay, teleport, NULL);
    player->runAction(seq);
                // player->setPosition(ccp(50000.0f, pos.y));        
                return true; 
            }
        }
    }
    return true; 
}

void PivotMenuLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    if(menuGame != nullptr) {
        void* player = *(void**)((char*)menuGame + 0x15c);
        if (player != nullptr) {
            handlePlayerInput(player, 1, false); 
        }
    }
}

void PivotMenuLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

class PatchManager {
private:
    vector<MemoryPatch> patches;
public:
    void addPatch(const char *libraryName, uintptr_t address,std::string hex){
        patches.push_back(MemoryPatch::createWithHex(libraryName,address,hex));
    }

    void Modify(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Modify();
        }
    }

    void Restore(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Restore();
        }
    }

};
PatchManager editorPatch;
PatchManager iconPatch;
PatchManager controlPatch;
PatchManager playerObjectPatch;

void onToggleFlip(bool enabled) {
    if (enabled) {
        flipPatch.Modify();
    } else {
        flipPatch.Restore();
    }
}

void onToggleEditor(bool enabled) {
    if (enabled) {
        editorPatch.Modify();
    } else {
        editorPatch.Restore();
    }
}

void onToggleIcon(bool enabled) {
    if (enabled) {
        iconPatch.Modify();
    } else {
        iconPatch.Restore();
    }
}

void onToggleControl(bool enabled) {
    if(enabled) {
        controlPatch.Modify();
    } else {
        controlPatch.Restore();
    }
}

void onTogglePlayerObject(bool enabled) {
    if(enabled) {
        playerObjectPatch.Modify();
    } else {
        playerObjectPatch.Restore();
    }
}

template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

static JNIEnv* getEnv() {
  if (!g_vm) return nullptr;
  JNIEnv* env = nullptr;
  jint res = g_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
  if (res == JNI_OK) return env;
  if (res == JNI_EDETACHED) {
    if (g_vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return nullptr;
    return env;
  }
  return nullptr;
}

void changeAllChannelsPitch(float pitch) {
  for (auto const& [id, channel] : effectChannels) {
    bool isPlaying = false;
    channel->isPlaying(&isPlaying);
    if (isPlaying) {
        channel->setPitch(1.5f);
    }
}
bgmChannel->setPitch(pitch);
}

CCSprite* getToggleSprite(CCSprite* on, CCSprite* off, bool state) { return (state) ? on : off; }
CCMenuItemSprite* getMenuToggleSprite(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; }

template <typename T, size_t size>
int random_array_index(const T (&array)[size]) {
    int num = std::rand() % size; 

    return num;
}

std::string formatWithChar(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

void openURL(const char* url) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxActivity", "openWebURL", "(Ljava/lang/String;)V")) {
        jstring stringArg = t.env->NewStringUTF(url);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
        t.env->DeleteLocalRef(stringArg);
        t.env->DeleteLocalRef(t.classID);
    }
}

void seekBackgroundMusicTo(int ms) {
    JNIEnv* env = getEnv();

    if (!env) {
        cocos2d::CCLog("Failed to get JNI environment");
        return;
    }
    jclass Cocos2dxActivity = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
    if (Cocos2dxActivity == nullptr) {
        cocos2d::CCLog("Failed to find Cocos2dxActivity class");
        return;
    }
    // some cocos2d java class names have been obfuscated, but not all
    jfieldID fieldID_backgroundMusicPlayer = env->GetStaticFieldID(Cocos2dxActivity, "backgroundMusicPlayer", "Lorg/cocos2dx/lib/p;");
    if (fieldID_backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of backgroundMusicPlayer");
        return;
    }
    jobject backgroundMusicPlayer = env->GetStaticObjectField(Cocos2dxActivity, fieldID_backgroundMusicPlayer);
    if (backgroundMusicPlayer == nullptr) {
        cocos2d::CCLog("Failed to get backgroundMusicPlayer");
        return;
    }

    jclass Cocos2dxMusic = env->FindClass("org/cocos2dx/lib/p");
    if (Cocos2dxMusic == nullptr) {
        cocos2d::CCLog("Failed to get Cocos2dxMusic");
        return;
    }
    jfieldID fieldID_mBackgroundMediaPlayer = env->GetFieldID(Cocos2dxMusic, "mBackgroundMediaPlayer", "Landroid/media/MediaPlayer;");
    if (fieldID_mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get field ID of mBackgroundMediaPlayer");
        return;
    }
    jobject mBackgroundMediaPlayer = env->GetObjectField(backgroundMusicPlayer, fieldID_mBackgroundMediaPlayer);
    if (mBackgroundMediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get mBackgroundMediaPlayer");
        return;
    }

    jclass MediaPlayer = env->GetObjectClass(mBackgroundMediaPlayer);
    if (MediaPlayer == nullptr) {
        cocos2d::CCLog("Failed to get MediaPlayer");
        return;
    }
    
    bool useNew = true;
    jclass versionClass;
    jfieldID sdkIntFieldID;
    jint sdkJint;
    int sdkInt;
    jmethodID seekTo;
    do {
        versionClass = env->FindClass("android/os/Build$VERSION");
        if (versionClass == nullptr) {
            cocos2d::CCLog("Warning: Failed to get class Build$VERSION. Using old \"broken\" method.");
            useNew = false;
            break;
        }
        sdkIntFieldID = env->GetStaticFieldID(versionClass, "SDK_INT", "I");
        if (sdkIntFieldID == nullptr) {
            cocos2d::CCLog("Warning: Failed to get field ID of SDK_INT. Using old \"broken\" method.");
            useNew = false;
            break;
        }

        sdkJint = env->GetStaticIntField(versionClass, sdkIntFieldID);
        sdkInt = static_cast<int>(sdkJint);
        if (sdkInt < 26) {
            cocos2d::CCLog("sdkInt: %i < 26. Using old \"broken\" method.", sdkInt);
            useNew = false;
            break;
        }
        seekTo = env->GetMethodID(MediaPlayer, "seekTo", "(JI)V");
        if (seekTo == nullptr) {
            cocos2d::CCLog("Warning: Failed to get method ID of seekTo(long, int). Using old \"broken\" method.");
            useNew = false;
            break;
        }
    } while (0);

    if (useNew) {
        env->CallVoidMethod(mBackgroundMediaPlayer, seekTo, static_cast<jlong>(static_cast<long>(ms)), static_cast<jint>(2));
    } else {
        jmethodID oldSeekTo = env->GetMethodID(MediaPlayer, "seekTo", "(I)V");
        if (oldSeekTo == nullptr) {
            cocos2d::CCLog("Failed to get method ID of seekTo(int)");
            return;
        }
        env->CallVoidMethod(mBackgroundMediaPlayer, oldSeekTo, static_cast<jint>(ms));
    } 
}

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

bool create_directories(const std::string& path) {
    std::stringstream ss(path);
    std::string item;
    std::string current_path = "";

    while (std::getline(ss, item, '/')) {
        if (item.empty()) continue;
        current_path += "/" + item;
        if (mkdir(current_path.c_str(), S_IRWXU) != 0) {
            if (errno != EEXIST) return false;
        }
    }
    return true;
}

int random(int max) {
    return std::rand() % max + 1;
}

int stoi(const std::string& s) {
    int result = 0;
    int multiplier = 1;
    if (s.empty()) return 0;
    for (size_t i = s.length(); i-- > 0;) {
        result += multiplier * (s[i] - '0');
        multiplier *= 10;
    }
    return result;
}

template <class R2>
R2 callFunctionFromSymbol(const char* symbol) {
    return reinterpret_cast<R2>(HookManager::getPointerFromSymbol(dlopen("libgame.so", RTLD_LAZY), symbol));
}

template <class R, class T>
R& from(T base, intptr_t offset) {
    return *reinterpret_cast<R*>(reinterpret_cast<uintptr_t>(base) + offset);
}

// test shitttt
#define cpatch(addr, val) addPatch("libgame.so", addr, val)

class LevelCell : public CCLayer {
public:
  virtual ~LevelCell() = default;
};

class SimpleAudioEngine {
  public:
  virtual ~SimpleAudioEngine() = default;
};

using namespace std;

#define cpatch(addr, val) addPatch("libgame.so", addr, val)

void PlayLayer::triggerCustomPulse(float duration)
{
    cocos2d::ccColor3B selectedColor = ExtraLayer::m_flashColor;
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pulseLayer = CCLayerColor::create(
        ccc4(selectedColor.r, selectedColor.g, selectedColor.b, 80), 
        win_size.width, 
        win_size.height
    );
    pulseLayer->setOpacity(80);
    pulseLayer->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
    pulseLayer->setAnchorPoint(ccp(0, 0));
    pulseLayer->setPosition(ccp(0, 0));
    this->addChild(pulseLayer, 10);
    CCActionInterval* fadeOut = CCFadeOut::create(duration);
    CCCallFunc* removeLayer = CCCallFunc::create(pulseLayer, callfunc_selector(CCNode::removeFromParentAndCleanup));
    auto sequence = CCSequence::create(fadeOut, removeLayer, NULL);
    pulseLayer->runAction(sequence);
}


void PlayLayer::pulseLabelRed(CCLabelBMFont* label, float duration)
{
    ccColor3B originalColor = ccc3(255, 255, 255); 
    ccColor3B pulseColor = ccc3(255, 0, 0);
    CCTintTo* tintToRed = CCTintTo::create(0.f, pulseColor.r, pulseColor.g, pulseColor.b);
    CCTintTo* tintToOriginal = CCTintTo::create(duration / 2.0f, originalColor.r, originalColor.g, originalColor.b);
    auto sequence = CCSequence::create(tintToRed, tintToOriginal, NULL);
    label->runAction(sequence);
}


char * (*LevelTools_getAudioTitle)(int ID);
char * LevelTools_getAudioTitle_H(int ID) {
  switch (ID) {
    case -1: return "Practice: Stay Inside Me";
    case 0: return "Stereo Madness";
    case 1: return "Back On Track";
    case 2: return "Polargeist";
    case 3: return "Dry Out";
    case 4: return "Base After Base";
    case 5: return "Cant Let Go";
    case 6: return "Jumper";
    case 7: return "Time Machine";
    case 8: return "Cycles";
    case 9: return "xStep";
    case 10: return "Clutterfunk";
    case 11: return "Theory Of Everything";
    case 12: return "Electroman Adventures";
    case 13: return "Clubstep";
    case 14: return "Active";
    case 15: return "Electrodynamix";
    case 16: return "Hexagon Force";
    case 17: return "Blast Processing";
    case 18: return "Theory Of Everything 2";
    case 19: return "Cosmic Dreamer";
    case 20: return "Sky Fortress";
    case 21: return "Sound of Infinity";
    case 22: return "Rupture";
    case 23: return "Stalemate";
    case 24: return "Glorious Morning";
    case 25: return "Chaoz Fantasy";
    case 26: return "Phazd";
    default: return "Custom Song";
  }
}

void (*SupportLayer_onEmail)(void*);
  void SupportLayer_onEmail_H(void*) {
    FLAlertLayer::create(
            nullptr,
            "Credits",
            CCString::createWithFormat("<cy>AntiMatter (Unsimply)</c>: For making updates 11-%i possible\n<cg>Gastiblast</c>: Creating the Pokemon Series \n<cl>Nikolyas</c>: Massive help with update 10 \n<cp>elektrick</c>: Making the \"nano\" logo \n<cr>Misty</c>: Patching the particles for the purple coins\n<cl> FMOD Studio, copyright Firelight Technologies Pty, Ltd.</c>\n\nYou: For playing!", CURRENT_VERSION)->getCString(),
            "OK",
            nullptr,
            600.f
        )->show();
        return;
  }

class ToggleHack {
  public:
    void toggleNoclip(CCObject*, GameManager* self) {noclip = !noclip;
    if(!hasActivatedNoclip && noclip) {
      FLAlertLayer::create(
            nullptr,
            "Warning",
            CCString::createWithFormat("Noclip will turn on Safe Mode and it will kick you out of the level if you try beating it.")->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
      hasActivatedNoclip = true;
    }
    return;
    }

    void patchNotes() {
      FLAlertLayer::create(
            nullptr,
            "Patch Notes",
            CCString::createWithFormat("1. Added <cr>Patch Notes</c>\n2. <cl>Practice Music Hack</c>\n3. <cg>Icon Hack</c>")->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
    }

    void showAtts() {
    auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
    cocos2d::CCArray* children = scene->getChildren();
    cocos2d::CCObject* obj;

    PauseLayer* pauseLayer = nullptr;
    CCARRAY_FOREACH(children, obj) {
        if (dynamic_cast<PauseLayer*>(obj)) {
            pauseLayer = (PauseLayer*)obj;
            break;
        }
    }
      if(!pauseLayer) {
        FLAlertLayer::create(
            nullptr,
            "Error",
            CCString::createWithFormat("You're <cr>not</c> currently playing a level!")->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
        return;
      }
      FLAlertLayer::create(
            nullptr,
            "Info",
            CCString::createWithFormat("<cy>Attempts:</c> %i\n<cg>Jumps:</c> %i", atts, jumps)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
    }

    void showLevelInfo(GJGameLevel* lvl) {
      int attempts = MEMBER_BY_OFFSET(int, lvl, 0x168);
      int totaljumps = MEMBER_BY_OFFSET(int, lvl, 0x16c);
      int normalpercent = MEMBER_BY_OFFSET(int, lvl, 0x170);
      int practicepercent = MEMBER_BY_OFFSET(int, lvl, 0x174);
      FLAlertLayer::create(
            nullptr,
            "Level Info",
            CCString::createWithFormat("<cg>Total Attempts:</c> %i\n<cl>Total Jumps:</c> %i\n<cp>Normal:</c> %i%%\n<co>Practice:</c> %i%%", attempts, totaljumps, normalpercent, practicepercent)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
    }

    void showMoreLevelInfo(GJGameLevel* lvl) {
      auto fVar2 = (float)(int)lvl / 311.0 * 1000.f;
      auto length = (int)(float)(int)floorf(fVar2);

      const char* audioTrack = LevelTools_getAudioTitle_H(MEMBER_BY_OFFSET(int, lvl, 0x148));
      FLAlertLayer::create(
            nullptr,
            "Level Info",
            CCString::createWithFormat("<cg>Length:</c> %i\n<cl>Song:</c> %s", length, audioTrack)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
    }

    void nothingHere() {
      FLAlertLayer::create(
            nullptr,
            "Patch Notes",
            CCString::createWithFormat("Nothing here..\n\n :)")->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
    }

    void hideAttempts() {hideatts = !hideatts;}

    void extraInfo() {extrainfo = !extrainfo;}

    void pmhf() {pmh = !pmh;}

    void hit() {hitbox = !hitbox;}

    void sf() {filterOption = !filterOption;
    if(!filterShowedMsg){
      FLAlertLayer::create(
            nullptr,
            "Warning",
            CCString::createWithFormat("Select Filter is really <cr>limited and buggy</c>, you should use it considering these facts.")->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
        filterShowedMsg = true;
    }}

    void dfps() {fps = !fps;}

    void np() {noParticles = !noParticles;}

    void nde() {noDeathEffect = !noDeathEffect;}

    void iconHackF() {iconHack = !iconHack;}

    void speedhackT() {speedhack = !speedhack;}

    void dummy(CCObject* pSender) {};

    void showCredits() {
      FLAlertLayer::create(
            nullptr,
            "Credits",
            CCString::createWithFormat("<cy>AntiMatter (Unsimply)</c>: For making updates 11-%i possible\n<cg>Gastiblast</c>: Creating the Pokemon Series \n<cl>Nikolyas</c>: Massive help with update 10 \n<cp>elektrick</c>: Making the \"nano\" logo \n<cr>Misty</c>: Patching the particles for the purple coins\n<cl> FMOD Studio, copyright Firelight Technologies Pty, Ltd.</c>\n\nYou: For playing!", CURRENT_VERSION)->getCString(),
            "OK",
            nullptr,
            600.f
        )->show();
    }

};

void (*GameStatsManager_incrementStat)(GameStatsManager* self, char* type, int amount);
void GameStatsManager_incrementStat_H(GameStatsManager* self, char* type, int amount) {
    GameStatsManager_incrementStat(self, type, amount);
}

std::string getUID() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, 
        "com/customRobTop/BaseRobTopActivity", 
        "getUserID", 
        "()Ljava/lang/String;")) 
    {
        jstring jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string uid = cocos2d::JniHelper::jstring2string(jstr);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jstr);
        return uid;
    }
    return "noUID";
}

  void ExtraLayer::incrementStat(GameStatsManager* self, char* type, int amount) {
    GameStatsManager_incrementStat(self, type, amount);
  } 
  bool ExtraLayer::saveSettingsToFile() {
    cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();
    
    def->setBoolForKey("noclip", noclip);
    def->setBoolForKey("hideatts", hideatts);
    def->setBoolForKey("extrainfo", extrainfo);
    def->setBoolForKey("fps", fps);
    def->setBoolForKey("noParticles", noParticles);
    def->setBoolForKey("noDeathEffect", noDeathEffect);
    def->setBoolForKey("speedhack", speedhack);
    def->setBoolForKey("noclipflash", ExtraLayer::m_flash);
    def->setFloatForKey("speedhackInt", ExtraLayer::m_speedhack);
    def->setBoolForKey("noRotation", noRotation);
    def->setBoolForKey("noRespawn", noRespawn);
    def->setBoolForKey("hidePauseMenu", hidePauseMenu);
    def->setBoolForKey("deleteAll", deleteAll);
    def->setBoolForKey("filterOption", filterOption);
    def->setBoolForKey("levelLength", levelLength);
    def->setFloatForKey("musicVolume", musicVolume);
    def->setBoolForKey("moreEditorButtons", moreEditorButtons);
    def->setBoolForKey("showEpic", showEpic);
    def->setBoolForKey("showSong", showSong);
    def->setBoolForKey("pmh", pmh);
    def->setBoolForKey("dontShowAlert", dontShowAlert);
    def->setBoolForKey("fps_label", fps_label);
    def->setBoolForKey("deaths_label", deaths_label);
    def->setBoolForKey("time_label", time_label);
    def->setBoolForKey("speedhack_label", speedhack_label);
    def->setBoolForKey("clicks_label", clicks_label);
    def->setBoolForKey("cheat_label", cheat_label);
    def->setBoolForKey("custom_label", custom_label);
    def->setBoolForKey("cps_label", cps_label);
    def->setBoolForKey("speedhackMusic", ExtraLayer::m_speedhackMusic);
    def->setBoolForKey("instantFlip", instantFlip);
    def->setBoolForKey("extendEditor", extendEditor);
    def->setBoolForKey("exitButton", exitButton);
    def->setBoolForKey("hideVault", hideVault);
    def->setBoolForKey("iconHack", iconHack);
    def->setBoolForKey("editorHitbox", editorHitbox);
    def->setBoolForKey("noSceneTransition", noSceneTransition);
    def->setBoolForKey("playerControl", playerControl);
    def->setBoolForKey("playback", playback);
    def->flush();
    return true;
  }

  void ExtraLayer::onLoadSettings() {
    cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();
  noclip = def->getBoolForKey("noclip", false);
  hideatts = def->getBoolForKey("hideatts", false);
  extrainfo = def->getBoolForKey("extrainfo", false);
  extrainfo = false;
  doorClosed = def->getBoolForKey("doorClosed", false);
  filterOption = def->getBoolForKey("filterOption", false);
  fps = def->getBoolForKey("fps", false);
  noParticles = def->getBoolForKey("noParticles", false);
  noDeathEffect = def->getBoolForKey("noDeathEffect", false);
  speedhack = def->getBoolForKey("speedhack", false);
  ExtraLayer::m_flash = def->getBoolForKey("noclipflash", false);
  ExtraLayer::m_speedhack = def->getFloatForKey("speedhackInt", 1.0f);
  noRotation = def->getBoolForKey("noRotation", false);
  noRespawn = def->getBoolForKey("noRespawn", false);
  hidePauseMenu = def->getBoolForKey("hidePauseMenu", false);
  deleteAll = def->getBoolForKey("deleteAll", false);
  levelLength = def->getBoolForKey("levelLength", false);
  musicVolume = def->getFloatForKey("musicVolume", 1.0f);
  moreEditorButtons = def->getBoolForKey("moreEditorButtons", false);
  showEpic = def->getBoolForKey("showEpic", false);
  showSong = def->getBoolForKey("showSong", false);
  pmh = def->getBoolForKey("pmh", false);
  dontShowAlert = def->getBoolForKey("dontShowAlert", false);
  fps_label = def->getBoolForKey("fps_label", false);
  deaths_label = def->getBoolForKey("deaths_label", false);
  time_label = def->getBoolForKey("time_label", false);
  speedhack_label = def->getBoolForKey("speedhack_label", false);
  clicks_label = def->getBoolForKey("clicks_label", false);
  cheat_label = def->getBoolForKey("cheat_label", false);
  custom_label = def->getBoolForKey("custom_label", false);
  cps_label = def->getBoolForKey("cps_label", false);
  ExtraLayer::m_speedhackMusic = def->getBoolForKey("speedhackMusic", false);
  instantFlip = def->getBoolForKey("instantFlip", false);
  extendEditor = def->getBoolForKey("extendEditor", false);
  exitButton = def->getBoolForKey("exitButton", false);
  hideVault = def->getBoolForKey("hideVault", false);
  iconHack = def->getBoolForKey("iconHack", false);
  editorHitbox = def->getBoolForKey("editorHitbox", false);
  noSceneTransition = def->getBoolForKey("noSceneTransition", false);
  playerControl = def->getBoolForKey("playerControl", false);
  playback = def->getBoolForKey("playback", false);
  }

  ExtraLayer* ExtraLayer::create(CCLayer* referrer) {
    auto ret = new ExtraLayer();
    if (ret->init(referrer)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ExtraLayer::onNoclipOptions() {
  auto extra = NoclipOptionsLayer::create(this);
    this->addChild(extra, 1000);
}

void ExtraLayer::onPEEOptions() {
  auto extra = SpeedhackOptionsLayer::create(this);
  this->addChild(extra, 1000);
}

void ExtraLayer::onCustomLabelOptions() {
  auto extra = CustomLabelOptionsLayer::create(this);
  this->addChild(extra, 1000);
}

void enablePage(bool enable, CCLayer* page) {
  CCObject* pObj = NULL;
  page->setVisible(enable);
    page->setTouchEnabled(enable);
    CCArray* children = page->getChildren(); 

CCARRAY_FOREACH(children, pObj) {
    CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
    if (menu) {
        menu->setEnabled(enable); 
    }
  }
}

void enableButton(bool enable, CCMenu* button) {
  button->setVisible(enable);
  button->setEnabled(enable);
}

void ExtraLayer::nextPage() {
  CCObject* pObj = NULL;
switch(ExtraLayer::m_currentPage) {
  case 1:
    enablePage(true, m_page2);
    enablePage(false, m_page1);
    enableButton(true, m_left);
    enableButton(true, m_right);
  break;
  case 2:
    enablePage(true, m_page3);
    enablePage(false, m_page2);
    enableButton(true, m_left);
    enableButton(true, m_right);
  break;
  case 3:
    enablePage(true, m_page4);
    enablePage(false, m_page3);
    enableButton(true, m_left);
    enableButton(true, m_right);
  break;
  case 4:
    enablePage(true, m_page5);
    enablePage(false, m_page4);
    enableButton(true, m_left);
    enableButton(false, m_right);
}
ExtraLayer::m_currentPage++;
}

void ExtraLayer::prevPage() {
  CCObject* pObj = NULL;
switch(ExtraLayer::m_currentPage) {
  case 2:
    enablePage(true, m_page1);
    enablePage(false, m_page2);
    enableButton(false, m_left);
    enableButton(true, m_right);
  break;
  case 3:
    enablePage(true, m_page2);
    enablePage(false, m_page3);
    enableButton(true, m_left);
    enableButton(true, m_right);
  break;
  case 4:
    enablePage(true, m_page3);
    enablePage(false, m_page4);
    enableButton(true, m_left);
    enableButton(true, m_right);
  break;
  case 5:
    enablePage(true, m_page4);
    enablePage(false, m_page5);
    enableButton(true, m_left);
    enableButton(true, m_right);
}
    ExtraLayer::m_currentPage--;
}

void ExtraLayer::toggle(CCObject* sender) {
    auto btn = static_cast<CCMenuItemToggler*>(sender);
    bool* toggleVar = static_cast<bool*>(btn->getUserData());
    if(toggleVar) *toggleVar = !*toggleVar;
}

void ExtraLayer::dummy(CCObject*) {
  return;
}

void ExtraLayer::volumeSliderCallback(CCObject* pSender, CCControlEvent controlEvent) {
    CCControlSlider* pSlider = (CCControlSlider*)pSender;
    float newVolume = pSlider->getValue();
    auto gman = GameManager::sharedState();
    // if(MEMBER_BY_OFFSET(int, gman, 0x171)) CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(newVolume);
    musicVolume = newVolume;
}

void PauseLayer::toggleVisibility() {
    CCObject* child;
    CCARRAY_FOREACH(this->getChildren(), child) {
        auto node = dynamic_cast<CCNode*>(child);
        if (node) node->setVisible(false);
        auto menu = dynamic_cast<CCMenu*>(node);
                if (menu) {
                    menu->setTouchEnabled(false);
                }
    }
    auto HPL = HidePauseLayer::create(this);
    this->addChild(HPL, 1000);
}

CCNode* ExtraLayer::optionToggler(const char* display, bool* toggleVar, bool addinfo, ExtraLayerInfo::InfoType infoType) {
  auto toggleOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
  auto toggleOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
  auto itemOff = CCMenuItemSprite::create(toggleOffSprite, toggleOffSprite, nullptr, nullptr);
  auto itemOn  = CCMenuItemSprite::create(toggleOnSprite,  toggleOnSprite,  nullptr, nullptr);

auto btn = CCMenuItemToggler::create(
    getMenuToggleSprite(itemOn, itemOff, *toggleVar), 
    getMenuToggleSprite(itemOff, itemOn, *toggleVar),  
    this, 
    menu_selector(ExtraLayer::toggle) 
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

CCMenu* ExtraLayer::createOptionsMenu(const char* display, int length, SEL_MenuHandler func) {
  auto sprite = ButtonSprite::create(
    CCString::createWithFormat("%s", display)->getCString(), length, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto menu = CCMenu::create();
  auto btn = CCMenuItemSpriteExtra::create(
    sprite,
    sprite,
    this,
    func
  );

  menu->addChild(btn);
  menu->setPosition(CCPointZero);
  return menu;
}

void ExtraLayer::exportSaveFile() {
bool gm = this->extractFile("CCGameManager.dat");
bool ll = this->extractFile("CCLocalLevels.dat");
// bool ud = this->extractFile("CCUserDefault.xml");

if(!gm || !ll/*|| !ud*/) return;

FLAlertLayer::create(
      nullptr,
      "Exported",
      CCString::createWithFormat("Exported save file to:\n<cy>/storage/emulated/0/violetps/userdata</c>")->getCString(),
      "OK",
      nullptr,
      300.f
    )->show();
}

bool ExtraLayer::extractFile(char const* file) {
    std::string internalPath = "/data/data/com.gastibx.pokemongdpps/" + std::string(file);
    std::string externalPath = "/storage/emulated/0/violetps/userdata/" + std::string(file);
    std::ifstream src(internalPath.c_str(), std::ios::binary);
    if (!src.is_open()) {
      FLAlertLayer::create(
      nullptr,
      "Error",
      CCString::createWithFormat("Error happened while trying to export to: \n<cy>%s</c>", externalPath.c_str())->getCString(),
      "OK",
      nullptr,
      300.f
    )->show();
      return false;
      }
    create_directories("/storage/emulated/0/violetps/userdata");
    std::ofstream dst(externalPath.c_str(), std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
    return true;
}

void ExtraLayer::manualSave(CCObject*) {
    saveSettingsToFile();
    FLAlertLayer::create(
      nullptr,
      "Success",
      "Succesfully saved all the settings",
      "OK",
      nullptr,
      300.f
    )->show();
}

void ExtraLayer::showUID(CCObject*) {
    FLAlertLayer::create(
      nullptr,
      "Info",
      CCString::createWithFormat("Your unique userID: <cy>%s</c>\n<cr>Do not show this to anybody.</c>", getUID().c_str())->getCString(),
      "OK",
      nullptr,
      300.f
    )->show();
}

  bool ExtraLayer::init(CCLayer* self) {

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
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("GJ_square01-hd.png", rect);
    panel->setContentSize(CCSizeMake(win_size.width - 75, win_size.height - 25));
    leftParent->addChild(panel);
    panel->setPosition({0.f, 0.f});

    CCLayer* mainLayoutLayer = CCLayer::create();
    mainLayoutLayer->setAnchorPoint(ccp(0.0f, 0.0f));


auto buttonMenu = CCMenu::create();
auto button = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, this, menu_selector(ExtraLayer::keyBackClicked));
menuBtn->setPosition({-50,-25});
buttonMenu->setPosition({win_size.width, win_size.height});
buttonMenu->addChild(menuBtn);
contentHolder->addChild(buttonMenu);

auto rightButtonMenu = CCMenu::create();
auto rightButton = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
rightButton->setFlipX(true);
CCMenuItemSpriteExtra* rightMenuBtn = CCMenuItemSpriteExtra::create(rightButton, rightButton, this, menu_selector(ExtraLayer::nextPage));
rightMenuBtn->setPosition({0,0});
rightButtonMenu->setPosition({win_size.width - 20, win_size.height / 2});
rightButtonMenu->addChild(rightMenuBtn);
m_right = rightButtonMenu;
contentHolder->addChild(rightButtonMenu);

auto leftButtonMenu = CCMenu::create();
auto leftButton = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
CCMenuItemSpriteExtra* leftMenuBtn = CCMenuItemSpriteExtra::create(leftButton, leftButton, this, menu_selector(ExtraLayer::prevPage));
leftMenuBtn->setPosition({0,0});
leftButtonMenu->setPosition({20, win_size.height / 2});
leftButtonMenu->addChild(leftMenuBtn);
m_left = leftButtonMenu;
contentHolder->addChild(leftButtonMenu);

  int padding = 35;
  int startX = 100;
  int endX = startX * 3 + 10;

  auto noclipBtn = this->optionToggler("No-Clip", &noclip);
  noclipBtn->setPosition(startX, win_size.height - 100);
  auto noclipBtnMore = this->createOptionsMenu("+", 50, menu_selector(ExtraLayer::onNoclipOptions));
  noclipBtnMore->setPosition(ccp(noclipBtn->getPosition().x + noclipBtn->getContentSize().width, noclipBtn->getPosition().y));

  auto speedhackBtn = this->optionToggler("Speedhack", &speedhack, true, ExtraLayerInfo::InfoType::PEE);
  speedhackBtn->setPosition(startX, win_size.height - 100 - padding);
  auto speedhackBtnMore = this->createOptionsMenu("+", 50, menu_selector(ExtraLayer::onPEEOptions));
  speedhackBtnMore->setPosition(ccp(noclipBtnMore->getPosition().x, speedhackBtn->getPosition().y));

  auto percentageBtn = this->optionToggler("Display Percentage", &noParticles);
  percentageBtn->setPosition(startX, win_size.height - 100 - (padding * 2));

  auto noDeathEffectBtn = this->optionToggler("No Death Effect", &noDeathEffect);
  noDeathEffectBtn->setPosition(startX, win_size.height - 100 - (padding * 3));

  auto hideAttemptsBtn = this->optionToggler("Hide Attempts", &hideatts);
  hideAttemptsBtn->setPosition(startX, win_size.height - 100 - (padding * 4));

  auto playSessionInfoBtn = this->optionToggler("Hide Pause Menu", &hidePauseMenu, true, ExtraLayerInfo::InfoType::HPM);
  playSessionInfoBtn->setPosition(endX, win_size.height - 100);

  auto noRotationBtn = this->optionToggler("No Rotation", &noRotation);
  noRotationBtn->setPosition(endX, win_size.height - 100 - (padding));

  auto noRespawnBtn = this->optionToggler("No Respawn Time", &noRespawn, true, ExtraLayerInfo::InfoType::NRT);
  noRespawnBtn->setPosition(endX, win_size.height - 100 - (padding * 2));

  auto pmhBtn = this->optionToggler("Practice Music Hack", &pmh);
  pmhBtn->setPosition(endX, win_size.height - 100 - (padding * 3));

  auto instantFlipBtn = this->optionToggler("No Mirror Transition", &instantFlip);
  instantFlipBtn->setPosition(endX, win_size.height - 100 - (padding * 4));

  /* auto menu7 = CCMenu::create();
  auto noclipBtnSprite = ButtonSprite::create(
    "Practice UI", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto noclipButton = CCMenuItemSpriteExtra::create(
    noclipBtnSprite,
    noclipBtnSprite,
    this,
    menu_selector(ToggleHack::toggleNoclip)
  );

  noclipButton->setScale(0.8f);
  menu7->addChild(noclipButton); */

auto creditsBtnSprite = ButtonSprite::create(
    "Credits", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto creditsBtn = CCMenuItemSpriteExtra::create(
    creditsBtnSprite,
    creditsBtnSprite,
    this,
    menu_selector(ToggleHack::showCredits)
  );

auto creditsMenu = CCMenu::create();
creditsMenu->addChild(creditsBtn);
creditsMenu->setPosition(ccp(100, 35));

auto sessionBtnSprite = ButtonSprite::create(
    "Session Info", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto sessionBtn = CCMenuItemSpriteExtra::create(
    sessionBtnSprite,
    sessionBtnSprite,
    this,
    menu_selector(ToggleHack::showAtts)
  );

auto sessionMenu = CCMenu::create();
sessionMenu->addChild(sessionBtn);
sessionMenu->setPosition(ccp(100, 35 + sessionBtnSprite->getContentSize().height + 10));

auto exportBtnSprite = ButtonSprite::create(
    "Export Save File", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto exportBtn = CCMenuItemSpriteExtra::create(
    exportBtnSprite,
    exportBtnSprite,
    this,
    menu_selector(ExtraLayer::exportSaveFile)
  );

auto exportMenu = CCMenu::create();
exportMenu->addChild(exportBtn);
exportMenu->setPosition(ccp(100, 35 + (exportBtnSprite->getContentSize().height * 2) + 20));

auto manualSaveBtnSprite = ButtonSprite::create(
    "Save Settings", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto manualSaveBtn = CCMenuItemSpriteExtra::create(
    manualSaveBtnSprite,
    manualSaveBtnSprite,
    this,
    menu_selector(ExtraLayer::manualSave)
  );

auto manualSaveMenu = CCMenu::create();
manualSaveMenu->addChild(manualSaveBtn);
manualSaveMenu->setPosition(ccp(100, 35 + (manualSaveBtnSprite->getContentSize().height * 3) + 30));

auto getUIDBtnSprite = ButtonSprite::create(
    "Show UID", 100, 0, 1, false, "goldFont.fnt", "GJ_button_01-hd.png"
  );
  
  auto getUIDBtn = CCMenuItemSpriteExtra::create(
    getUIDBtnSprite,
    getUIDBtnSprite,
    this,
    menu_selector(ExtraLayer::showUID)
  );

auto getUIDMenu = CCMenu::create();
getUIDMenu->addChild(getUIDBtn);
getUIDMenu->setPosition(ccp(100, 35 + (getUIDBtnSprite->getContentSize().height * 4) + 40));

auto filterBtn = this->optionToggler("Select Filter", &filterOption, true, ExtraLayerInfo::InfoType::SF);
filterBtn->setPosition(startX, win_size.height - 100);

auto deleteAllBtn = this->optionToggler("Delete All", &deleteAll, true, ExtraLayerInfo::InfoType::DA);
deleteAllBtn->setPosition(startX, win_size.height - 100 - padding);

auto levelLengthBtn = this->optionToggler("Show Level Length", &levelLength, true, ExtraLayerInfo::InfoType::LL);
levelLengthBtn->setPosition(startX, win_size.height - 100 - (padding * 2));

auto editorExtensionBtn = this->optionToggler("Editor Extension", &extendEditor, true, ExtraLayerInfo::InfoType::EX);
editorExtensionBtn->setPosition(startX, win_size.height - 100 - (padding * 3));

auto MEBBtn = this->optionToggler("More Editor Buttons", &moreEditorButtons, true, ExtraLayerInfo::InfoType::MEB);
MEBBtn->setPosition(startX, win_size.height - 100 - (padding * 4));

auto EHBtn = this->optionToggler("Editor Hitboxes", &editorHitbox, true, ExtraLayerInfo::InfoType::EH);
EHBtn->setPosition(endX, win_size.height - 100);

auto playbackBtn = this->optionToggler("Audio Playback", &playback);
playbackBtn->setPosition(endX, win_size.height - 100 - padding);







auto epicBtn = this->optionToggler("Display Epic Icons", &showEpic);
epicBtn->setPosition(startX, win_size.height - 100);

auto songBtn = this->optionToggler("Display Audio Track", &showSong, true, ExtraLayerInfo::InfoType::DAT);
songBtn->setPosition(startX, win_size.height - 100 - padding);

auto songAlertBtn = this->optionToggler("Disable Song Alert", &dontShowAlert);
songAlertBtn->setPosition(startX, win_size.height - 100 - (padding * 2));

auto exitBtnBtn = this->optionToggler("Add Quit Button", &exitButton, true, ExtraLayerInfo::InfoType::QB);
exitBtnBtn->setPosition(startX, win_size.height - 100 - (padding * 3));

auto hideVaultBtn = this->optionToggler("Hide Vault", &hideVault, true, ExtraLayerInfo::InfoType::HV);
hideVaultBtn->setPosition(startX, win_size.height - 100 - (padding * 4));

auto iconHackBtn = this->optionToggler("Icon Hack", &iconHack);
iconHackBtn->setPosition(endX, win_size.height - 100);

auto noTransitionBtn = this->optionToggler("No Scene Transition", &noSceneTransition);
noTransitionBtn->setPosition(endX, win_size.height - 100 - (padding));

auto controlBtn = this->optionToggler("Control Start Menu Icons", &playerControl, true, ExtraLayerInfo::InfoType::CTP);
controlBtn->setPosition(endX, win_size.height - 100 - (padding * 2));


CCControlSlider* pSlider = CCControlSlider::create(
        "slidergroove.png", 
        "00_transparent.png", 
        "sliderthumb.png"
    );
    pSlider->setMinimumValue(0.0f);
    pSlider->setMaximumValue(1.0f);
    float currentVol = CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
    pSlider->setValue(currentVol);
    pSlider->setPosition(ccp(win_size.width / 2, 30));
    pSlider->addTargetWithActionForControlEvents(
        this, 
        cccontrol_selector(ExtraLayer::volumeSliderCallback), 
        CCControlEventValueChanged
    );

    auto volumeLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Music Volume")->getCString(), 
            "bigFont.fnt"
        );
  volumeLabel->setScale(0.5f);
  volumeLabel->setPosition(ccp(pSlider->getPosition().x, 50));

  auto titleLabel1 = CCLabelBMFont::create(
            CCString::createWithFormat("Gameplay")->getCString(), 
            "goldFont-hd.fnt"
        );

  auto labelFPS = this->optionToggler("FPS", &fps_label);
  labelFPS->setPosition(startX, win_size.height - 100);

  auto labelDeaths = this->optionToggler("Deaths", &deaths_label);
  labelDeaths->setPosition(startX, win_size.height - 100 - padding);

  auto labelTime = this->optionToggler("Time", &time_label);
  labelTime->setPosition(startX, win_size.height - 100 - (padding * 2));

  auto labelCustom = this->optionToggler("Custom", &custom_label);
  labelCustom->setPosition(startX, win_size.height - 100 - (padding * 3));
  auto customLabelBtnMore = this->createOptionsMenu("+", 50, menu_selector(ExtraLayer::onCustomLabelOptions));
  customLabelBtnMore->setPosition(ccp(labelCustom->getPosition().x + labelCustom->getContentSize().width, labelCustom->getPosition().y));

  auto labelSpeedhack = this->optionToggler("Speedhack", &speedhack_label);
  labelSpeedhack->setPosition(startX, win_size.height - 100 - (padding * 4));

  auto labelClicks = this->optionToggler("Clicks", &clicks_label);
  labelClicks->setPosition(endX, win_size.height - 100);

  auto labelCPS = this->optionToggler("CPS", &cps_label);
  labelCPS->setPosition(endX, win_size.height - 100 - (padding));

  titleLabel1->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel1->setScale(1.0f);

  auto titleLabel2 = CCLabelBMFont::create(
            CCString::createWithFormat("Editor")->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel2->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel2->setScale(1.0f);

  auto titleLabel3 = CCLabelBMFont::create(
            CCString::createWithFormat("Misc")->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel3->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel3->setScale(1.0f);

  auto titleLabel4 = CCLabelBMFont::create(
            CCString::createWithFormat("Labels")->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel4->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel4->setScale(1.0f);

  auto titleLabel5 = CCLabelBMFont::create(
            CCString::createWithFormat("Tools")->getCString(), 
            "goldFont-hd.fnt"
        );

  titleLabel5->setPosition({win_size.width / 2, win_size.height - 35});
  titleLabel5->setScale(1.0f);

m_page1 = CCLayer::create();
m_page2 = CCLayer::create();
m_page3 = CCLayer::create();
m_page4 = CCLayer::create();
m_page5 = CCLayer::create();
m_page1->addChild(noclipBtn);
m_page1->addChild(noclipBtnMore);
m_page1->addChild(speedhackBtn);
m_page1->addChild(speedhackBtnMore);
m_page1->addChild(percentageBtn);
m_page1->addChild(noDeathEffectBtn);
m_page1->addChild(hideAttemptsBtn);
m_page1->addChild(playSessionInfoBtn);
m_page1->addChild(noRotationBtn);
m_page1->addChild(noRespawnBtn);
m_page1->addChild(pmhBtn);
m_page1->addChild(instantFlipBtn);

  m_page2->addChild(filterBtn);
  m_page2->addChild(deleteAllBtn);
  m_page2->addChild(levelLengthBtn);
  m_page2->addChild(MEBBtn);
  m_page2->addChild(editorExtensionBtn);
  m_page2->addChild(EHBtn);
  m_page2->addChild(playbackBtn);

  m_page5->addChild(creditsMenu);
  m_page5->addChild(sessionMenu);
  m_page5->addChild(exportMenu);
  m_page5->addChild(manualSaveMenu);
  m_page5->addChild(getUIDMenu);
  m_page3->addChild(epicBtn);
  m_page3->addChild(songBtn);
  m_page3->addChild(songAlertBtn);
  m_page3->addChild(exitBtnBtn);
  m_page3->addChild(hideVaultBtn);
  m_page3->addChild(iconHackBtn);
  m_page3->addChild(noTransitionBtn);
  m_page3->addChild(controlBtn);

  if(random(100) == 1) {
auto oppsBtn = this->optionToggler("Destroy All Opps", &destroyAllOpps, true, ExtraLayerInfo::InfoType::OPPS);
oppsBtn->setPosition(endX, win_size.height - 100 - (padding * 3));
m_page3->addChild(oppsBtn);
}

  m_page4->addChild(labelFPS);
  m_page4->addChild(labelDeaths);
  m_page4->addChild(labelTime);
  m_page4->addChild(labelCustom);
  m_page4->addChild(customLabelBtnMore);
  m_page4->addChild(labelSpeedhack);
  m_page4->addChild(labelClicks);
  m_page4->addChild(labelCPS);

  m_page1->addChild(titleLabel1);
  m_page2->addChild(titleLabel2);
  m_page3->addChild(titleLabel3);
  m_page4->addChild(titleLabel4);
  m_page5->addChild(titleLabel5);
enablePage(true, m_page1);
enablePage(false, m_page2);
enablePage(false, m_page3);
enablePage(false, m_page4);
enablePage(false, m_page5);
CCObject* pObj = NULL;

CCArray* childrenself = self->getChildren(); 

CCARRAY_FOREACH(childrenself, pObj) {
    CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
    if (menu) {
        menu->setEnabled(false); 
    }
  }

  m_self = self;

  m_left->setVisible(false);
    m_left->setEnabled(false);

    m_right->setVisible(true);
    m_right->setEnabled(true);

  contentHolder->addChild(m_page1);
  contentHolder->addChild(m_page2);
  contentHolder->addChild(m_page3);
  contentHolder->addChild(m_page4);
  contentHolder->addChild(m_page5);
  contentHolder->addChild(mainLayoutLayer);

  auto infoLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Version: %i", CURRENT_VERSION)->getCString(), 
            "chatFont.fnt"
        );
        infoLabel->setScale(0.5f);
        infoLabel->setAnchorPoint(CCPointMake(0.5f, 0.5f));
        infoLabel->setColor(ccc3(129, 68, 37));
        infoLabel->setPosition(win_size.width - 75, 25);

        contentHolder->addChild(infoLabel);

  
  this->setKeypadEnabled(true);
  return true;
  }

  void ExtraLayer::onClose(CCObject* sender) {
    if (sender) this->retain();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
    elc = false;
    ExtraLayer::m_currentPage = 1;
    CCArray* childrenself = m_self->getChildren();
    CCObject* pObj = NULL; 

CCARRAY_FOREACH(childrenself, pObj) {
    CCMenu* menu = dynamic_cast<CCMenu*>(pObj);
    if (menu) {
        menu->setEnabled(true); 
    }
  }
    this->saveSettingsToFile();
  }

  void ExtraLayer::keyBackClicked() {
    onClose(nullptr);
  }

  void ExtraLayer::showInfo(CCObject* sender) {
    ExtraLayerInfo::InfoType type = static_cast<ExtraLayerInfo::InfoType>(reinterpret_cast<intptr_t>(static_cast<CCMenu*>(sender)->getUserData()));
    const char* displayText;
    switch(type) {
      case ExtraLayerInfo::InfoType::PMH: 
      displayText = "Replaces practice music.\n<cr>WARNING:</c> It might not work as intended!";
      titleText = "Practice Music Hack";
      break;
      case ExtraLayerInfo::InfoType::HA: 
      displayText = "Hides the Attempt Label while playing.";
      titleText = "Hide Attempts";
      break;
      case ExtraLayerInfo::InfoType::SEI: 
      displayText = "Shows Attempts and Jumps on your current level session.";
      titleText = "Show Extra Info";
      break;
      case ExtraLayerInfo::InfoType::SF:
      displayText = "Enables Select Filter.";
      titleText = "Select Filter";
      break;
      case ExtraLayerInfo::InfoType::FPS:
      displayText = "Displays Framerate. \n <cy>If you're using an android emulator on PC, you don't need this option.</c>";
      titleText = "Show FPS";
      break;
      case ExtraLayerInfo::InfoType::NP:
      displayText = "Displays percentage as a number.";
      titleText = "Show Percentage";
      break;
      case ExtraLayerInfo::InfoType::NDE:
      displayText = "Removes all effects when dying.";
      titleText = "No Death Effect";
      break;
      case ExtraLayerInfo::InfoType::NOCLIP:
      displayText = "Disables death on collision. <cr>Turns on Safe Mode automatically</c>";
      titleText = "No-Clip";
      break;
      case ExtraLayerInfo::InfoType::DEATHS:
      displayText = "Shows deaths when turning noclip on.";
      titleText = "NoClip Deaths";
      break;
      case ExtraLayerInfo::InfoType::FLASH:
      displayText = "Shows a red flash over the screen on death.";
      titleText = "Show Flash";
      break;
      case ExtraLayerInfo::InfoType::PEE:
      displayText = "Turns on speedhack. <cr>Turns on Safe Mode automatically</c>";
      titleText = "Speedhack";
      break;
      case ExtraLayerInfo::InfoType::NRT:
      displayText = "Removes wait time for respawn, respawn position might be unconsistent.";
      titleText = "No Death Effect";
      break;
      case ExtraLayerInfo::InfoType::HPM:
      displayText = "Allows you to hide the pause menu, you can zoom and move around the screen.";
      titleText = "Hide Pause Menu";
      break;
      case ExtraLayerInfo::InfoType::DA:
      displayText = "<cg>Deletes all objects of the same type</c> your selected object has. If multiple objects selected, the first one will be chosen.";
      titleText = "Delete All";
      break;
      case ExtraLayerInfo::InfoType::LL:
      displayText = "Displays the length of the level in the editor pause menu.";
      titleText = "Show Level Length";
      break;
      case ExtraLayerInfo::InfoType::MEB:
      displayText = "Adds 1/2 grid move buttons and 1/30 grid move buttons.";
      titleText = "More Editor Buttons";
      break;
      case ExtraLayerInfo::InfoType::DAT:
      displayText = "<cg>Displays the song of the level</c> in the main page.";
      titleText = "Display Audio Track";
      break;
      case ExtraLayerInfo::InfoType::SPEEDM:
      displayText = "Changes the speed of the song when using speedhack.";
      titleText = "Speedhack Music";
      break;
      case ExtraLayerInfo::InfoType::OPPS:
      displayText = "A mod that destroys all <cr>opps.</c>";
      titleText = "Destroy All Opps";
      break;
      case ExtraLayerInfo::InfoType::EX:
      displayText = "Extends the editor up to 524280 pixels.";
      titleText = "Editor Extension";
      break;
      case ExtraLayerInfo::InfoType::QB:
      displayText = "<cg>Adds a quit button</c> to the top right part of the screen in the start menu.";
      titleText = "Add Quit Button";
      break;
      case ExtraLayerInfo::InfoType::HV:
      displayText = "Disables the <cp>Forgotten Vault</c> door in the create menu.";
      titleText = "Hide Vault";
      break;
      case ExtraLayerInfo::InfoType::EH:
      displayText = "Adds hitboxes to the editor";
      titleText = "Editor Hitboxes";
      break;
      case ExtraLayerInfo::InfoType::CTP:
      displayText = "Allows you to <cg>control the icons in the start menu.</c>";
      titleText = "Control Start Menu Icons";
      break;
      default:
      displayText = "<cr>unknown error</c>";
      titleText = "ERROR";
    }
    FLAlertLayer::create(
            nullptr,
            CCString::createWithFormat("Info")->getCString(),
            CCString::createWithFormat("%s", displayText)->getCString(),
            "OK",
            nullptr,
            300.f
        )->show();
  }

ExtraLayer* extra = nullptr;
  void PauseLayer::onOpenMenu() {
    if(!elc) { //HaxOverlay::create(this);
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    extra = ExtraLayer::create(this);
    this->addChild(extra, 1000);
    elc = true;
    extra->_setZOrder(10000000);
    } else {
      menu_selector(ExtraLayer::onClose);
    }
}

std::map<GJGameLevel*, int> magic;

GJGameLevel* (*GJGameLevel_create)(CCDictionary* dict);
GJGameLevel* GJGameLevel_create_H(CCDictionary* dict) {
  auto lvl = GJGameLevel_create(dict);
  if (dict->valueForKey("42")->intValue() > 0) magic[lvl] = 1;
  else magic[lvl] = 0;
  return lvl;
}

void (*GJGameLevel_encodeWithCoder)(GJGameLevel* self, DS_Dictionary* dsd);
void GJGameLevel_encodeWithCoder_H(GJGameLevel* self, DS_Dictionary* dsd) {
  GJGameLevel_encodeWithCoder(self, dsd);
  dsd->setIntegerForKey("k42", magic[self]);
}

GJGameLevel* (*GJGameLevel_createWithCoder)(DS_Dictionary* dsd);
GJGameLevel* GJGameLevel_createWithCoder_H(DS_Dictionary* dsd) {
  auto lvl = GJGameLevel_createWithCoder(dsd);
  magic[lvl] = dsd->getIntegerForKey("k42");
  return lvl;
}

void (*GJGameLevel_destructor)(GJGameLevel*);
void GJGameLevel_destructor_H(GJGameLevel* self) {
  auto val = magic.find(self);
  if (val != magic.end()) magic.erase(val);
  GJGameLevel_destructor(self);
}

 int GJGameLevel::getFeatured(GJGameLevel* self) { return 0; }
void (*LevelCell_loadCustomLevelCell)(LevelCell*);
void LevelCell_loadCustomLevelCell_H(LevelCell* self) {
  LevelCell_loadCustomLevelCell(self);
  if(!showEpic) return;
  GJGameLevel* gamelevel;
  auto lvl = from<GJGameLevel*>(self, 0x180);
  int epic = MEMBER_BY_OFFSET(int, lvl, 0x184);
  if (epic < 2) return;
  CCSprite* frame = CCSprite::create("GJ_epicCoin_001.png");
   frame->retain();
    frame->setPosition({26.f, 40.f});
    frame->_setZOrder(-1);
    self->addChild(frame);
     frame->release();
}

bool (*LevelInfoLayer_init)(CCLayer* self, GJGameLevel* lvl);
bool LevelInfoLayer_init_H(CCLayer* self, GJGameLevel* lvl) {
  auto win_size = CCDirector::sharedDirector()->getWinSize();
  LevelInfoLayer_init(self, lvl);
  auto epic = MEMBER_BY_OFFSET(int, lvl, 0x184);
  if(showSong) {
  int audioTrack = MEMBER_BY_OFFSET(int, lvl, 0x148);
    CCNode* containerNode = CCNode::create();
    auto trackLabel = CCLabelBMFont::create(
            CCString::createWithFormat("%s", LevelTools_getAudioTitle_H(audioTrack))->getCString(), 
            "bigFont.fnt"
        );
  std::string audiotitlestd = LevelTools_getAudioTitle_H(audioTrack);
  trackLabel->setScale(0.5f);
  CCSprite* note = CCSprite::createWithSpriteFrameName("GJ_noteIcon_001.png");
  note->setPosition({win_size.width / 2 + 60, win_size.height / 2 + 30});
  trackLabel->setAnchorPoint(CCPointMake(0.0f, 0.5f));
  trackLabel->setPosition({note->getPosition().x + 20, note->getPosition().y});
  containerNode->addChild(trackLabel);
  containerNode->addChild(note);
  self->addChild(containerNode);
      }
  if(!showEpic) return 1;

/* auto button2 = CCSprite::create("GJ_duplicateBtn_001.png");
CCTexture2D *texture = button2->getTexture();
    texture->setAntiAliasTexParameters(); 
CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, self, menu_selector(LevelInfoLayer::onClone));
menuBtn2->setScale(1.f);
menuBtn2->_setZOrder(100);
menuBtn2->setPosition({10, win_size.height / 2});
auto buttonMenu2 = CCMenu::create(menuBtn2, NULL);
self->addChild(buttonMenu2); */
  if (epic < 2) return true;
  CCSprite* frame = CCSprite::create("GJ_epicCoin_001.png");
   frame->retain();
    frame->setPosition({(win_size.width / 2) - 120, (win_size.height / 2) + 35});
    frame->_setZOrder(-1);
    self->addChild(frame);
     frame->release();
  gjlvl = lvl;
  return true;
}

struct CustomSongData {
    float bpm;
    float duration;
};

char* audioFiles[] = {
    "StereoMadness.mp3",
    "BackOnTrack.mp3",       
    "Polargeist.mp3",   
    "DryOut.mp3",       
    "BaseAfterBase.mp3",
    "CantLetGo.mp3",    
    "Jumper.mp3",       
    "TimeMachine.mp3",  
    "Cycles.mp3",       
    "xStep.mp3",        
    "Clutterfunk.mp3",        
    "TheoryOfEverything.mp3", 
    "Electroman.mp3",         
    "Clubstep.mp3",           
    "Active.mp3",             
    "Electrodynamix.mp3",     
    "HexagonForce.mp3",       
    "BlastProcessing.mp3",    
    "TheoryOfEverything2.mp3",
    "CosmicDreamer.mp3",      
    "SkyFortress.ogg",        
    "SoundOfInfinity.mp3",    
    "Rupture.mp3",            
    "Stalemate.mp3",          
    "GloriousMorning.ogg",     
    "ChaozFantasy.ogg",
    "Phazd.ogg"
};

#define SONGS sizeof(audioFiles) / sizeof(audioFiles[0]) - 1 // made this as easy to replace as possible

static std::string s_persistentPath;
char * (*LevelTools_getAudioFilename)(int);
char * LevelTools_getAudioFilename_H(int songID) {
  if(songID > SONGS) {
    auto newSongID = songID + 1;
    s_persistentPath = "/storage/emulated/0/Music/saved_songs/" + to_string(newSongID) + ".mp3";
    return (char*)s_persistentPath.c_str();
  }
  else if (songID < 0) return "BackOnTrack.mp3";
  else return audioFiles[songID];
}

class LevelTools {

};

struct SongCache {
    float bpm;
    float duration;
    bool ready;
    SongCache(float b = 0.0f, float d = 0.0f, bool r = false) 
        : bpm(b), duration(d), ready(r) {}
};
std::unordered_map<std::string, SongCache> g_songCache;
std::mutex g_cacheMutex;

CustomSongData getSongData(const char* filePath, FMOD::System* system) {
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = system->createSound(filePath, FMOD_CREATESAMPLE | FMOD_ACCURATETIME, nullptr, &sound);
    if (result != FMOD_OK) return { 0.0f, 0.0f };

    unsigned int lengthPCM, lengthMS;
    int channels;
    float freq;
    
    sound->getLength(&lengthPCM, FMOD_TIMEUNIT_PCM);
    sound->getLength(&lengthMS, FMOD_TIMEUNIT_MS);
    sound->getFormat(nullptr, nullptr, &channels, nullptr);
    sound->getDefaults(&freq, nullptr);
    void *ptr1, *ptr2;
    unsigned int len1, len2;
    sound->lock(0, lengthPCM * channels * sizeof(short), &ptr1, &ptr2, &len1, &len2);
    
    short* pcmData = (short*)ptr1;
    unsigned int totalSamples = len1 / (sizeof(short) * channels);
    std::vector<float> monoBuffer;
    monoBuffer.reserve(totalSamples);

    for (unsigned int i = 0; i < totalSamples * channels; i += channels) {
        float s = 0;
        for (int c = 0; c < channels; ++c) s += pcmData[i + c];
        monoBuffer.push_back((s / (float)channels) / 32768.0f);
    }

    breakfastquay::MiniBPM minibpm(freq);
    minibpm.process(monoBuffer.data(), monoBuffer.size());
    float bpm = (float)minibpm.estimateTempo();

    sound->unlock(ptr1, ptr2, len1, len2);
    sound->release();

    return { bpm, (float)lengthMS / 1000.0f };
}

std::string generateAudioString(float bpm, float duration, float intensity = 0.9f) {
    std::string result = "";
    float beatInterval = 60.0f / bpm;
    for (float time = 0.0f; time < duration; time += beatInterval) {
        result += to_string(time) + ";" + to_string(intensity) + ";";
    }
    return result;
}

struct RawStr { char* data; size_t size; size_t cap; };

void* (*LevelTools_getAudioString)(void* self, int ID);
void* LevelTools_getAudioString_H(void* self, int ID) {
    LevelTools_getAudioString(self, (ID > SONGS) ? 5 : ID);
    if (ID > SONGS) {
        std::string path = LevelTools_getAudioFilename_H(ID);
        std::string generated;
        {
            std::lock_guard<std::mutex> lock(g_cacheMutex);
            auto it = g_songCache.find(path);
            if (it != g_songCache.end() && it->second.ready) {
                generated = generateAudioString(it->second.bpm, it->second.duration, 0.9f);
            } else {
                if (it == g_songCache.end()) {
                    g_songCache[path] = SongCache(0.0f, 0.0f, false);
                    std::thread([path]() {
                        CustomSongData data = getSongData(path.c_str(), fmodSystem);
                        std::lock_guard<std::mutex> innerLock(g_cacheMutex);
                        g_songCache[path] = SongCache(data.bpm, data.duration, true);
                    }).detach();
                }
                generated = "0;0.9;";
            }
        }
        RawStr* strObj = reinterpret_cast<RawStr*>(self);
        uint32_t actualCap = strObj->cap & 0x3FFFFFFF;
        if (strObj->data && actualCap >= generated.length()) {
            memcpy(strObj->data, generated.c_str(), generated.length());
            strObj->data[generated.length()] = '\0';
            strObj->size = generated.length();
        } else {
            uint32_t fitLen = (actualCap > 0) ? actualCap - 1 : 0;
            if (fitLen > 0 && strObj->data) {
                memcpy(strObj->data, generated.c_str(), fitLen);
                strObj->data[fitLen] = '\0';
                strObj->size = fitLen;
            }
        }
    }
    return self;
}






int (*LevelTools_artistForAudio)(int);
int LevelTools_artistForAudio_H(int ID) {
  int aToS[36] = {2, 3, 0, 4, 0, 0, 0, 1, 1, 0, 0, 1, 5, 1, 5, 5, 1};
  if (ID == -1) return 2;
  if (ID > 36) return 0;
  return aToS[ID];
}

char * (*LevelTools_nameForArtist)(int);
char * LevelTools_nameForArtist_H(int ID) {
  switch (ID) {
    case 0: return "DJVI";
    case 1: return "Waterflame";
    case 2: return "OcularNebula";
    case 3: return "ForeverBound";
    case 4: return "Step";
    case 5: return "DJ-Nate";
    case 6: return "F-777";
    case 7: return "TMM43";
    case 8: return "Kid2Will";
    case 9: return "ParagonX9";
    case 10: return "MadhouseDUDE";
    case 11: return "EnV";
    case 12: return "KzX";
    case 13: return "Bossfight";
    case 14: return "Dimrain47";
    case 15: return "Helix";
    case 16: return "Solkrieg";
    case 17: return "Jumper";
    default: return "Unknown";
  }
}
cocos2d::CCArray* getPlayLayerHazards(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(cocos2d::CCArray*, playLayer, 0x192);
}

void (*PlayLayer_destroyPlayer)(PlayLayer*);
void PlayLayer_destroyPlayer_H(PlayLayer* self) {
  if (!noclip) {
    CCDirector* director = CCDirector::sharedDirector();
    CCScheduler* scheduler = director->getScheduler();
    if(noRespawn) scheduler->setTimeScale(99.0f);
    if(noRespawn) noRespawnReset = true;
    PlayLayer_destroyPlayer(self);
    auto audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    if(MEMBER_BY_OFFSET(int, self, 0x29d) && pmh) audioEngine->pauseBackgroundMusic(); // check for practice mode
  }
  else if(noclip) {
    if (lastDeadFrame < frameCount - 1) {
            deaths++;
            if(ExtraLayer::m_flash) self->triggerCustomPulse(0.5f);
        }
        lastDeadFrame = frameCount;
    MEMBER_BY_OFFSET(cocos2d::CCArray*, self, 0x18c)->removeAllObjects();
  } 

  return;
}

std::string version = "0";

VersionRequest* VersionRequest::sharedRequest() {
  VersionRequest* request = new VersionRequest();
  return request;
}

void MenuLayer::checkVersionIsOutdated() {
    MenuLayer::s_hasVersionData = false;
    VersionRequest::sharedRequest()->fetchVersion();
    this->schedule(schedule_selector(MenuLayer::versionCheckPoll), 0.1f);
}

void MenuLayer::versionCheckPoll(float dt) {
    if (MenuLayer::s_hasVersionData) {
        this->unschedule(schedule_selector(MenuLayer::versionCheckPoll));
        
        std::string fetchedVersion = std::string(MenuLayer::s_newVersion.load());
        if (fetchedVersion != currentSpecificVersion) {
        version = fetchedVersion;
            this->showUpdateAlert(0);
        }
        MenuLayer::s_hasVersionData = false;
    }
CCFadeOut* fadeout = CCFadeOut::create(2.f);
CCCallFunc* callback = CCCallFunc::create(updateLabel, callfunc_selector(CCNode::removeFromParentAndCleanup));
auto sequence = CCSequence::create(fadeout, callback, NULL);

updateLabel->runAction(sequence);
}


void MenuLayer::showUpdateAlert(float dt) {
  if (!this->isRunning()) return;

    auto alert = FLAlertLayer::create(
        this,
        "New update found!",
        CCString::createWithFormat("Your version of VioletPS is <cr>outdated</c>, download the new version to obtain the new features! \n\n<cy>Your version:</c> %s\n<cp>New version:</c> %s", currentSpecificVersion.c_str(), version.c_str())->getCString(),
        "OK", "Download", 300.f
    );
    alert->setTag(1001);
    alert->show();
    ExtraLayer::uploadNotAllowed_ = true;
}

void (*MenuLayer_init)(MenuLayer*);
void MenuLayer_init_H(MenuLayer* self) {
ExtraLayer::onLoadSettings();
if(playerControl) {
onToggleControl(true);
auto pivot = PivotMenuLayer::create();
self->addChild(pivot, -1); 
} else onToggleControl(false);
MenuLayer_init(self);
musicVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("musicVolume", 1.0f);
auto gman = GameManager::sharedState();
// if(MEMBER_BY_OFFSET(int, gman, 0x171)) CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(musicVolume);
auto win_size = CCDirector::sharedDirector()->getWinSize();
if(!checked) {
updateLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Checking for updates")->getCString(), 
            "goldFont-hd.fnt"
        );

  updateLabel->setPosition({win_size.width / 2, 100});
  updateLabel->setScale(0.5f);
  self->addChild(updateLabel);
if(!warnedOutdated) self->checkVersionIsOutdated();
}

if(iconHack) onToggleIcon(true);
else onToggleIcon(false);
checked = true;
MenuLayer::sharedLayer = self;
auto buttonMenu = CCMenu::create();
auto button = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, self, menu_selector(MenuLayer::keyBackClicked));
menuBtn->setPosition({-25,-25});
buttonMenu->setPosition({win_size.width, win_size.height});
buttonMenu->addChild(menuBtn);
iconHack = CCUserDefault::sharedUserDefault()->getBoolForKey("iconHack", false);
// i give up fuck this shit i wanna di
/*
auto button2 = CCSprite::create("GJ_violetMod_001.png");
CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, self, menu_selector(MenuLayer::onOpenMenu));
CCMenu* bottomMenu;
if(!playerControl) bottomMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(self->getChildrenCount() - 3));
else bottomMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(self->getChildrenCount() - 2));
CCArray* menuChildren = bottomMenu->getChildren();
bottomMenu->addChild(menuBtn2); 
if (menuChildren && menuChildren->count() > 1) {
    menuChildren->removeObject(menuBtn2, false);
    menuChildren->insertObject(menuBtn2, 0);
    }
bottomMenu->alignItemsHorizontallyWithPadding(5.0f);
if(exitButton) self->addChild(buttonMenu);
*/
}

void (*PauseLayer_customSetup)(CCLayer* self);
void PauseLayer_customSetup_H(CCLayer* self) {
PauseLayer_customSetup(self);
HidePauseLayer::m_pauseLayer = self;
CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
changeAllChannelsPitch(1.0f);
ExtraLayer::saveSettingsToFile();
auto win_size = CCDirector::sharedDirector()->getWinSize();
auto buttonMenu = CCMenu::create();
auto infobutton = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(infobutton, infobutton, self, menu_selector(ToggleHack::showAtts));
menuBtn->setPosition({-20,-20});
menuBtn->setScale(1.f);
menuBtn->_setZOrder(100);
buttonMenu->setPosition({60, win_size.height - 30});
if(extrainfo) {
buttonMenu->addChild(menuBtn);
self->addChild(buttonMenu);
}

auto buttonMenuu = CCMenu::create();
auto buttoon = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
CCMenuItemSpriteExtra* menuu = CCMenuItemSpriteExtra::create(buttoon, buttoon, self, menu_selector(PauseLayer::toggleVisibility));
menuu->setPosition({-20,-20});
menuu->setScale(1.f);
menuu->_setZOrder(100);
buttonMenuu->setPosition({60, win_size.height - 30});
buttonMenuu->addChild(menuu);
if(hidePauseMenu) self->addChild(buttonMenuu);

auto buttonMenu2 = CCMenu::create();
auto button2 = CCSprite::create("GJ_violetMod_001.png");
CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, self, menu_selector(PauseLayer::onOpenMenu));
menuBtn2->setPosition({-20,-20});
menuBtn2->setScale(1.f);
menuBtn2->_setZOrder(100);
buttonMenu2->setPosition({win_size.width - 60, win_size.height - 30});
buttonMenu2->addChild(menuBtn2);
self->addChild(buttonMenu2);
}

void (*PlayLayer_levelComplete)(CCLayer*);
void PlayLayer_levelComplete_H(CCLayer* self) {
  CCDirector* director = CCDirector::sharedDirector();
  CCScheduler* scheduler = director->getScheduler();
  scheduler->setTimeScale(1.f);
  if (safeMode) callFunctionFromSymbol<void (*)(CCLayer*)>("_ZN9PlayLayer6onQuitEv")(self);
  else PlayLayer_levelComplete(self);
}

bool getPlayLayerPractice(PlayLayer* playLayer);
bool getPlayLayerPractice();
void setPlayLayerPractice(PlayLayer* playLayer, bool isPractice);
void setPlayLayerPractice(bool isPractice);
cocos2d::CCArray* getPlayLayerCheckpoints(PlayLayer* playLayer);
cocos2d::CCArray* getPlayLayerCheckpoints();
cocos2d::CCPoint getCheckpointPosition(CCNode* checkpoint);
cocos2d::CCPoint getStartPos(PlayLayer* playLayer);
cocos2d::CCPoint getStartPos();

cocos2d::CCPoint getStartPos(PlayLayer* playLayer) {
    return from<CCPoint>(playLayer, 0x268);
}

cocos2d::CCPoint getCheckpointPosition(CCNode* checkpoint) {
    return from<CCPoint>(checkpoint, 0x12c);
}


void (*PlayLayer_resetLevel)(PlayLayer*);
void PlayLayer_resetLevel_H(PlayLayer* self) {
musicVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("musicVolume", 1.0f);
auto gman = GameManager::sharedState();
onToggleFlip(instantFlip);
// if(MEMBER_BY_OFFSET(int, gman, 0x171)) CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(musicVolume);
CCDirector* director = CCDirector::sharedDirector();
CCScheduler* scheduler = director->getScheduler();
  if(noRespawnReset) {
  scheduler->setTimeScale(1.0f);
  noRespawnReset = false;
}
if(MEMBER_BY_OFFSET(bool, self, 0x29d) == false) CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
  PlayLayer_resetLevel(self);
  clickTimestamps.clear();
  deaths = 0;
  clicks = 0;
  if(noclip || speedhack) safeMode = true;
  else safeMode = false;
  PlayerObject* player = MEMBER_BY_OFFSET(PlayerObject*, self, 0x274);
float seconds = player->getPosition().x / 311.58;
  if(pmh && MEMBER_BY_OFFSET(int, self, 0x29d)) CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicTime(seconds);
  return;
}
/* cocos2d::CCLabelBMFont* percentage;
  percentage = nullptr;
void UILayer::createPercentageLabel() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto percentageLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
    percentageLabel->setPosition(ccp(winSize.width / 2, winSize.height - 10));
    percentageLabel->setScale(0.5f);
    percentage = percentageLabel;
    addChild(percentageLabel, 10000);
} */

void (*PlayLayer_resume)(CCLayer*);
void PlayLayer_resume_H(CCLayer* self) {
  PlayLayer_resume(self);
  if(noclip || speedhack) safeMode = true;
  extraLayerCreated = false;
  elc = false;
  PlayerObject* player = MEMBER_BY_OFFSET(PlayerObject*, self, 0x274);
float seconds = player->getPosition().x / 311.58;
  if(pmh && MEMBER_BY_OFFSET(int, self, 0x29d)) CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicTime(seconds);
    }

cocos2d::CCLabelBMFont* noclipLabel;
cocos2d::CCLabelBMFont* percentageLabel;
cocos2d::CCLabelBMFont* goldenPercentageLabel;
void (*UILayer_init)(CCLayer*);
void UILayer_init_H(CCLayer* self) {
  UILayer_init(self);
  auto win_size = CCDirector::sharedDirector()->getWinSize();
  if(fps_label) labelFPSDisplay = (char*)"0 FPS";
  else labelFPSDisplay = (char*)"";
  if(deaths_label) labelDeathsDisplay = (char*)"0 deaths";
  else labelDeathsDisplay = (char*)"";
  if(time_label) labelTimeDisplay = (char*)"00:00.000";
  else labelTimeDisplay = (char*)"";
  if(custom_label) labelCustomDisplay = (char*)"Custom";
  else labelCustomDisplay = (char*)"";
  if(speedhack_label) labelSpeedhackDisplay = (char*)"Speedhack";
  else labelSpeedhackDisplay = (char*)"";
  if(clicks_label) labelClicksDisplay = (char*)"0 clicks";
  else labelClicksDisplay = (char*)"";
  if(cps_label) labelCPSDisplay = (char*)"0 CPS";
  else labelCPSDisplay = (char*)"";

  labelInfo = CCLabelBMFont::create(
            CCString::createWithFormat("%s\n%s\n%s\n%s\n%s\n%s\n%s", labelFPSDisplay.c_str(), labelDeathsDisplay.c_str(), labelTimeDisplay.c_str(), labelCustomDisplay.c_str(), labelSpeedhackDisplay.c_str(), labelClicksDisplay.c_str(), labelCPSDisplay.c_str())->getCString(), 
            "chatFont.fnt"
        );
        labelInfo->setScale(0.5f);
        labelInfo->setAnchorPoint({0.f, 0.5f});
        labelInfo->setPosition(10, win_size.height - 60);
        labelInfo->setOpacity(127);
        self->addChild(labelInfo, 10000);
  noclipLabel = CCLabelBMFont::create(
            CCString::createWithFormat("Safe Mode")->getCString(), 
            "chatFont.fnt"
        );
        noclipLabel->setScale(0.5f);
        noclipLabel->setAnchorPoint({0.f, 0.5f});
        noclipLabel->setPosition(10,10);
        noclipLabel->setColor(ccc3(255, 0, 0));
        self->addChild(noclipLabel, 10000);

        percentageLabel = CCLabelBMFont::create(
            CCString::createWithFormat("0%")->getCString(), 
            "bigFont.fnt"
        );
        percentageLabel->setScale(0.5f);
        percentageLabel->setAnchorPoint({0.f, 0.5f});
        self->addChild(percentageLabel, 10000);

        goldenPercentageLabel = CCLabelBMFont::create(
            CCString::createWithFormat("0%")->getCString(), 
            "goldFont.fnt"
        );
        goldenPercentageLabel->setScale(0.5f);
        goldenPercentageLabel->setAnchorPoint({0.f, 0.5f});
        self->addChild(goldenPercentageLabel, 10000);

        if(!safeMode) noclipLabel->setVisible(false);
        else noclipLabel->setVisible(true);

        if(noParticles) {
          percentageLabel->setVisible(true);
          percentageLabel->setString("0%");
          GameManager* state = GameManager::sharedState();
  if(MEMBER_BY_OFFSET(int, state, 0x1a5)) percentageLabel->setPosition(win_size.width - 190, win_size.height - 10);
  else percentageLabel->setPosition(win_size.width / 2, win_size.height - 10);
          goldenPercentageLabel->setVisible(false);
        } else {
          percentageLabel->setVisible(false);
          goldenPercentageLabel->setVisible(false);
        }
}

bool (*GameManager_isColorUnlocked)(GameManager*, int, bool);
bool GameManager_isColorUnlocked_H(GameManager* self, int id, bool idk) {
  return true;
}

bool (*GameManager_isIconUnlocked)(GameManager*, int, int);
bool GameManager_isIconUnlocked_H(GameManager* self, int id, int id2) {
  return true;
}


void PlayLayer::draw() {
  if(hitbox) {
    CCLayer::draw();
  CCArray* children = this->getChildren();
for (unsigned int i = 0; i < children->count(); ++i) {
    CCSprite* sprite = dynamic_cast<CCSprite*>(children->objectAtIndex(i));
if(sprite != nullptr) {
CCRect rect = sprite->boundingBox();
CCLayer::draw();
CCPoint lowerLeft = rect.origin;
CCPoint lowerRight = ccp(rect.origin.x + rect.size.width, rect.origin.y);
CCPoint upperRight = ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
CCPoint upperLeft = ccp(rect.origin.x, rect.origin.y + rect.size.height);

ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

ccDrawRect(lowerLeft, upperRight);
}
}
  }
}

FPSCounter* FPSCounter::sharedCounter()
{
    if (!s_sharedCounter)
    {
        s_sharedCounter = new FPSCounter();
    }
    return s_sharedCounter;
}

void FPSCounter::update(float dt) {
        return;
      }

    int frames = 0;
  float accumulatedTime = 0.f;
  float currentFPS = 0.f;

void toggleGoldenPercentage(bool activate) {
  if(!activate) {
    percentageLabel->setVisible(true);
    goldenPercentageLabel->setVisible(false);
  } else {
    percentageLabel->setVisible(false);
    goldenPercentageLabel->setVisible(true);
  }
}
void (*PlayLayer_onUpdate)(PlayLayer* self, float dt);
void PlayLayer_onUpdate_H(PlayLayer* self, float dt) { // typo
PlayLayer_onUpdate(self, dt);
auto m_player = MEMBER_BY_OFFSET(PlayerObject*, self, 0x274);
CCLog("Y Acelleration: %.2f \n Gravity: %.2f \n Y Start: %.2f", MEMBER_BY_OFFSET(double, m_player, 0x3c0), MEMBER_BY_OFFSET(double, m_player, 0x3a0), MEMBER_BY_OFFSET(double, m_player, 0x398));
if(!gjlvl || MEMBER_BY_OFFSET(float, self, 0x1dc) == 0 || !MEMBER_BY_OFFSET(PlayerObject*, self, 0x274)) return;
auto normalPercentage = MEMBER_BY_OFFSET(int, gjlvl, 0x170);
CCDirector* director = CCDirector::sharedDirector();
CCScheduler* scheduler = director->getScheduler();
if(speedhack) {
  scheduler->setTimeScale(ExtraLayer::m_speedhack);
  if(ExtraLayer::m_speedhackMusic) changeAllChannelsPitch(ExtraLayer::m_speedhack);
}
auto win_size = CCDirector::sharedDirector()->getWinSize();
frameCount++;
cocos2d::CCUserDefault* ccdefault = cocos2d::CCUserDefault::sharedUserDefault();
if(noclipLabel != nullptr) {
    if(safeMode) noclipLabel->setVisible(true);
    else noclipLabel->setVisible(false);
  }
  atts = MEMBER_BY_OFFSET(int, self, 0x2d8);
  jumps = MEMBER_BY_OFFSET(int, self, 0x2dc);
  if(MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x1e4)) {
  if(hideatts) MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x1e4)->setVisible(false);
  else MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x1e4)->setVisible(true);
  }
  extraLayerCreated = false;
elc = false;
  auto sharedCounter = FPSCounter::sharedCounter();
 accumulatedTime += dt;
       frames++;
        if (accumulatedTime >= 0.25f)
        {
            currentFPS = (float)frames / accumulatedTime;
            if(fps_label) labelFPSDisplay = formatWithChar("%i FPS\n", (int)currentFPS);
            frames = 0;
            accumulatedTime = 0.0f;
        }
if(ccdefault->getBoolForKey("noParticles", false) || noParticles) {
  PlayerObject* player = MEMBER_BY_OFFSET(PlayerObject*, self, 0x274);
    float percent = (player->getPosition().x / MEMBER_BY_OFFSET(float, self, 0x1dc)) * 100.0; // destroyplayer
    if(percent > 100) percent = 100;
    if(percent < 0) percent = 0;
    percent = floorf(percent);
  std::string str = to_string((int)percent) + "%";
if(percentageLabel) percentageLabel->setString(str.c_str());
  if(goldenPercentageLabel) goldenPercentageLabel->setString(str.c_str());
  if((int)percent > normalPercentage && MEMBER_BY_OFFSET(int, self, 0x29d) == 0) toggleGoldenPercentage(true);
  else toggleGoldenPercentage(false);
  GameManager* state = GameManager::sharedState();
        if(MEMBER_BY_OFFSET(int, state, 0x1a5)) {
        CCSprite* percentageBar = MEMBER_BY_OFFSET(CCSprite*, self, 0x214);
        CCPoint barPos = percentageBar->getPosition();
        CCSize barSize = percentageBar->getContentSize();
        CCPoint anchor = percentageBar->getAnchorPoint();
        float rightEdgeX = barPos.x + (barSize.width * (1.0f - anchor.x));
        percentageLabel->setAnchorPoint(ccp(0, 0.5f)); 

          percentageLabel->setPosition(ccp(rightEdgeX + 5, barPos.y));
          goldenPercentageLabel->setPosition(ccp(rightEdgeX + 5, barPos.y)); 
        }
        else {
          percentageLabel->setPosition(win_size.width / 2, win_size.height - 10);
          goldenPercentageLabel->setPosition(win_size.width / 2, win_size.height - 10);
        }
} else {
  percentageLabel->setVisible(false);
  goldenPercentageLabel->setVisible(false);
}
if(deaths_label) labelDeathsDisplay = formatWithChar("%i deaths\n", deaths);
int totalSeconds = MEMBER_BY_OFFSET(float, self, 0x2e4);
int h = static_cast<int>(totalSeconds) / 3600;
int m = (static_cast<int>(totalSeconds) % 3600) / 60;
int s = static_cast<int>(totalSeconds) % 60;
if(time_label) labelTimeDisplay = formatWithChar("%02d:%02d:%02d\n", h, m, s);
if(custom_label) labelCustomDisplay = formatWithChar("%s\n", ExtraLayer::m_customLabelValue.c_str());
if(speedhack_label && speedhack) labelSpeedhackDisplay = formatWithChar("Speedhack: %.2fx\n", ExtraLayer::m_speedhack);
else if(speedhack_label && !speedhack) labelSpeedhackDisplay = (char*)"No Speedhack\n";
if(clicks_label) labelClicksDisplay = formatWithChar("%i clicks\n", clicks);
while (!clickTimestamps.empty() && clickTimestamps.front() < (totalSeconds - 1.0f)) {
    clickTimestamps.erase(clickTimestamps.begin());
}
float cps = static_cast<float>(clickTimestamps.size());
if(cps_label) labelCPSDisplay = formatWithChar("%i CPS\n", static_cast<int>(cps));
labelInfo->setString(CCString::createWithFormat("%s%s%s%s%s%s%s", labelFPSDisplay.c_str(), labelDeathsDisplay.c_str(), labelTimeDisplay.c_str(), labelCustomDisplay.c_str(), labelSpeedhackDisplay.c_str(), labelClicksDisplay.c_str(), labelCPSDisplay.c_str())->getCString());
}

void fuck() {
  return;
}

CCMenu* createSpriteButtonWithFunction(CCSprite* sprite, CCPoint position, cocos2d::SEL_MenuHandler selector, CCLayer* layer) {
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(sprite, sprite, layer, selector);
auto buttonMenu = CCMenu::create(menuBtn, NULL);
buttonMenu->setAnchorPoint(CCPointZero);
buttonMenu->setPosition(CCPointZero);
buttonMenu->setPosition(position);
return buttonMenu;
}

CCMenu* createSpriteFrameNameButtonWithFunctionAndSpriteName(const char* spriteName, CCPoint position, cocos2d::SEL_MenuHandler selector, CCLayer* layer) {
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(spriteName), CCSprite::createWithSpriteFrameName(spriteName), layer, selector);
auto buttonMenu = CCMenu::create(menuBtn, NULL);
buttonMenu->setAnchorPoint(CCPointZero);
buttonMenu->setPosition(CCPointZero);
buttonMenu->setPosition(position);
return buttonMenu;
}

void LevelSettingsLayer::onCustomSongs(CCObject* pSender) {
  auto btn = static_cast<CCMenuItemSpriteExtra*>(pSender);
  auto lso = reinterpret_cast<LevelSettingsObject*>(btn->getUserData());
  auto song = CustomSongLayer::create(this, lso);
  this->onClose();
  ExtraLayer::m_lel->addChild(song);
}

int (*LevelSettingsLayer_init)(CCLayer* self, LevelSettingsObject* obj);
int LevelSettingsLayer_init_H(CCLayer* self, LevelSettingsObject* obj) {
LevelSettingsLayer_init(self, obj);
CCLayer* container = MEMBER_BY_OFFSET(CCLayer*, self, 0x1c8);
CCMenu* menuContainer = MEMBER_BY_OFFSET(CCMenu*, self, 0x1b0);
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(CCSprite::create("GJ_musicOnBtn_001.png"), CCSprite::create("GJ_musicOnBtn_001.png"), self, menu_selector(LevelSettingsLayer::onCustomSongs));
CCPoint pos = CCPointMake((CCDirector::sharedDirector()->getWinSize().width / 2) - menuBtn->getContentSize().width - 5, (CCDirector::sharedDirector()->getWinSize().height / 2) - menuBtn->getContentSize().height - 5);
menuBtn->setUserData(obj); 
// menuBtn->setPosition(pos);
menuContainer->addChild(menuBtn, 100000); 
return 1;
}

void (*CreatorLayer_init)(CCLayer*);
void CreatorLayer_init_H(CCLayer* self) {
  CreatorLayer_init(self);
  if(hideVault) return;
  CCArray* children = self->getChildren();

    std::vector<CCNode*> labelsToRemove;
    CCObject* child;

    CCARRAY_FOREACH(children, child)
    {
        CCNode* node = static_cast<CCNode*>(child);
        CCSprite* label = dynamic_cast<CCSprite*>(node);
        if (label != nullptr)
        {
            labelsToRemove.push_back(node);
        }
    }
    for (CCNode* label : labelsToRemove)
    {
        self->removeChild(label, true);
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    backgroundSprite->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    self->addChild(backgroundSprite, -2);
    float scaleX = (winSize.width + 10.0f) / backgroundSprite->getContentSize().width;
    float scaleY = (winSize.height + 10.0f) / backgroundSprite->getContentSize().height;
    backgroundSprite->setScaleX(scaleX);
    backgroundSprite->setScaleY(scaleY);
    backgroundSprite->setOpacity(255);
    backgroundSprite->setColor(ccc3(87, 87, 255));


auto buttonMenu = CCMenu::create();
auto win_size = CCDirector::sharedDirector()->getWinSize();
bool closed = CoderLayer::checkIfTheDoorInCreateLayerInitIsClosedOrNot();
if(!closed) { 
  auto button = CCSprite::create("secretDoor_open.png");
  button->setPosition(CCPointMake(win_size.width - 20, 20));
  auto buttonMenu = createSpriteButtonWithFunction(button, button->getPosition(), menu_selector(CreatorLayer::onSecret), self);
  buttonMenu->setScale(0.5f);
self->addChild(buttonMenu);
}
else {
  auto button = CCSprite::create("secretDoor_closed.png");
  button->setPosition(CCPointMake(win_size.width - 20, 20));
  auto buttonMenu = createSpriteButtonWithFunction(button, button->getPosition(), menu_selector(CreatorLayer::onSecret), self);
  buttonMenu->setScale(0.5f);
self->addChild(buttonMenu);
}
}

void (*PlayLayer_onQuit)(CCLayer*);
void PlayLayer_onQuit_H(CCLayer* self) {
PlayLayer_onQuit(self);
added = false;
extraLayerCreated = false;
elc = false;
deaths = 0;
clicks = 0;
clickTimestamps.clear();
CCDirector::sharedDirector()->setDisplayStats(false);
}

void (*CommentCell_loadFromComment)(CCLayer*, GJComment*);
void CommentCell_loadFromComment_H(CCLayer* self, GJComment* comment) {
  CommentCell_loadFromComment(self, comment);
  auto com = from<GJComment*>(self, 0x400);
  int commentID = MEMBER_BY_OFFSET(int, com, 0x130);
  CCLabelBMFont* idLabel = CCLabelBMFont::create( CCString::createWithFormat("ID: %i", commentID)->getCString(), 
            "chatFont.fnt"
        );
        self->addChild(idLabel);
}

char* (*LoadingLayer_getLoadingString)(void*);
char* LoadingLayer_getLoadingString_H(void*) {

      static const char* randArr[] = {
      "i loove gd cologne",
      "Made by our idiotic developer, nano",
      "Previously known as PokemonGDPS.",
      "are you ready to die?",
      "Loading everything we can...",
      "reading this gives bad luck...",
      "1 star low quality gdps",
      "Made on April 30th 2025.",
      "The Master Pack makes you question reality.",
      "fun fact: i use this menu loop because of evogdps",
      "PACKED with new songs!",
      "\"heavily modded gdps\" -System32",
      "I hope nobody reads this...",
      "If you ever feel like you're at the bottom of the sea, well, think about SeaGDPS.",
      "I'm seeing a bag, somewhere...",
      "Shenanigans are happening behind the screen.",
      "you AREN'T supposed to see this message.",
      "There's a vaultkeeper that gives you rewards for solving codes, i do not know what that means.",
      "i love my life",
      "You MUST be bored to play this GDPS.",
      "hey guys welcome to object fool",
      "Loading the",
      "59 crashes and 12 abandoned projects, welcome to VioletPS.",
      "Loading until i find the offsets...",
      "Waiting for planets to align",
      "Loading until the next solar eclipse",
      "Type \"funny codes\" on the vaultkeeper and you will get a reward!",
      "Don't you hear construction sounds?",
      "Invading 4 undiscovered civilizations",
      "Getting 82\% on allegiance",
      "Getting GDPS of the Month...",
      "Sorry guys, i dropped my purple paint onto the coins, hope it doesn't bother... .. .",
      "I need the fire extinguisher for the chicken!",
      "A Fantasy of Chaoz!",
      "Life is bad if you only focus on the deaths you have.",
      "Custom Songs!",
      "RobTop is love, RobTop is life.",
      "Roses are red, violets are blue, this GDPS is great. Fuck, it doesn't rhyme.",
      "You have been dinde"
};
    return const_cast<char*>(randArr[random_array_index(randArr)]);
}

void selectSong(CCLayer* self, int currentSong) {
  return;
}

void (*LevelSettingsLayer_audioNext)(CCLayer*);
void (*LevelSettingsLayer_audioPrevious)(CCLayer*);

void LevelSettingsLayer_audioNext_H(CCLayer* self) {
  auto song = MEMBER_BY_OFFSET(CCLayer*, self, 0x1dc);
  int currentSong = MEMBER_BY_OFFSET(int, song, 0x138);
  if(currentSong < 18) selectSong(self, currentSong + 1);
return;
}

void LevelSettingsLayer_audioPrevious_H(CCLayer* self) {
  auto song = MEMBER_BY_OFFSET(CCLayer*, self, 0x1dc);
  int currentSong = MEMBER_BY_OFFSET(int, song, 0x138);
  if(currentSong > -1) selectSong(self, currentSong - 1);
return;
}

int (*PlayLayer_init)(CCLayer*, GJGameLevel*);
int PlayLayer_init_H(CCLayer* self, GJGameLevel* lvl) {
  PlayLayer_init(self, lvl);
  HidePauseLayer::m_playLayer = self;
  if(hideatts) MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x1e4)->setVisible(false);
  else MEMBER_BY_OFFSET(CCLabelBMFont*, self, 0x1e4)->setVisible(true);
  gjlvl = lvl;
  if(!percentageLabel->getParent() && percentageLabel != nullptr) MEMBER_BY_OFFSET(CCLayer*, self, 0x270)->addChild(percentageLabel, 10000);
  if(!goldenPercentageLabel->getParent() && goldenPercentageLabel != nullptr) MEMBER_BY_OFFSET(CCLayer*, self, 0x270)->addChild(goldenPercentageLabel, 10000);
  return 1;
}

void (*PlayLayer_togglePracticeMode)(CCLayer*, bool idk);
void PlayLayer_togglePracticeMode_H(CCLayer* self, bool idk) {
  if((gjlvl == nullptr && idk) || (!pmh && idk)) {
        PlayLayer_togglePracticeMode(self, true);
        return;
    }
  if(!idk) {
        PlayLayer_togglePracticeMode(self, false);
        return;
    }
PlayLayer_togglePracticeMode(self, true);
SimpleAudioEngine* audioengine;
int audioTrack = MEMBER_BY_OFFSET(int, gjlvl, 0x148);
char* audiotitlestd = LevelTools_getAudioFilename_H(audioTrack);
CocosDenshion::SimpleAudioEngine* sharedEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
sharedEngine->playBackgroundMusic(audiotitlestd,true); 
return;
}

void MenuLayer::onOpenMenu() {
  this->setTouchEnabled(false);
    extra = ExtraLayer::create(this);
    this->addChild(extra, 1000);
}

void EditLevelLayer::onOpenMenu() {
  this->setTouchEnabled(false);
    extra = ExtraLayer::create(this);
    this->addChild(extra, 1000);
}

void (*OptionsLayer_onRate)(void);
void OptionsLayer_onRate_H(void) {
FLAlertLayer::create(
            nullptr,
            "Info",
            CCString::createWithFormat("Nothing here..\n\n :)")->getCString(),
            "OK?",
            nullptr,
            300.f
        )->show();
}

void (*InfoLayer_init)(CCLayer* self, GJGameLevel* lvl);
void InfoLayer_init_H(CCLayer* self, GJGameLevel* lvl) {
  InfoLayer_init(self, lvl);
  auto buttonMenu = CCMenu::create();
auto win_size = CCDirector::sharedDirector()->getWinSize();
auto button = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(button, button, self, menu_selector(ToggleHack::showMoreLevelInfo));
menuBtn->setPosition({-20,-20});
menuBtn->setScale(1.f);
menuBtn->_setZOrder(100);
buttonMenu->setPosition({win_size.width - 10, win_size.height - 10});
buttonMenu->addChild(menuBtn);
self->addChild(buttonMenu);

}

void EditorUI::deleteAll() {
    CCArray* selected = MEMBER_BY_OFFSET(CCArray*, this, 0x180);
    LevelEditorLayer* editLayer = MEMBER_BY_OFFSET(LevelEditorLayer*, this, 0x1d0);
    CCArray* sections = MEMBER_BY_OFFSET(CCArray*, editLayer, 0x144);
    if(editLayer == nullptr || sections == nullptr || selected == nullptr) {
        return;
    }
    GameObject* definingObject = nullptr;
    if (selected->count() > 0) {
        definingObject = static_cast<GameObject*>(selected->objectAtIndex(0));
    } else {
       definingObject = MEMBER_BY_OFFSET(GameObject*, this, 0x254);
    }
    if (definingObject == nullptr) {
        return;
    }
    auto* selectedObj1Member = MEMBER_BY_OFFSET(void*, definingObject, 0x32c);
    if (selectedObj1Member == nullptr) {
        return;
    }
    std::vector<GameObject*> objectsToDelete;
    for (int i = 0; i < sections->count(); i++) {
        auto section = static_cast<CCArray*>(sections->objectAtIndex(i));
        if (section == nullptr) continue; 
        for (int j = 0; j < section->count(); j++) {
            auto currObj = static_cast<GameObject*>(section->objectAtIndex(j));
            if (currObj == nullptr) continue; 
            auto* currObjMember = MEMBER_BY_OFFSET(void*, currObj, 0x32c);
            if (currObjMember == selectedObj1Member) {
                objectsToDelete.push_back(currObj);
            }
        }
    }
    for (GameObject* obj : objectsToDelete) {
        obj->stopAllActions();
        obj->unscheduleAllSelectors();
        editLayer->removeObject(obj);
    }
    // this->deselectAll(); 
    this->toggleDuplicateButton();
}

LevelEditorLayer* getEditorLayer(EditorUI* ui) {
    return MEMBER_BY_OFFSET(LevelEditorLayer*, ui, 0x1d0);
}

bool (*EditorUI_ccTouchBegan)(CCTouch*, CCEvent*);
bool EditorUI_ccTouchBegan_H(CCTouch* touch, CCEvent* event) {
    if(!PlaytestLayer::get() && PlaytestLayer::get() == nullptr) {
        EditorUI_ccTouchBegan(touch, event);
        return true;
    }
    onTogglePlayerObject(true);
    PlaytestLayer::get()->m_jumping = true;
    // g_player->pushButton(PlayerButton::Button1);
    return true;
}

void (*EditorUI_ccTouchEnded)(CCTouch*, CCEvent*);
void EditorUI_ccTouchEnded_H(CCTouch* touch, CCEvent* event) {
    EditorUI_ccTouchEnded(touch, event);
    if(!PlaytestLayer::get() && PlaytestLayer::get() == nullptr) return;
    PlaytestLayer::get()->m_jumping = false;
    g_player->releaseButton(PlayerButton::Button1);
}


void EditorUI::playtest(CCObject*) {
    auto m_manager = GameManager::sharedState();
    auto playerFrame = MEMBER_BY_OFFSET(int, m_manager, 0x1e0); // GameManager::getPlayerFrame
    auto playerShip = MEMBER_BY_OFFSET(int, m_manager, 0x1e8); // GameManager::getPlayerShip
    g_player = PlayerObject::create(playerFrame, playerShip, MEMBER_BY_OFFSET(LevelEditorLayer*, this, 0x1d0));
    g_player->setPosition(ccp(0, 105));
    MEMBER_BY_OFFSET(CCNode*, getEditorLayer(this), 0x158)->addChild(g_player);
    MEMBER_BY_OFFSET(double, g_player, 0x3c0) = 0;
    CCLog("[VIOLETPS] Player Pos: %.1f, %.1f", g_player->getPositionY(), g_player->getPositionX());
    auto playtest = PlaytestLayer::create(this, g_player);
    this->addChild(playtest);
}

void EditorUI::onPlay(CCObject*) {
auto lel = MEMBER_BY_OFFSET(CCLayer*, this, 0x1d0);
auto grid = MEMBER_BY_OFFSET(DrawGridLayer*, lel, 0x154);
g_gameLayer = MEMBER_BY_OFFSET(CCNode*, lel, 0x158);
g_lineX = g_gameLayer->convertToNodeSpace(CCPointZero).x;
if(g_lineX < lel->convertToNodeSpace(CCPointZero).x) g_lineX = 0.0f;
g_playback = true;
auto lvl = MEMBER_BY_OFFSET(GJGameLevel*, lel, 0x15c);
auto audioTrack = MEMBER_BY_OFFSET(int, lvl, 0x148);
CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(LevelTools_getAudioFilename_H(audioTrack));
CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicTime((g_lineX < 0) ? 0 : g_lineX / 311.58f);

g_playBtn->setVisible(false);
g_playBtn->setEnabled(false);

g_stopBtn->setVisible(true);
g_stopBtn->setEnabled(true);
}

void EditorUI::stopMusic(CCObject*) {
g_playback = false;
CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
g_playBtn->setVisible(true);
g_playBtn->setEnabled(true);

g_stopBtn->setVisible(false);
g_stopBtn->setEnabled(false);
}

CCMenuItemSpriteExtra* menuDeleteAll = nullptr;
void (*EditorUI_setupDeleteMenu)(CCLayer* self);
void EditorUI_setupDeleteMenu_H(CCLayer* self) {
  EditorUI_setupDeleteMenu(self);
  auto win_size = CCDirector::sharedDirector()->getWinSize();

  if(playback) {
    auto spr = CCSprite::create("GJ_playMusicBtn_001.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, spr, self, menu_selector(EditorUI::onPlay));
    g_playBtn = btn;
    btn->setPosition({30, win_size.height / 1.5f});
    auto menu = CCMenu::create(btn, NULL);
    menu->setPosition(CCPointZero);
    self->addChild(menu);

    spr = CCSprite::create("GJ_stopMusicBtn_001.png");
    btn = CCMenuItemSpriteExtra::create(spr, spr, self, menu_selector(EditorUI::stopMusic));
    btn->setVisible(false);
    btn->setEnabled(false);
    g_stopBtn = btn;
    btn->setPosition({30, win_size.height / 1.5f});
    menu = CCMenu::create(btn, NULL);
    menu->setPosition(CCPointZero);
    self->addChild(menu);
    }

    auto spr = CCSprite::create("GJ_playEditorBtn_001.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, spr, self, menu_selector(EditorUI::playtest));
    btn->setPosition({30, win_size.height / 1.5f - 85});
    auto menu = CCMenu::create(btn, NULL);
    menu->setPosition(CCPointZero);
    self->addChild(menu);

auto buttonMenu = CCMenu::create();
auto button = CCSprite::create("deleteAll_001.png");
if(!button || button == nullptr) return;
if(deleteAll) {
menuDeleteAll = CCMenuItemSpriteExtra::create(button, button, self, menu_selector(EditorUI::deleteAll));
menuDeleteAll->setPosition({0,0});
menuDeleteAll->setScale(1.f);
CCLabelBMFont* label = CCLabelBMFont::create( CCString::createWithFormat("Delete All")->getCString(), 
            "bigFont.fnt"
        );
CCPoint point = CCPointMake(win_size.width - 100, win_size.height / 5);
buttonMenu->setPosition(point);
label->setPosition(point);
buttonMenu->addChild(menuDeleteAll);
      }
// self->addChild(label);
if(filterOption) {
auto toggleOffSprite2 = CCSprite::create("GJ_filterBtn_002.png");
  auto toggleOnSprite2 = CCSprite::create("GJ_filterBtn_001.png");
  auto itemOff2 = CCMenuItemSprite::create(toggleOffSprite2, toggleOffSprite2, nullptr, nullptr);
auto itemOn2  = CCMenuItemSprite::create(toggleOnSprite2,  toggleOnSprite2,  nullptr, nullptr);

  auto menuFilter = CCMenuItemToggler::create(
    getMenuToggleSprite(itemOn2, itemOff2, filter),
    getMenuToggleSprite(itemOff2, itemOn2, filter),  
    self,
    menu_selector(EditorUI::toggleFilter) 
);
menuFilter->setPosition({0,0});
menuFilter->setScale(1.f);
buttonMenu->setPosition({win_size.width - 27, win_size.height / 2 - 25});
buttonMenu->addChild(menuFilter);
// self->addChild(label);
self->addChild(buttonMenu, 100);
}
}

void (*EditorUI_toggleMode)(CCLayer* self, CCNode* param);
void EditorUI_toggleMode_H(CCLayer* self, CCNode* param) {
    EditorUI_toggleMode(self, param);
    if(!deleteAll) return;
  auto type = MEMBER_BY_OFFSET(int, self, 0x1cc);
  if(type != 1 && menuDeleteAll != nullptr) {
    menuDeleteAll->removeFromParentAndCleanup(true);
    menuDeleteAll = nullptr;
  } else if(type == 1 && menuDeleteAll == nullptr) {
    auto buttonMenu = CCMenu::create();
auto win_size = CCDirector::sharedDirector()->getWinSize();
auto button = CCSprite::create("deleteAll_001.png");
menuDeleteAll = CCMenuItemSpriteExtra::create(button, button, self, menu_selector(EditorUI::deleteAll));
menuDeleteAll->setPosition(175, -100);
menuDeleteAll->setScale(1.f);
buttonMenu->addChild(menuDeleteAll);
self->addChild(buttonMenu, 100);
  }
}


void* TARGET_OBJ = nullptr; 

void EditorUI::toggleFilter() {
  CCArray* selected = MEMBER_BY_OFFSET(CCArray*, this, 0x180);

  if(filter) {
    filter = false;
    TARGET_OBJ = nullptr;
    return;
  } else filter = true;
    
    if(selected == nullptr) {
        return;
    }

    GameObject* defObject = nullptr;
    if (selected->count() > 0) {
        return;
    } else {
       defObject = MEMBER_BY_OFFSET(GameObject*, this, 0x254);
    }
    if (defObject) {
        TARGET_OBJ = MEMBER_BY_OFFSET(void*, defObject, 0x32c);
    } else {
        TARGET_OBJ = nullptr;
    }
}


CCArray* (*LevelEditorLayer_objectsInRect)(float, float, int, CCRect*);
CCArray* LevelEditorLayer_objectsInRect_H(float param_1, float param_2, int param_3, CCRect* param_4) {
    CCArray* sel = LevelEditorLayer_objectsInRect(param_1, param_2, param_3, param_4);
    if (TARGET_OBJ == nullptr) {
        return sel;
    }
    CCArray* array = CCArray::create(); 
    for (int i = 0; i < sel->count(); i++) {
        GameObject* obj = static_cast<GameObject*>(sel->objectAtIndex(i));
        if (obj == nullptr) continue;
        auto* objMember = MEMBER_BY_OFFSET(void*, obj, 0x32c);
        if (objMember == TARGET_OBJ) {
            array->addObject(obj);
        }
    }
    return array;
}
int (*EditorPauseLayer_init)(CCLayer* self, CCLayer* editor);
int EditorPauseLayer_init_H(CCLayer* self, CCLayer* editor) {
  EditorPauseLayer_init(self, editor);
  onTogglePlayerObject(false);
  if(playback) {
    g_playback = false;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    g_playBtn->setVisible(true);
    g_playBtn->setEnabled(true);

    g_stopBtn->setVisible(false);
    g_stopBtn->setEnabled(false);
  }
  CCArray* children = self->getChildren();
    std::vector<CCNode*> labelsToRemove;
    CCObject* child;
    CCARRAY_FOREACH(children, child) {
        CCNode* node = static_cast<CCNode*>(child);
        CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(node);
        if (label != nullptr) labelsToRemove.push_back(node);
    }
    for (CCNode* label : labelsToRemove) {
        self->removeChild(label, true);
    }
  auto win_size = CCDirector::sharedDirector()->getWinSize();
  int objectCount = MEMBER_BY_OFFSET(int, editor, 0x150);
  auto objText = CCString::createWithFormat("%i/%i objects", objectCount, 16000)->getCString();
  auto objLabel = CCLabelBMFont::create(objText, "goldFont.fnt");
  objLabel->setAnchorPoint({0, 1});
  objLabel->setPosition({10, win_size.height - 5});
  objLabel->setScale(0.5f);
  self->addChild(objLabel);
  if(!levelLength) return 1;

  int editorDistance = floorf((MEMBER_BY_OFFSET(int, editor, 0x164) / 311.0));
  int minutes = editorDistance / 60;
  int seconds = editorDistance % 60;
  CCString* distText = nullptr;
  if(minutes == 1 && seconds != 1) distText = CCString::createWithFormat("%i minute %i seconds", 1, seconds);
  else if(minutes == 1 && seconds == 1) distText = CCString::createWithFormat("%i minute %i second", 1, 1);
  else if(minutes > 1 && seconds != 1) distText = CCString::createWithFormat("%i minutes %i seconds", minutes, seconds);
  else if(minutes > 1 && seconds == 1) distText = CCString::createWithFormat("%i minutes %i second", minutes, 1);
  else if(minutes == 0 && seconds != 1) distText = CCString::createWithFormat("%i seconds", seconds);
  else if(minutes == 0 && seconds == 1) distText = CCString::createWithFormat("%i second", 1);
  else if(minutes > 1 && seconds == 0) distText = CCString::createWithFormat("%i minutes", minutes);
  else if(minutes == 1 && seconds == 0) distText = CCString::createWithFormat("%i minute", 1);
  auto distLabel = CCLabelBMFont::create(distText->getCString(), "goldFont.fnt");
  distLabel->setAnchorPoint({0, 1});
  distLabel->setPosition({10, win_size.height - 20});
  distLabel->setScale(0.5f);
  self->addChild(distLabel);

  cocos2d::CCUserDefault *def = cocos2d::CCUserDefault::sharedUserDefault();
    
    def->setBoolForKey("filterOption", filterOption);

    def->flush();
  return 1;
}

 CCParticleSystemQuad* (*PlayLayer_createParticle)(void* self, int a1, const char* a2, int a3, tCCPositionType type);
 CCParticleSystemQuad* PlayLayer_createParticle_H(void* self, int a1, const char* a2, int a3, tCCPositionType type) {
     CCParticleSystemQuad* particles = PlayLayer_createParticle(self, a1, a2, a3, type);
     if (noParticles) particles->setVisible(false);
     return particles;
 }

void (*PauseLayer_onProgressBar)();
void PauseLayer_onProgressBar_H() {
  PauseLayer_onProgressBar();
  if (noParticles) {
  if(!percentageLabel) {
    percentageLabel = CCLabelBMFont::create(
            CCString::createWithFormat("0%")->getCString(), 
            "bigFont.fnt"
        );
        percentageLabel->setScale(0.5f);
        percentageLabel->setAnchorPoint({0.f, 0.5f});
  }
  GameManager* state = GameManager::sharedState();
  auto win_size = CCDirector::sharedDirector()->getWinSize();
  if(MEMBER_BY_OFFSET(int, state, 0x1a5)) percentageLabel->setPosition(win_size.width - 190, win_size.height - 10);
  else percentageLabel->setPosition(win_size.width / 2, win_size.height - 10);
  }
}

void (*PlayerObject_playerDestroyed)(PlayerObject*);
void PlayerObject_playerDestroyed_H(PlayerObject* self) {
  if(!noDeathEffect) PlayerObject_playerDestroyed(self);
}

void (*EditorUI_showMaxError)();
void EditorUI_showMaxError_H() {
  FLAlertLayer::create(
            nullptr,
            "Max Objects",
            CCString::createWithFormat("You cannot create more than <cy>%i</c> <cg>objects</c> in a single level.", 16000)->getCString(),
            "OK",
            nullptr,
            300.f
  )->show();
}

void (*OptionsLayer_customSetup)(OptionsLayer* self);
void OptionsLayer_customSetup_H(OptionsLayer* self) {
OptionsLayer_customSetup(self);
return;
auto win_size = CCDirector::sharedDirector()->getWinSize();
auto buttonMenu2 = CCMenu::create();
auto button2 = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, self, menu_selector(OptionsLayer::onOpenMenu));
buttonMenu2->setPosition(CCPointZero);
menuBtn2->setPosition({win_size.width, win_size.height});
buttonMenu2->addChild(menuBtn2);
buttonMenu2->setScale(0.5f);
buttonMenu2->setPosition(25, 10);
self->addChild(buttonMenu2, 100);
}

EditorUI* EditorUI::create() {
  auto ret = new EditorUI();
  return ret;
}

CCArray* selObjects = nullptr;
GameObject* selObject = nullptr;
void (*EditorUI_ccTouchMoved)(CCTouch* touch, CCEvent* event);
void EditorUI_ccTouchMoved_H(CCTouch* touch, CCEvent* event) {
EditorUI_ccTouchMoved(touch, event);
auto self = EditorUI::create();
CCArray* selected = MEMBER_BY_OFFSET(CCArray*, self, 0x180);
    if (selected->count() > 0) {
        selObjects = selected;
    } else {
       selObject = MEMBER_BY_OFFSET(GameObject*, self, 0x254);
    }
CCPoint rawViewLocation = touch->locationInView();
CCPoint touchLocation = CCDirector::sharedDirector()->convertToGL(rawViewLocation);
if(selObjects != nullptr) {
  CCObject* object = NULL;
  std::vector<cocos2d::CCPoint> initialOffsets;
  CCARRAY_FOREACH(selObjects, object) 
        {
            if (object)
            {
                GameObject* obj = dynamic_cast<GameObject*>(object);
                CCPoint offset = ccpSub(obj->getPosition(), touchLocation);
                initialOffsets.push_back(offset);
            }
} 

unsigned int index = 0;
    CCARRAY_FOREACH(selObjects, object) 
    {
        if (object && index < initialOffsets.size())
        {
            GameObject* obj = dynamic_cast<GameObject*>(object);
            CCPoint newPosition = ccpAdd(touchLocation, initialOffsets[index]);
            obj->setPosition(newPosition);
            index++;
        }
    }
}
}

struct LegacyString {
    struct Metadata {
        int capacity;
        int length;
        int refCount;
    };
    char* data;
};

// helper to create a string in the format the game expects
void* to_fake_str(const char* text) {
    size_t len = strlen(text);
    auto* totalBuffer = (uint8_t*)malloc(12 + len + 1);

    int* meta = (int*)totalBuffer;
    meta[0] = len;
    meta[1] = len;
    meta[2] = -1;
    
    char* dataPtr = (char*)(totalBuffer + 12);
    strcpy(dataPtr, text);
    
    return dataPtr;
}

// This is the common layout for libstdc++ gnustl strings in older games
struct GameString {
    char* data; // Pointer to the char buffer
};

void set_string_universal(uintptr_t baseObj, uintptr_t offset, const char* text) {
    if (!baseObj) return;
    size_t len = strlen(text);
    uint8_t* buf = (uint8_t*)malloc(len + 13); 
    *(int*)(buf) = len;
    *(int*)(buf + 4) = len;
    *(int*)(buf + 8) = -1;
    char* dataPtr = (char*)(buf + 12);
    memcpy(dataPtr, text, len + 1);
    *(char**)(baseObj + offset) = dataPtr;
}

typedef void* (*tStringCtor)(void* strObj, const char* text, void* allocator);
tStringCtor stringCtor = (tStringCtor)0x2c5dc0;

void set_string_safe(uintptr_t baseObj, uintptr_t offset, const char* text) {
    void* strPtr = (void*)(baseObj + offset);
    
    // 1. Prepare a clean "dummy" allocator object
    // In 32-bit ARM, this is usually 4 bytes.
    uint32_t dummyAlloc = 0; 

    // 2. IMPORTANT: Zero the destination string memory first.
    // If there was an old pointer there, we must clear it so the 
    // constructor doesn't think it needs to 'reallocate' something.
    memset(strPtr, 0, 12); // std::string is typically 12 bytes in 32-bit

    // 3. Call constructor with the text AND the clean allocator
    stringCtor(strPtr, text, &dummyAlloc);
}




GJGameLevel* (*levelTools_getLevel)(int level);
GJGameLevel* levelTools_getLevel_H(int level) {
  if(level != 15) return levelTools_getLevel(level);
  auto lvl = levelTools_getLevel(6);
  /*
  std::string* levelString = new std::string("kS1,40,kS2,125,kS3,255,kS4,0,kS5,102,kS6,255,kA1,10;");
  MEMBER_BY_OFFSET(std::string*, lvl, 0x134) = levelString;

   std::string* levelName = new std::string("test");
  MEMBER_BY_OFFSET(std::string, lvl, 0x12c) = CCString::create("test")->m_sString; */

/* auto setLevelName = reinterpret_cast<void(__thiscall*)(GJGameLevel*, std::string)>(
    *(uintptr_t*)(*(uintptr_t*)lvl + 0x134)
  );
  setLevelName(lvl, "test"); 

  lvl->setLevelName("test");
  lvl->setLevelString("S1,40,kS2,125,kS3,255,kS4,0,kS5,102,kS6,255,kA1,10;");
  */
 /*
    intptr_t vtable = *reinterpret_cast<uintptr_t*>(lvl);
    using setLevelName_t = void(*)(void*, std::string);
    setLevelName_t setLevelName = *reinterpret_cast<setLevelName_t*>(vtable + 0x134);
    using setLevelString_t = void(*)(void*, std::string);
    setLevelString_t setLevelString = *reinterpret_cast<setLevelString_t*>(vtable + 0x144);
    
    setLevelName(lvl, LEVELNAME_1);
    setLevelString(lvl, LEVELSTRING_1); */
    /*
    void* fakeStringData = to_fake_str(LEVELNAME_1);
    void* fakeStringData2 = to_fake_str(LEVELSTRING_1);
    *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(lvl) + 0x12c) = fakeStringData;
    *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(lvl) + 0x134) = fakeStringData2;
    */
    /* void* lvlPtr = &MEMBER_BY_OFFSET(std::string, lvl, 0x12c);
    void* lvlStrPtr = &MEMBER_BY_OFFSET(std::string, lvl, 0x134);
    new (lvlPtr) std::string(LEVELNAME_1);
    new (lvlStrPtr) std::string(LEVELSTRING_1); */

    /* std::string sourceStr = cocos2d::CCString::create(LEVELNAME_1)->m_sString;
    void* destAddr = (void*)((uintptr_t)lvl + 0x12c);
    memcpy(destAddr, &sourceStr, sizeof(std::string)); */

    /* char* gameStrBuffer = (char*)((uintptr_t)lvl + 0x12c + 12); // +12 for gnustl header
    strncpy(gameStrBuffer, LEVELNAME_1, 20);

    char* gameStrBuffer2 = (char*)((uintptr_t)lvl + 0x134 + 12); // +12 for gnustl header
    strncpy(gameStrBuffer2, LEVELSTRING_1, 20); */

    /* std::string sourceStr2 = cocos2d::CCString::create(LEVELSTRING_1)->m_sString;
    void* destAddr2 = (void*)((uintptr_t)lvl + 0x134);
    memcpy(destAddr2, &sourceStr2, sizeof(std::string)); */

    // MEMBER_BY_OFFSET(std::string, lvl, 0x12c) = CCString::create(LEVELNAME_1)->m_sString;
    // MEMBER_BY_OFFSET(std::string, lvl, 0x134) = CCString::create(LEVELSTRING_1)->m_sString;

    set_string_safe((uintptr_t)lvl, 0x12c, LEVELNAME_1);
    set_string_safe((uintptr_t)lvl, 0x134, LEVELSTRING_1);

  MEMBER_BY_OFFSET(int, lvl, 0x148) = 25;
  lvl->setDifficulty(lvl, 4);
  MEMBER_BY_OFFSET(int, lvl, 0x18c) = 12;
  MEMBER_BY_OFFSET(int, lvl, 0x128) = 15;
  MEMBER_BY_OFFSET(int, lvl, 0x1e0) = 1;
  MEMBER_BY_OFFSET(int, lvl, 0x194) = 3;
  MEMBER_BY_OFFSET(int, lvl, 0x1b0) = 0;
  return lvl;
}

std::vector<uint8_t> uintptrToBytes(uintptr_t value) {
    std::vector<uint8_t> bytes(sizeof(uintptr_t));

    for (size_t i = 0; i < bytes.size(); ++i) {
        bytes[i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    }

    return bytes;
}

void LevelEditorLayer_update(LevelEditorLayer* self, float dt) {
    CCRect visible_rect;
    
    auto vr_origin = MEMBER_BY_OFFSET(cocos2d::CCLayer*, self, 0x158)->convertToNodeSpace(CCPoint(0,0));
    auto vr_dest = CCDirector::sharedDirector()->getWinSize();
    
    visible_rect.origin = vr_origin;
    visible_rect.size = vr_dest;
    
    // handle zooming
    visible_rect.size.width *= 1 / MEMBER_BY_OFFSET(cocos2d::CCLayer*, self, 0x158)->getScale();
    visible_rect.size.height *= 1 / MEMBER_BY_OFFSET(cocos2d::CCLayer*, self, 0x158)->getScale();

    // fix blocks disappearing where it shouldn't by extending the visible rect by a few blocks
    visible_rect.origin.x -= 75;
    visible_rect.origin.y -= 75;
    
    visible_rect.size.width += 150;
    visible_rect.size.height += 150;

    auto bn = MEMBER_BY_OFFSET(CCSpriteBatchNode*, self, 0x140);

    for (uint32_t section_id = 0; section_id < MEMBER_BY_OFFSET(CCArray*, self, 0x144)->count(); section_id++) {
        CCArray* section_objects = static_cast<CCArray*>(MEMBER_BY_OFFSET(CCArray*, self, 0x144)->objectAtIndex(section_id));

        for (uint32_t index = 0; index < section_objects->count(); index++) {
            GameObject* object = static_cast<GameObject*>(section_objects->objectAtIndex(index));
            CCPoint object_pos = object->getPosition();

            if (CCRect::CCRectContainsPoint(visible_rect, object_pos)) {
                if (!object->getParent()) {
                    OrderingData* s = static_cast<OrderingData*>(object->getUserData());

                    bn->addChild(object, s->z_order);
                    //object->setOrderOfArrival(s->order_of_arrival);

                    bn->sortAllChildren();
                }
            } else {
                if (object->getParent()) {
                    if (object->getUserData() == nullptr) {
                        OrderingData s = OrderingData {object->getOrderOfArrival(), object->getZOrder()};
                        object->setUserData((void*)&s);
                    }

                    bn->removeChild(object, false);
                }
            }

        }
    }
}

bool (*levelEditorLayer_init)(LevelEditorLayer* self, GJGameLevel* lvl);
bool levelEditorLayer_init_H(LevelEditorLayer* self, GJGameLevel* lvl) {
  levelEditorLayer_init(self, lvl);
  ExtraLayer::m_lel = self;
  if(extendEditor) onToggleEditor(true); 
  else onToggleEditor(false);
   void** vtable = *(void***)self; // ty omnimenu
   void (LevelEditorLayer::* ptr)(float) = &LevelEditorLayer::update;
   void* offset = *(void**)&ptr;
   DobbyCodePatch(&vtable[((uintptr_t)offset)/sizeof(void*)], uintptrToBytes((uintptr_t)&LevelEditorLayer_update).data(), 4);
   CCNode* container = MEMBER_BY_OFFSET(CCNode*, self, 0x158);

    if (container && editorHitbox) {
        HitboxLayer* hitboxes = HitboxLayer::create(self);
        container->addChild(hitboxes, 9999); 
    }
   return true;
}

void (*PlayerObject_updateShipRotation)(PlayerObject* self, float dt);
void PlayerObject_updateShipRotation_H(PlayerObject* self, float dt) {
    if (noRotation) return;
    PlayerObject_updateShipRotation(self, dt);
}

void (*PlayerObject_runRotateAction)(PlayerObject* self);
void PlayerObject_runRotateAction_H(PlayerObject* self) {
    if (noRotation) return;
    PlayerObject_runRotateAction(self);
}
void (*PlayerObject_runBallRotation2)(PlayerObject* self);
void PlayerObject_runBallRotation2_H(PlayerObject* self) {
    if (noRotation) return;
    PlayerObject_runBallRotation2(self);
}

void setOriginalScale(CCMenuItemSpriteExtra* btn, float scale) {
    MEMBER_BY_OFFSET(float, btn, 0x150) = scale;
}

CCMenuItemSpriteExtra* EditorUI::getSpriteButton2(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale)
{
    auto spr = CCSprite::create(name);
    auto btnSpr = ButtonSprite::create(spr, 32, 0, 32, 1.0, true, "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, 0, this, callback);
    btn->setScale(scale);
    setOriginalScale(btn, scale);

    if (menu) menu->addChild(btn);

    return btn;
}
CCMenuItemSpriteExtra* EditorUI::getSpriteButton3(const char* name, SEL_MenuHandler callback, CCMenu* menu, float scale, float sprScale)
{
    auto spr = CCSprite::createWithSpriteFrameName(name);
    spr->setScale(sprScale);
    auto btnSpr = ButtonSprite::create(spr, 32, 0, 32, 1.0, true, "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(btnSpr, 0, this, callback);
    btn->setScale(scale);
    setOriginalScale(btn, scale);

    if (menu) menu->addChild(btn);

    return btn;
}

void (*EditorUI_createMoveMenu)(EditorUI* self);
void EditorUI_createMoveMenu_H(EditorUI* self) {
  if(moreEditorButtons) {
        CCArray* buttons = CCArray::create();

        CCMenuItemSpriteExtra* btn;

        btn = self->getSpriteButton("edit_upBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(3);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(4);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(1);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(2);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_upBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(7);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(8);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(5);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn2_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(6);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_upBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(11);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_downBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(12);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_leftBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(9);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_rightBtn3_001.png", menu_selector(EditorUI::moveObjectCall), nullptr, 0.9);
        btn->setTag(10);
        buttons->addObject(btn);

                btn = self->getSpriteButton3("edit_upBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(1001);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_downBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(1002);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_leftBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(1003);
        buttons->addObject(btn);
        btn = self->getSpriteButton3("edit_rightBtn_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9, 0.8);
        btn->setTag(1004);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_upBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(1005);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_downBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(1006);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_leftBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(1007);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("edit_rightBtn5_001.png", menu_selector(EditorUI::moveObjectCall2), nullptr, 0.9);
        btn->setTag(1008);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_flipXBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
        btn->setTag(17);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_flipYBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
        btn->setTag(18);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_cwBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
        btn->setTag(19);
        buttons->addObject(btn);
        btn = self->getSpriteButton("edit_ccwBtn_001.png", menu_selector(EditorUI::transformObjectCall), nullptr, 0.9);
        btn->setTag(20);
        buttons->addObject(btn);
        btn = self->getSpriteButton2("objectInfo_001.png", menu_selector(EditorUI::getObjectInfoCall), nullptr, 0.9);
        btn->setTag(1);
        buttons->addObject(btn);
        CCDirector* director = CCDirector::sharedDirector();
        auto win_size = director->getWinSize();
        EditButtonBar* newBar = EditButtonBar::create(buttons, ccp(win_size.width * 0.5 - 5, MEMBER_BY_OFFSET(float, director->getOpenGLView(), 0xe4) + MEMBER_BY_OFFSET(float, self, 0x158) - 6.f), false);
        MEMBER_BY_OFFSET(EditButtonBar*, self, 0x148) = newBar;
        self->addChild(newBar, 11);
  } else EditorUI_createMoveMenu(self);
    }

    void EditorUI::getObjectInfoCall(CCNode* sender) {
      if (!MEMBER_BY_OFFSET(GameObject*, this, 0x254) && MEMBER_BY_OFFSET(CCArray*, this, 0x180)->count() <= 0) return;
      int identifier = sender->getTag();
      if(identifier == 1) {
        CCArray* selected = MEMBER_BY_OFFSET(CCArray*, this, 0x180);

    GameObject* definingObject = nullptr;
    if (selected->count() == 0) {
      definingObject = MEMBER_BY_OFFSET(GameObject*, this, 0x254);
      int typeID = MEMBER_BY_OFFSET(int, definingObject, 0x32c);

      CCPoint point = definingObject->getPosition();
      const char* pointStr = CCString::createWithFormat("{%.0f, %.0f}", point.x, point.y)->getCString();

      int objectType = MEMBER_BY_OFFSET(int, definingObject, 0x2c8);
      int flipY = definingObject->isFlipY();
      int flipX = definingObject->isFlipX();
      int rotation = definingObject->getRotation();
      FLAlertLayer::create(
            nullptr,
            ObjectInfoHandler::getObjectNameFromKey(typeID),
            CCString::createWithFormat("<cy>Object Type ID: </c>%i\n<cp>Position:</c> %s\n<cr>Object Type:</c> %i\n<cg>Flip X:</c> %i\n<cg>Flip Y:</c> %i\n<cg>Rotation:</c> %i", typeID, pointStr, objectType, flipX, flipY, rotation)->getCString(),
            "OK",
            nullptr,
            300.f
      )->show();
    }
    }
    }

    void EditorUI::moveObjectCall2(CCNode* sender) {
    if (!MEMBER_BY_OFFSET(GameObject*, this, 0x254) && MEMBER_BY_OFFSET(CCArray*, this, 0x180)->count() <= 0) return;

    auto transform = ccp(0, 0);

    switch (sender->getTag()) {

        case 1001:
            transform.y = 1;
            break;
        case 1002:
            transform.y = -1;
            break;
        case 1003:
            transform.x = -1;
            break;
        case 1004:
            transform.x = 1;
            break;
        case 1005:
            transform.y = 15;
            break;
        case 1006:
            transform.y = -15;
            break;
        case 1007:
            transform.x = -15;
            break;
        case 1008:
            transform.x = 15;
            break;

        default:
            break;
    }
    if (MEMBER_BY_OFFSET(CCArray*, this, 0x180)->count() > 0) {
        for (int i = 0; i < MEMBER_BY_OFFSET(CCArray*, this, 0x180)->count(); i++) {
            this->moveObject(static_cast<GameObject*>(MEMBER_BY_OFFSET(CCArray*, this, 0x180)->objectAtIndex(i)), transform);
        }
    } else {
        this->moveObject(MEMBER_BY_OFFSET(GameObject*, this, 0x254), transform);
    }
}

void OptionsLayer::onOpenMenu() {
  this->setTouchEnabled(false);
  extra = ExtraLayer::create(this);
  this->addChild(extra, 1000);
  extra->_setZOrder(10000000);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void VersionRequest::fetchVersion() {
  std::thread t(&VersionRequest::networkThread, this);
  t.detach();
}

void VersionRequest::networkThread() {
    void* handle = dlopen("libcurl.so", RTLD_LAZY);
    if (!handle) handle = dlopen("libgame.so", RTLD_LAZY);
    if (!handle) return;

    typedef void* (*curl_init_t)();
    typedef int (*curl_setopt_t)(void*, int, ...);
    typedef int (*curl_perform_t)(void*);
    typedef void (*curl_cleanup_t)(void*);
    typedef const char* (*curl_error_t)(int);

    auto curl_init = (curl_init_t)dlsym(handle, "curl_easy_init");
    auto curl_setopt = (curl_setopt_t)dlsym(handle, "curl_easy_setopt");
    auto curl_perform = (curl_perform_t)dlsym(handle, "curl_easy_perform");
    auto curl_cleanup = (curl_cleanup_t)dlsym(handle, "curl_easy_cleanup");

    void* curl = curl_init();
    if (curl) {
        std::string responseString;
        curl_setopt(curl, 10002, "pok.ps.fhgdps.com/ver.php");
        curl_setopt(curl, 20011, WriteCallback);
        curl_setopt(curl, 10001, &responseString);
        curl_setopt(curl, 64, 0L);

        int res = curl_perform(curl);

        if (res == 0) { // CURLE_OK
            char* copy = new char[responseString.length() + 1];
            strcpy(copy, responseString.c_str());
            MenuLayer::s_newVersion.store(copy); 
            MenuLayer::s_hasVersionData = true;
        }

        curl_cleanup(curl);
    }
    dlclose(handle);
}

typedef void (*MenuLayer_Clicked_T)(MenuLayer*, FLAlertLayer*, bool);
MenuLayer_Clicked_T MenuLayer_FLAlert_Clicked_Orig = nullptr;
void MenuLayer_FLAlert_Clicked_H(MenuLayer* self, FLAlertLayer* alert, bool btn2) {
  if(!alert || !btn2) return;
    switch (alert->getTag()) { 
      case 1001:
      cocos2d::CCApplication::sharedApplication().openURL("http://pok.ps.fhgdps.com/downloads");
      break;
      case 2:
      cocos2d::CCApplication::sharedApplication().openURL("http://www.youtube.com/@nano56gd");
      break;
      case 0:
      auto delegate = (AppDelegate *)AppDelegate::get();
      AppDelegate::trySaveGame();
      self->endGame();
    } 
}

void (*GJGameLevel_savePercentage)(GJGameLevel*, int, bool);
void GJGameLevel_savePercentage_H(GJGameLevel* self, int percentage, bool asfcsdvfvnvhf) {
if(safeMode) return;
}

void (*PlayLayer_showNewBest)(CCLayer*);
void PlayLayer_showNewBest_H(CCLayer* self) {
  if(!safeMode) PlayLayer_showNewBest(self);
}

void (*CocosDenshion_SimpleAudioEngine_playEffect)(char* file, bool loop, float setting1, float setting2, float setting3);
void CocosDenshion_SimpleAudioEngine_playEffect_H(char* file, bool loop, float setting1, float setting2, float setting3) {
    if (!fmodSystem) return;
    FMOD::Sound* sound = nullptr;
    fmodSystem->createSound(CCString::createWithFormat("file:///android_asset/%s", file)->getCString(), FMOD_DEFAULT, 0, &sound);
    FMOD::Channel* channel = nullptr;
    fmodSystem->playSound(sound, nullptr, true, &channel);
    if (channel) {
    channel->setPitch(setting1);
    channel->setPan(setting2);
    channel->setVolume(setting3);
    if (loop) {
    channel->setMode(FMOD_LOOP_NORMAL);
    channel->setLoopCount(-1);
    }
    channel->setPaused(false);
    }
}

void (*stopBackgroundMusicJNI_tram)(void*, void*, void*, bool);
void stopBackgroundMusicJNI_H(void* env, void* clazz, void* methodID, bool idk) {
    if (bgmChannel) {
        bgmChannel->stop();
        bgmChannel = nullptr;
    }
    if (idk && bgmSound) {
        bgmSound->release();
        bgmSound = nullptr;
    }
}

void (*rewindBackgroundMusicJNI_tram)(void*, void*, void*, bool);
void rewindBackgroundMusicJNI_H(void* env, void* clazz, void* methodID, bool idk) {
if (bgmChannel) {
    bgmChannel->setPosition(0, FMOD_TIMEUNIT_MS);
}
}

void (*pauseBackgroundMusicJNI_tram)(JNIEnv* env, jclass clazz);
void pauseBackgroundMusicJNI_H(JNIEnv* env, jclass clazz) {
if (bgmChannel) {
        bgmChannel->setPaused(true);
    }
fmodSystem->update();
}

void (*resumeBackgroundMusicJNI_tram)(JNIEnv* env, jclass clazz);
void resumeBackgroundMusicJNI_H(JNIEnv* env, jclass clazz) {  
    if (bgmChannel) {
        bgmChannel->setPaused(false);
    }
    fmodSystem->update();
  }

void (*playBackgroundMusicJNI_tram)(const char *path, bool isLoop);
void playBackgroundMusicJNI_H(const char *path, bool isLoop) {
    if (bgmChannel) {
        bgmChannel->stop();
        bgmChannel = nullptr;
    }
    std::string fullPath;
        if (path[0] != '/') {
    fullPath = "file:///android_asset/" + std::string(path);
        } else fullPath = std::string(path);
        
        FMOD_MODE mode = isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        if (fmodSystem) {
            fmodSystem->createStream(fullPath.c_str(), mode, 0, &bgmSound);
            fmodSystem->playSound(bgmSound, 0, false, &bgmChannel);
            fmodSystem->update();
        }
}


int (*isBackgroundMusicPlayingJNI_tram)();
int isBackgroundMusicPlayingJNI_H() {
  if (bgmChannel) {
  bool playing = false;
  if (bgmChannel->isPlaying(&playing) == FMOD_OK) {
  return playing ? 1 : 0;
  }
  }
  return 0;
}

void (*CCDirector_drawScene)(CCDirector* director);
void CCDirector_drawScene_H(CCDirector* director) {
  CCDirector_drawScene(director);
  if (fmodSystem) {
        fmodSystem->update();
  }
}

int (*AppDelegate_applicationDidFinishLaunching)(AppDelegate* self);
int AppDelegate_applicationDidFinishLaunching_H(AppDelegate* self) {
    int ret = AppDelegate_applicationDidFinishLaunching(self);
    
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    g_vm = cocos2d::JniHelper::getJavaVM(); 
    JNIEnv* env = nullptr;
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_4) == JNI_EDETACHED) {
        if (jvm->AttachCurrentThread(&env, nullptr) != 0) return ret;
    }
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject activityThreadObj = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject activityContext = env->CallObjectMethod(activityThreadObj, getApplication);

    if (activityContext) {
        FMOD_Android_JNI_Init(jvm, activityContext);
        
        FMOD::System_Create(&fmodSystem);
        fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
        
        env->DeleteLocalRef(activityContext);
    }
    env->DeleteLocalRef(activityThreadObj);
    env->DeleteLocalRef(activityThread);
    return ret;
}



int (*scene)(GJGameLevel*);
int scene_H(GJGameLevel* lvl) {
  if (bgmChannel) {
        bgmChannel->stop();
        bgmChannel = nullptr;
    }
    if (bgmSound) {
        bgmSound->release();
        bgmSound = nullptr;
    }

    FMOD::ChannelGroup *masterGroup;
    fmodSystem->getMasterChannelGroup(&masterGroup);
    masterGroup->stop();
    fmodSystem->update();
    return scene(lvl);
}

void (*setBackgroundMusicTimeJNI_tram)(float time);
void setBackgroundMusicTimeJNI_H(float time) {
unsigned int ms = (unsigned int)(time * 1000.0f);
bgmChannel->setPosition(ms, FMOD_TIMEUNIT_MS);
}

void (*setBackgroundMusicVolumeJNI_tram)(float);
void setBackgroundMusicVolumeJNI_H(float volume) {
  if (volume < 0.0f) volume = 0.0f;
  bgmChannel->setVolume(volume);
}

void (*playEffectJNI_tram)(const char*, bool);
void playEffectJNI_H(const char* path, bool bLoop) {
    if (!fmodSystem || !path) return;
    std::string fullPath = (path[0] == '/') ? path : "file:///android_asset/" + std::string(path);
    FMOD::Sound* targetSound = nullptr;
    auto it = sfxCache.find(fullPath);
    if (it != sfxCache.end()) {
        targetSound = it->second;
    } else {
        FMOD_MODE mode = (bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | FMOD_DEFAULT;
        if (fmodSystem->createSound(fullPath.c_str(), mode, 0, &targetSound) == FMOD_OK) {
            sfxCache[fullPath] = targetSound;
        }
    }
    if (targetSound) {
        FMOD::Channel* channel = nullptr;
        fmodSystem->playSound(targetSound, 0, false, &channel);
    }
}



void (*stopEffectJNI_tram)(unsigned int);
void stopEffectJNI_H(unsigned int) {
    if (sfxChannel) {
    sfxChannel->stop();
    sfxChannel = nullptr;
    }
}

int (*LoadingLayer_init)(CCLayer*);
int LoadingLayer_init_H(CCLayer* self) {
  LoadingLayer_init(self);
  auto win_size = CCDirector::sharedDirector()->getWinSize();
  auto fmodLogo = CCSprite::create("FMODLogo_001.png");
  fmodLogo->setAnchorPoint(CCPointMake(1.0f, 0.f));
  fmodLogo->setPosition(CCPointMake(win_size.width -5, 5));
  self->addChild(fmodLogo);
  return 1;
}

void (*PlayerObject_pushButton)(float, void*, int);
void PlayerObject_pushButton_H(float idk, void* gman, int idk2) {
PlayerObject_pushButton(idk, gman, idk2);
auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
    cocos2d::CCArray* children = scene->getChildren();
    cocos2d::CCObject* obj;

    PlayLayer* playLayer = nullptr;
    CCARRAY_FOREACH(children, obj) {
        if (dynamic_cast<PlayLayer*>(obj)) {
            playLayer = (PlayLayer*)obj;
            break;
        }
    }
if(!playLayer) return;
clicks++;
float currentTime = MEMBER_BY_OFFSET(float, HidePauseLayer::m_playLayer, 0x2e4);
clickTimestamps.push_back(currentTime);
}

bool checkHasDownloadedSong(int ID) {
  auto newSongID = ID + 1;
  std::string s_persistentPath = "/storage/emulated/0/Music/saved_songs/" + to_string(newSongID) + ".mp3";
  std::ifstream file(s_persistentPath);
  return file.good();
}

void (*LevelInfoLayer_onPlay)(LevelInfoLayer*);
void LevelInfoLayer_onPlay_H(LevelInfoLayer* self) {
auto lvl = MEMBER_BY_OFFSET(GJGameLevel*, self, 0x154);
auto songID = MEMBER_BY_OFFSET(int, lvl, 0x148);
if(!checkHasDownloadedSong(songID) && !hasShowedAlert && songID > SONGS && !dontShowAlert) {
  auto alert = FLAlertLayer::create(
    self,
    "No Song",
    CCString::createWithFormat("This level uses a <cl>custom song</c> that has not been <cg>downloaded</c> yet.\nDo you want to play without music?\n<cy>Download by going to the Custom Song Library and search for the ID %i</c>.", songID + 1)->getCString(), // No CCString
    "Cancel",
    "Play",
    300.f
);
  alert->setTag(1000);
  alert->show();
  hasShowedAlert = true;
  return;
}
LevelInfoLayer_onPlay(self);
}

void (*LevelInfoLayer_FLAlertClicked)(LevelInfoLayer*, FLAlertLayer*, bool);
void LevelInfoLayer_FLAlertClicked_H(LevelInfoLayer* self, FLAlertLayer* alert, bool btn2) {
  if(btn2 && alert->getTag() == 1000) {
    LevelInfoLayer_onPlay(self);
    return;
  }
  LevelInfoLayer_FLAlertClicked(self, alert, btn2);
}

std::string formattedTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t timer = std::chrono::system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);
    std::ostringstream oss;
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", &bt);
    std::string str(buffer); 
    return str;
}

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct sigaction old_sa[NSIG];

void native_crash_handler(int sig, siginfo_t* info, void* secret) {
    const char* path = "/storage/emulated/0/violetps/crash_log.txt";
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    
    if (fd != -1) {
        char buf[128];
        int len = snprintf(buf, sizeof(buf), "Native Crash\nSignal: %d\n", sig);
        write(fd, buf, len);
        close(fd);
    }

    if (old_sa[sig].sa_flags & SA_SIGINFO) {
        if (old_sa[sig].sa_sigaction) {
            old_sa[sig].sa_sigaction(sig, info, secret);
        }
    } else {
        if (old_sa[sig].sa_handler == SIG_DFL) {
            signal(sig, SIG_DFL);
            raise(sig);
        } else if (old_sa[sig].sa_handler != SIG_IGN) {
            old_sa[sig].sa_handler(sig);
        }
    }
}

void setup_crash_handler() {
    stack_t ss;
    ss.ss_sp = malloc(SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = native_crash_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigfillset(&sa.sa_mask);

    int signals[] = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS};
    for (int s : signals) {
        sigaction(s, &sa, &old_sa[s]);
    }
}

void (*EditorUI_constrainGameLayerPosition)(CCLayer* self);
void EditorUI_constrainGameLayerPosition_H(CCLayer* self) {
    if(!extendEditor) EditorUI_constrainGameLayerPosition(self);
}

void (*EditorUI_selectObject)(void*, void*);
void EditorUI_selectObject_H(void* self, void* object) {
    if (filter && TARGET_OBJ != nullptr) {
        auto objectID = MEMBER_BY_OFFSET(void*, object, 0x32c);
        if (objectID == TARGET_OBJ) {
            EditorUI_selectObject(self, object);
        }
    } else {
        EditorUI_selectObject(self, object);
    }
}

void (*EditorUI_onDelete)(void*);
void EditorUI_onDelete_H(void* self) {
   auto object = MEMBER_BY_OFFSET(LevelEditorLayer*, self, 0x1d0)->objectAtPosition(MEMBER_BY_OFFSET(CCPoint, self, 0x234));
   if(!object) return;
   if(!filter || MEMBER_BY_OFFSET(void*, object, 0x32c) == TARGET_OBJ) EditorUI_onDelete(self);
}

// this one is optional since its not actually needed
void (*EditorUI_onDeleteSelected)(void*);
void EditorUI_onDeleteSelected_H(void* self) {
if(filter) {
CCArray* selected = MEMBER_BY_OFFSET(CCArray*, self, 0x180);
    LevelEditorLayer* edit = MEMBER_BY_OFFSET(LevelEditorLayer*, self, 0x1d0);
    CCArray* sections = MEMBER_BY_OFFSET(CCArray*, edit, 0x144);
    if(edit == nullptr || sections == nullptr || selected == nullptr) {
        return;
    }
    if (selected->count() > 0) {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(selected, pObj) {
            GameObject* object = (GameObject*)pObj;
            if(object == TARGET_OBJ) MEMBER_BY_OFFSET(LevelEditorLayer*, self, 0x1d0)->removeObject(object);
        }
    } else {
        auto object = MEMBER_BY_OFFSET(GameObject*, self, 0x254);
        auto* objectID = MEMBER_BY_OFFSET(void*, object, 0x32c);
        if(TARGET_OBJ == objectID) EditorUI_onDeleteSelected(self);
    }
} else EditorUI_onDeleteSelected(self);
}

void (*CCTransitionFade_create)(float, CCScene*, const ccColor3B&);
void CCTransitionFade_create_H(float dur, CCScene* scene, const ccColor3B& color) {
    if(noSceneTransition) CCTransitionFade_create(0.f, scene, color);
    else CCTransitionFade_create(dur, scene, color);
}

void DrawGridLayer::update(float dt) {
g_lineX += dt * 311.58f;
}

void (*DrawGridLayer_draw)(DrawGridLayer* self);
void DrawGridLayer_draw_H(DrawGridLayer* self) {
    DrawGridLayer_draw(self);
    if(g_playback) {
        static long lastFrameId = -1;
        long currentFrameId = cocos2d::CCDirector::sharedDirector()->getTotalFrames();
        if (currentFrameId != lastFrameId) {
            float dt = cocos2d::CCDirector::sharedDirector()->getAnimationInterval();
            g_lineX += dt * 311.58f;
            
            lastFrameId = currentFrameId;
        }
        ccDrawColor4F(0.0f, 1.f, 0.0f, 1.0f);
        ccDrawLine(ccp(g_lineX, 0), ccp(g_lineX, 100000));
    }
}

void EditLevelLayer::exportGMD(CCObject*) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, 
        "org/cocos2dx/lib/Cocos2dxActivity",
        "openGmdSelector",
        "()V")) 
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void LevelBrowserLayer::onSelectGMD() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, 
        "org/cocos2dx/lib/Cocos2dxActivity",
        "openGmdImportSelector",
        "()V")) 
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void LevelBrowserLayer::loadLevel(GJGameLevel* level) {
    CCLog("1");
    level->m_nLevelID = 0;
    level->m_eLevelType = GJLevelType::Editor;
#ifdef FORCE_AUTO_SAFE_MODE
    level->m_bIsVerified = false;
#if GAME_VERSION > GV_1_2
    level->m_bIsDemon = false;
    level->m_nStars = 0;
#endif
#endif
    MEMBER_BY_OFFSET(CCArray*, LocalLevelManager::sharedState(), 0x130)->addObject(level);
    runAction(CCSequence::create(
        CCDelayTime::create(0.2f),
        CCCallFunc::create(this, callfunc_selector(LevelBrowserLayer::reload)),
        nullptr
    ));
    
}

bool (*EditLevelLayer_init)(CCLayer*, GJGameLevel*);
bool EditLevelLayer_init_H(CCLayer* self, GJGameLevel* lvl) {
    g_currentExportLevel = lvl;
    EditLevelLayer_init(self, lvl);
    auto win_size = CCDirector::sharedDirector()->getWinSize();
    auto buttonMenu2 = CCMenu::create();
auto button2 = CCSprite::create("GJ_violetMod_001.png");
CCMenuItemSpriteExtra* menuBtn2 = CCMenuItemSpriteExtra::create(button2, button2, self, menu_selector(EditLevelLayer::onOpenMenu));
buttonMenu2->setPosition(CCPointZero);
menuBtn2->setPosition({-60, win_size.height + 90});
buttonMenu2->addChild(menuBtn2);
buttonMenu2->setScale(0.5f);
buttonMenu2->setPosition(-55, 12.5);
self->addChild(buttonMenu2, 100);
    CCSprite* exportSpr = CCSprite::create("gdshare_export.png");
    CCMenuItemSpriteExtra* exportBtn = CCMenuItemSpriteExtra::create(exportSpr, exportSpr, self, menu_selector(EditLevelLayer::exportGMD));
    auto shareMenu = CCMenu::create(exportBtn, NULL);
    self->addChild(shareMenu, 1001);
    shareMenu->setPosition(ccp(win_size.width - 29, win_size.height - 85.f));
    return true;
}

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxActivity_nativeOnGmdSaveSelected(JNIEnv* env, jobject thiz, jobject uri) {
        GJGameLevel* level = g_currentExportLevel; 

        if (!level || !uri) return;
        jclass activityClass = env->GetObjectClass(thiz);
        jmethodID getCR = env->GetMethodID(activityClass, "getContentResolver", "()Landroid/content/ContentResolver;");
        jobject resolver = env->CallObjectMethod(thiz, getCR);

        jclass resolverClass = env->FindClass("android/content/ContentResolver");
        jmethodID openOut = env->GetMethodID(resolverClass, "openOutputStream", "(Landroid/net/Uri;)Ljava/io/OutputStream;");
        jobject outputStream = env->CallObjectMethod(resolver, openOut, uri);

        if (!outputStream) return;
        auto dict = new DS_Dictionary();
        void* encodeWithCoder = DobbySymbolResolver("libgame.so", "_ZN11GJGameLevel15encodeWithCoderEP13DS_Dictionary");
        if (encodeWithCoder) {
            ((void(*)(GJGameLevel*, DS_Dictionary*))encodeWithCoder)(level, dict);
        }

        std::string strObj = dict->saveRootSubDictToString();
        
        jbyteArray jData = env->NewByteArray(strObj.length());
        env->SetByteArrayRegion(jData, 0, strObj.length(), reinterpret_cast<const jbyte*>(strObj.c_str()));

        jclass outputStreamClass = env->FindClass("java/io/OutputStream");
        jmethodID writeMethod = env->GetMethodID(outputStreamClass, "write", "([B)V");
        jmethodID closeMethod = env->GetMethodID(outputStreamClass, "close", "()V");

        env->CallVoidMethod(outputStream, writeMethod, jData);
        env->CallVoidMethod(outputStream, closeMethod);

        env->DeleteLocalRef(jData);
        env->DeleteLocalRef(outputStream);
        env->DeleteLocalRef(outputStreamClass);
        env->DeleteLocalRef(resolver);
        env->DeleteLocalRef(resolverClass);
        env->DeleteLocalRef(activityClass);
        
        dict->release();
    }
}

void* getCurrentLayer() {
    return cocos2d::CCDirector::sharedDirector()->getRunningScene();
}

void LevelBrowserLayer::reload() {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, LevelBrowserLayer::scene(MEMBER_BY_OFFSET(GJSearchObject*, this, 0x154))));
}

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxActivity_nativeOnGmdImportSelected(JNIEnv* env, jobject thiz, jobject uri) {
        if (!uri) return;
        jclass activityClass = env->GetObjectClass(thiz);
        jmethodID getCR = env->GetMethodID(activityClass, "getContentResolver", "()Landroid/content/ContentResolver;");
        jobject resolver = env->CallObjectMethod(thiz, getCR);
        jclass resolverClass = env->FindClass("android/content/ContentResolver");
        jmethodID openIn = env->GetMethodID(resolverClass, "openInputStream", "(Landroid/net/Uri;)Ljava/io/InputStream;");
        jobject inputStream = env->CallObjectMethod(resolver, openIn, uri);
        if (!inputStream) return;
        std::vector<uint8_t> result;
        jclass streamClass = env->FindClass("java/io/InputStream");
        jmethodID readMethod = env->GetMethodID(streamClass, "read", "([B)I");
        jbyteArray buffer = env->NewByteArray(8192);
        while (true) {
            jint bytesRead = env->CallIntMethod(inputStream, readMethod, buffer);
            if (bytesRead <= 0) break;
            jbyte* bytes = env->GetByteArrayElements(buffer, nullptr);
            result.insert(result.end(), bytes, bytes + bytesRead);
            env->ReleaseByteArrayElements(buffer, bytes, JNI_ABORT);
        }
        jmethodID closeMethod = env->GetMethodID(streamClass, "close", "()V");
        env->CallVoidMethod(inputStream, closeMethod);
        std::string str(result.begin(), result.end());
        auto dict = new DS_Dictionary();
        dict->retain();
        if (dict->loadRootSubDictFromString(str)) {
            typedef GJGameLevel* (*createWithCoder_t)(DS_Dictionary*);
            static auto createFunc = (createWithCoder_t)DobbySymbolResolver("libgame.so", "_ZN11GJGameLevel15createWithCoderEP13DS_Dictionary");
            
            if (createFunc) {
                GJGameLevel* level = createFunc(dict);
                if (level) {
                    level->retain();
                    g_importedLevel = level;
                }
            }
        }
        dict->release(); 
        env->DeleteLocalRef(buffer);
        env->DeleteLocalRef(inputStream);
        env->DeleteLocalRef(resolver);
        env->DeleteLocalRef(resolverClass);
        env->DeleteLocalRef(activityClass); 
    }
}

void LevelBrowserLayer::onImport(CCObject* sender) {
    onSelectGMD();
    this->schedule(schedule_selector(LevelBrowserLayer::checkImport), 0.1f);
}

void LevelBrowserLayer::checkImport(float dt) {
    if (g_importedLevel != nullptr) {
        this->unschedule(schedule_selector(LevelBrowserLayer::checkImport));
        
        this->loadLevel(g_importedLevel);
        g_importedLevel->release();
        g_importedLevel = nullptr;
    }
}

bool (*LevelBrowserLayer_init)(CCLayer* self, GJSearchObject* search);
bool LevelBrowserLayer_init_H(CCLayer* self, GJSearchObject* search) {
    LevelBrowserLayer_init(self, search);
    if(MEMBER_BY_OFFSET(int, search, 0x128) != 98) return true;
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        CCMenu* importMenu = CCMenu::create();
        CCSprite* importSpr = cocos2d::CCSprite::create("gdshare_import.png");
        CCMenuItemSpriteExtra* importBtn = CCMenuItemSpriteExtra::create(importSpr, importSpr, self, menu_selector(LevelBrowserLayer::onImport));
        importMenu->addChild(importBtn);
        self->addChild(importMenu, 100000);
        importMenu->setPosition(ccp(winSize.width - 30.f, 90.f));
    return true;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  setup_crash_handler();
  HOOK("_ZN8EditorUI12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE", EditorUI_ccTouchEnded_H, EditorUI_ccTouchEnded);
  HOOK("_ZN8EditorUI12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", EditorUI_ccTouchBegan_H, EditorUI_ccTouchBegan);
  HOOK("_ZN17LevelBrowserLayer4initEP14GJSearchObject", LevelBrowserLayer_init_H, LevelBrowserLayer_init);
  HOOK("_ZN13DrawGridLayer4drawEv", DrawGridLayer_draw_H, DrawGridLayer_draw);
  HOOK("_ZN13MenuGameLayer4initEv", MenuGameLayer_init_H, MenuGameLayer_init);
  HOOK("_ZN7cocos2d16CCTransitionFade6createEfPNS_7CCSceneERKNS_10_ccColor3BE", CCTransitionFade_create_H, CCTransitionFade_create);
  // HOOK("_ZN8EditorUI4drawEv", EditorUI_draw_H, EditorUI_draw);
  HOOK("_ZN8EditorUI8onDeleteEv", EditorUI_onDelete_H, EditorUI_onDelete);
  HOOK("_ZN8EditorUI16onDeleteSelectedEv", EditorUI_onDeleteSelected_H, EditorUI_onDeleteSelected);
  HOOK("_ZN8EditorUI12selectObjectEP10GameObject", EditorUI_selectObject_H, EditorUI_selectObject);
  HOOK("_ZN8EditorUI26constrainGameLayerPositionEv", EditorUI_constrainGameLayerPosition_H, EditorUI_constrainGameLayerPosition);
  HOOK("_ZN14LevelInfoLayer15FLAlert_ClickedEP12FLAlertLayerb", LevelInfoLayer_FLAlertClicked_H, LevelInfoLayer_FLAlertClicked);
  HOOK("_ZN14LevelInfoLayer6onPlayEv", LevelInfoLayer_onPlay_H, LevelInfoLayer_onPlay);

  HOOK("_ZN12PlayerObject10pushButtonE12PlayerButton", PlayerObject_pushButton_H, PlayerObject_pushButton);
  HOOK("_ZN12LoadingLayer4initEv", LoadingLayer_init_H, LoadingLayer_init);
  HOOK("_ZN16LevelEditorLayer5sceneEP11GJGameLevel", scene_H, scene);

  HOOK("_ZN11AppDelegate29applicationDidFinishLaunchingEv", AppDelegate_applicationDidFinishLaunching_H, AppDelegate_applicationDidFinishLaunching);
  HOOK("_ZN7cocos2d10CCDirector9drawSceneEv", CCDirector_drawScene_H, CCDirector_drawScene);

  /*********************************** REPLACE SIMPLEAUDIOENGINE WITH FMOD ***********************************/
  HOOK("resumeBackgroundMusicJNI", resumeBackgroundMusicJNI_H, resumeBackgroundMusicJNI_tram);
  HOOK("pauseBackgroundMusicJNI", pauseBackgroundMusicJNI_H, pauseBackgroundMusicJNI_tram);
  HOOK("stopBackgroundMusicJNI", stopBackgroundMusicJNI_H, stopBackgroundMusicJNI_tram);
  HOOK("playBackgroundMusicJNI", playBackgroundMusicJNI_H, playBackgroundMusicJNI_tram);
  HOOK("isBackgroundMusicPlayingJNI", isBackgroundMusicPlayingJNI_H, isBackgroundMusicPlayingJNI_tram);
  HOOK("setBackgroundMusicTimeJNI", setBackgroundMusicTimeJNI_H, setBackgroundMusicTimeJNI_tram);
  HOOK("setBackgroundMusicVolumeJNI", setBackgroundMusicVolumeJNI_H, setBackgroundMusicVolumeJNI_tram);
  HOOK("playEffectJNI", playEffectJNI_H, playEffectJNI_tram);
  HOOK("stopEffectJNI", stopEffectJNI_H, stopEffectJNI_tram);
  HOOK("rewindBackgroundMusicJNI", rewindBackgroundMusicJNI_H, rewindBackgroundMusicJNI_tram);
  /*******************************************************************************************************/

  // HOOK("_ZN9PlayLayer11showNewBestEv", PlayLayer_showNewBest_H, PlayLayer_showNewBest);
  // HOOK("_ZN11GJGameLevel14savePercentageEib", GJGameLevel_savePercentage_H, GJGameLevel_savePercentage);
  HOOK("_ZN9MenuLayer15FLAlert_ClickedEP12FLAlertLayerb", MenuLayer_FLAlert_Clicked_H, MenuLayer_FLAlert_Clicked_Orig);
  HOOK("_ZN12OptionsLayer11customSetupEv", OptionsLayer_customSetup_H, OptionsLayer_customSetup);
  // HOOK("_ZN13CocosDenshion17SimpleAudioEngine19playBackgroundMusicEPKcb", CocosDenshion_SimpleAudioEngine_playBackgroundMusic_H, CocosDenshion_SimpleAudioEngine_playBackgroundMusic);
  HOOK("_ZN8EditorUI14createMoveMenuEv", EditorUI_createMoveMenu_H, EditorUI_createMoveMenu);
  HOOK("_ZN12PlayerObject18updateShipRotationEf", PlayerObject_updateShipRotation_H, PlayerObject_updateShipRotation);
  HOOK("_ZN12PlayerObject15runRotateActionEv", PlayerObject_runRotateAction_H, PlayerObject_runRotateAction);
  HOOK("_ZN12PlayerObject16runBallRotation2Ev", PlayerObject_runBallRotation2_H, PlayerObject_runBallRotation2);
  // HOOK("_ZN10LevelTools8getLevelEi", levelTools_getLevel_H, levelTools_getLevel);
  HOOK("_ZN16LevelEditorLayer4initEP11GJGameLevel", levelEditorLayer_init_H, levelEditorLayer_init);
  // HOOK("_ZN8EditorUI12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", EditorUI_ccTouchMoved_H, EditorUI_ccTouchMoved);
  HOOK("_ZN8EditorUI12showMaxErrorEv", EditorUI_showMaxError_H, EditorUI_showMaxError);
  HOOK("_ZN12PlayerObject15playerDestroyedEv", PlayerObject_playerDestroyed_H, PlayerObject_playerDestroyed);
  HOOK("_ZN10PauseLayer13onProgressBarEv", PauseLayer_onProgressBar_H, PauseLayer_onProgressBar);
  // HOOK("_ZN8EditorUI19transformObjectCallEPN7cocos2d6CCNodeE", EditorUI_transformObjectCall_H, EditorUI_transformObjectCall);
  // HOOK("_ZN9PlayLayer14createParticleEiPKciN7cocos2d15tCCPositionTypeE", PlayLayer_createParticle_H, PlayLayer_createParticle);
 // HOOK("_ZN16GameStatsManager7getStatEPKc", GameStatsManager_getStat_H, GameStatsManager_getStat);
  HOOK("_ZN16GameStatsManager13incrementStatEPKci", GameStatsManager_incrementStat_H, GameStatsManager_incrementStat);
  HOOK("_ZN16EditorPauseLayer4initEP16LevelEditorLayer", EditorPauseLayer_init_H, EditorPauseLayer_init);
  HOOK("_ZN16LevelEditorLayer13objectsInRectEN7cocos2d6CCRectE", LevelEditorLayer_objectsInRect_H, LevelEditorLayer_objectsInRect);
  HOOK("_ZN8EditorUI10toggleModeEPN7cocos2d6CCNodeE", EditorUI_toggleMode_H, EditorUI_toggleMode);
  HOOK("_ZN8EditorUI15setupDeleteMenuEv", EditorUI_setupDeleteMenu_H, EditorUI_setupDeleteMenu);
  HOOK("_ZN12OptionsLayer6onRateEv", OptionsLayer_onRate_H, OptionsLayer_onRate);
  HOOK("_ZN14EditLevelLayer4initEP11GJGameLevel", EditLevelLayer_init_H, EditLevelLayer_init);

  HOOK("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayer_init_H, PlayLayer_init);
  HOOK("_ZN9PlayLayer18togglePracticeModeEb", PlayLayer_togglePracticeMode_H, PlayLayer_togglePracticeMode);
  // HOOK("_ZN18LevelSettingsLayer9audioNextEv", LevelSettingsLayer_audioNext_H, LevelSettingsLayer_audioNext);
  // HOOK("_ZN18LevelSettingsLayer13audioPreviousEv", LevelSettingsLayer_audioPrevious_H, LevelSettingsLayer_audioPrevious);
  HOOK("_ZN12LoadingLayer16getLoadingStringEv", LoadingLayer_getLoadingString_H, LoadingLayer_getLoadingString);
  // HOOK("_ZN11CommentCell15loadFromCommentEP9GJComment", CommentCell_loadFromComment_H, CommentCell_loadFromComment);
  HOOK("_ZN9PlayLayer6onQuitEv", PlayLayer_onQuit_H, PlayLayer_onQuit);

  HOOK("_ZN12CreatorLayer4initEv", CreatorLayer_init_H, CreatorLayer_init);

  HOOK("_ZN18LevelSettingsLayer4initEP19LevelSettingsObject", LevelSettingsLayer_init_H, LevelSettingsLayer_init);
  // HOOK("_ZN11GameManager14isIconUnlockedEi8IconType", GameManager_isIconUnlocked_H, GameManager_isIconUnlocked);
  // HOOK("_ZN11GameManager15isColorUnlockedEib", GameManager_isColorUnlocked_H, GameManager_isColorUnlocked);

  HOOK("_ZN12SupportLayer7onEmailEv", SupportLayer_onEmail_H, SupportLayer_onEmail);

  HOOK("_ZN9PlayLayer13destroyPlayerEv", PlayLayer_destroyPlayer_H, PlayLayer_destroyPlayer);
  HOOK("_ZN9PlayLayer13levelCompleteEv", PlayLayer_levelComplete_H, PlayLayer_levelComplete);
  HOOK("_ZN7UILayer4initEv", UILayer_init_H, UILayer_init);
  HOOK("_ZN9PlayLayer10resetLevelEv", PlayLayer_resetLevel_H, PlayLayer_resetLevel);
  HOOK("_ZN9PlayLayer6resumeEv", PlayLayer_resume_H, PlayLayer_resume);

  HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_customSetup_H, PauseLayer_customSetup);

  HOOK("_ZN10LevelTools13getAudioTitleEi", LevelTools_getAudioTitle_H, LevelTools_getAudioTitle);
  HOOK("_ZN10LevelTools16getAudioFileNameEi", LevelTools_getAudioFilename_H, LevelTools_getAudioFilename);

   HOOK("_ZN9MenuLayer4initEv", MenuLayer_init_H, MenuLayer_init);

  HOOK("_ZN9PlayLayer6updateEf", PlayLayer_onUpdate_H, PlayLayer_onUpdate);
  
  HOOK("_ZN10LevelTools14getAudioStringEi", LevelTools_getAudioString_H, LevelTools_getAudioString);
  // HOOK("_ZN10LevelTools14artistForAudioEi", LevelTools_artistForAudio_H, LevelTools_artistForAudio);
  // HOOK("_ZN10LevelTools13nameForArtistEi", LevelTools_nameForArtist_H, LevelTools_nameForArtist); */
  
  HOOK("_ZN9LevelCell19loadCustomLevelCellEv", LevelCell_loadCustomLevelCell_H, LevelCell_loadCustomLevelCell);
  HOOK("_ZN11GJGameLevel15encodeWithCoderEP13DS_Dictionary", GJGameLevel_encodeWithCoder_H, GJGameLevel_encodeWithCoder);
  HOOK("_ZN11GJGameLevel15createWithCoderEP13DS_Dictionary", GJGameLevel_createWithCoder_H, GJGameLevel_createWithCoder);
  HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevel", LevelInfoLayer_init_H, LevelInfoLayer_init);
  HOOK("_ZN11GJGameLevelD1Ev", GJGameLevel_destructor_H, GJGameLevel_destructor); 

  PatchManager pm;
  pm.cpatch(0x14d8d4, CCString::createWithFormat("%02X 29", SONGS + 1)->getCString()); // LevelSettingsLayer::selectSong
  pm.cpatch(0x14d8d8, CCString::createWithFormat("%02X 21", SONGS)->getCString()); // LevelSettingsLayer::selectSong
  pm.cpatch(0x14d920, CCString::createWithFormat("%02X 28", SONGS + 1)->getCString()); // LevelSettingsLayer::audioNext

  // pm.cpatch(0x16bbf4, "10 2d"); // LevelSelectLayer::init

  pm.cpatch(0x16aa8c, "04 21"); // LevelPage::init

  editorPatch.cpatch(0x14fff4, "00 00 00 00"); // EditorUI::onCreateObject
  editorPatch.cpatch(0x14ee10, "00 00 00 00"); // EditorUI::moveObject

  editorPatch.cpatch(0x14fffc, "00 FF FF 48"); // EditorUI::onCreateObject
  // editorPatch.cpatch(0x14ee18, "00 FF FF 48"); // EditorUI::moveObject

  iconPatch.cpatch(0x13bd8c, "01 20 70 47");
  iconPatch.cpatch(0x13be2c, "01 20 70 47");

  /*
  pm.cpatch(0x139968, "00 BF 00 BF"); // PlayLayer::resetLevel
  pm.cpatch(0x138bb2, "00 BF 00 BF"); // PlayLayer::resume
  */

  pm.cpatch(0x1399a0, "00 BF 00 BF"); // PlayLayer::resetLevel

  controlPatch.cpatch(0x1545aa, "00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF 00 BF");

  flipPatch = MemoryPatch::createWithHex("libgame.so", 0x1373fe, "1DE0"); 


  playerObjectPatch.cpatch(0x1446ee, "00 BF 00 BF");
  playerObjectPatch.cpatch(0x1461e6, "00 BF 00 BF");
  playerObjectPatch.cpatch(0x145054, "00 BF 00 BF");


  pm.Modify();
  return JNI_VERSION_1_6;
}