#pragma once

#include <cocos2dcpp.h>
#include "enums.hpp"

struct Frame {
    double frame;
    float flipY;
    CCPoint position;
    float rotation;
    bool click;
    bool release;
    bool onGround;
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
    double rotation;
};

struct PlayerHitbox {
    CCRect hazardRect;
    CCRect blockRect;
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
    std::vector<PlayerHitbox> playerHitboxes;
    bool gameStarted;
    bool rbotOpened;
    CCMenuItemExt* speedhackBtn;
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