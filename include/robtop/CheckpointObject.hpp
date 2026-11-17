#pragma once

#include <cocos2dcpp.h>
using namespace cocos2d;

class CheckpointObject : public CCNode
{
    static CheckpointObject* create();
};

class PlayerCheckpoint : public CCNode
{
    static PlayerCheckpoint* create();
};