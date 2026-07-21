#pragma once
#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "CCTextInputNode.hpp"

class ObjectToolbox {

public:
    static ObjectToolbox* sharedState();
	char const* keyToFrame(char const*);
};
