#pragma once
#include <cocos2dcpp.h>
#include "base/structs.hpp"
#include "base/consts.hpp"

class rbot 
{
public:
    static Modules& getModules(){
        static Modules data;
        return data;
    };

    static std::vector<Frame>& getFrameData()
    {
        static std::vector<Frame> data;
        return data;
    }

    static std::vector<CheckpointData>& getCheckpointData()
    {
        static std::vector<CheckpointData> data;
        return data;
    }

    static void reset()
    {
        getModules().frame = 0;
        getModules().completed = false;
        rbot::getCheckpointData().clear();
    }

    static void stopMacro()
    {
        rbot::getModules().mode = kModeNone;
        rbot::getModules().recording = false;
        rbot::getModules().playing = false;
    }

    static std::string getVersion()
    {
        return "0.93 beta";
    }
};