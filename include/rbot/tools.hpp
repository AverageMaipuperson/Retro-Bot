#pragma once
#include <cocos2dcpp.h>
#include "CCMenuItemSpriteExtra.hpp"
#include "robtop/CCMenuItemToggler.hpp"
#include "robtop/UILayer.hpp"
#include "robtop/PlayerObject.hpp"
#include "MemoryPatch.h"
#include <type_traits>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "sys/stat.h"
#include "offsets.hpp"
using namespace cocos2d;

inline CCSprite* getToggleSprite(CCSprite* on, CCSprite* off, bool state) { return (state) ? on : off; }
inline CCMenuItemSprite* getMenuToggleSprite(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; }

/* template <typename T>
inline T getFuncPtrFromSymbol(const char* symbol) {
void* addr = dlsym(RTLD_DEFAULT, symbol);
if (!addr) {
    return nullptr;
}
return reinterpret_cast<T>(addr);
} */

inline void* fake(const char* text) {
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

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

inline PlayerObject* getPlayer(void* playLayer) {
return MEMBER_BY_OFFSET(PlayerObject*, playLayer, PlayLayer__m_playerObject);
}

inline bool isPractice(void* playLayer) {
return MEMBER_BY_OFFSET(bool, playLayer, PlayLayer__m_isPractice);
}

struct RawStr {
    char* data;
    uint32_t size;
    uint32_t cap;
};

inline void setAllowedChars(CCTextInputNode* node, const std::string& newChars) {
    static std::string persistentChars = newChars; 
    MEMBER_BY_OFFSET(void*, node, CCTextInputNode__m_allowedChars) = fake(newChars.c_str());
}

inline void setCharLimit(CCTextInputNode* node, int limit) {
    MEMBER_BY_OFFSET(int, node, CCTextInputNode__m_charLimit) = limit;
}

/* inline void setRealPosition(PlayerObject* player, CCPoint pos) {
    MEMBER_BY_OFFSET(CCPoint, player, PlayerObject__m_realPosition) = pos;
} */

#if GAME_VERSION < GV_1_7
inline void patchMemory(std::vector<MemoryPatch> patch, uintptr_t hex, char const* bytes) {
    patch.push_back(MemoryPatch::createWithHex("libgame.so", hex, bytes));
}
#else
inline void patchMemory(std::vector<MemoryPatch> patch, uintptr_t hex, char const* bytes) {
    patch.push_back(MemoryPatch::createWithHex("libcocos2dcpp.so", hex, bytes));
}
#endif

inline void modifyAll(std::vector<MemoryPatch> patch) {
    for(int k = 0; k < patch.size(); k++){
        patch[k].Modify();
    }
}

inline void restoreAll(std::vector<MemoryPatch> patch) {
    for(int k = 0; k < patch.size(); k++){
        patch[k].Restore();
    }
}

inline PlayerObject* getPlayer(PlayLayer* playLayer) {
    return MEMBER_BY_OFFSET(PlayerObject*, playLayer, PlayLayer__m_playerObject);
}

inline bool create_directories(const std::string& path) {
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

inline void setChildrenEnabled(CCLayer* layer, bool enabled)
{
    auto children = layer->getChildren();
    if(children)
    {
        for(int i = 0 ; i < children->count() ; i++)
        {
            if(auto menu = dynamic_cast<CCMenu*>(children->objectAtIndex(i)))
            {
                menu->setEnabled(enabled);
            }
        }
    }

    layer->setVisible(enabled);
}

inline void setAllMenusEnabled(CCNode* node, bool enabled) {
    if (auto menu = dynamic_cast<CCMenu*>(node)) menu->setEnabled(enabled);

    CCArray* children = node->getChildren();
    if (children)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(children, pObj)
        {
            if (auto child = dynamic_cast<CCNode*>(pObj)) setAllMenusEnabled(child, enabled);
        }
    }
}

inline void limitLabelWidth(CCLabelBMFont* label, float maxWidth)
{
    float width = label->getContentSize().width * label->getScale();
    if (width > maxWidth) label->setScale(maxWidth / label->getContentSize().width);
}