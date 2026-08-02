#pragma once
#include <cocos2dcpp.h>
#include <vector>
#include "robtop/CCBlockLayer.h"
#include "robtop/PauseLayer.hpp"
#include "rbot/Toggler.h"
#include "robtop/CCTextInputNode.hpp"
#include "robtop/PlayLayer.hpp"
#include "MemoryPatch.h"
#include "FLAlertLayer.hpp"
#include "init.hpp"
using namespace cocos2d;

#pragma pack(push, 1)
struct Action {
    int frame;
    bool push;
    bool operator==(const Action& other) const {
        return this->frame == other.frame;
    }
    CCPoint position;
    double yVelocity;
};
#pragma pack(pop)

struct Frame {
    double frame;
    float flipY;
    CCPoint position;
    float rotation;
    bool click;
    bool release;
    bool operator==(const Frame& other) const {
        return this->frame == other.frame;
    }
};

struct CheckpointData {
    double frame;
    bool operator==(const CheckpointData& other) const {
        return this->frame == other.frame;
    }
    double yVelocity;
    double gravity;
};

enum Modes {
    kModeNone,
    kModeRecording,
    kModePlaying,
};

struct Modules {
    int frame;
    int action;
    bool dead;
    Modes mode;
    bool recording;
    bool playing;
    float time;
    bool completed;
    CCLabelBMFont* playbackLabel;
    bool advanceFrame;
    CCMenu* stepperMenu;
    float dt;
    bool stepperOn;
    bool paused;
    std::string soundPath;
    bool isClicking;
    Frame* usedFrame;
};

struct File
{
    std::string path;
    std::string name;
    long long size;
    bool operator==(const File& other) const
    {
        return name == other.name;
    }
};

class RBot {
public:
    static Modules& getModules();
    static std::vector<Action>& getActions();
    static std::vector<Frame>& getFrameData();
    static std::vector<CheckpointData>& getCheckpointData();
    static void onNextFrame(PlayLayer* self);
    static void loadActionData();
    static Action* getCurrentAction();
    static Action* getActionByNumber(int num);
    static Action* getActionByFrame(int frame);
    static void addAction(int frame, bool push);
    static void removeLastAction();
    static void removeAllActions();
    static void reset();
    static void stopMacro();
    static std::string getVersion();
};

class RBotLayer : public CCLayer, public CCTextFieldDelegate, public FLAlertLayerProtocol {
public:
    PauseLayer* m_parent;
    Toggler* m_recordToggler;
    Toggler* m_playToggler;
    CCTextInputNode* m_textInput;
    CCLayer* m_macroPage;
    CCLayer* m_settingsPage;
    CCMenuItemToggler* m_macroToggler;
    CCMenuItemToggler* m_settingsToggler;
    CCLabelBMFont* m_label;
    CCMenu* m_soundsMenu;
    static RBotLayer* create(PauseLayer*);
    virtual bool init();
    void clearMacro(CCObject*);
    virtual void keyBackClicked() override;
    void toggle(CCObject*);
    void saveFile();
    void loadFile();
    CCNode* togglerFromModule(const Module&);
    void updateLabel();
    CCNode* createToggler(const char* text, bool* var, CCObject* target, SEL_MenuHandler callback, int tag);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) override;
};