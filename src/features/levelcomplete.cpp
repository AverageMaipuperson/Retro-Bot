#include "EndLevelLayer.hpp"
#include "other/hooking.h"
#include "TextArea.hpp"
#include "tools.hpp"
#include "RBot.h"

char* (*EndLevelLayer_getEndText)(EndLevelLayer*);
char* EndLevelLayer_getEndText_H(EndLevelLayer*)
{
    return (char*)"Recorded with Retro Bot :)";
}

void (*EndLevelLayer_customSetup)(EndLevelLayer*);
void EndLevelLayer_customSetup_H(EndLevelLayer* self)
{
    EndLevelLayer_customSetup(self);
    if (RBot::getModules().mode != kModePlaying) return;

    auto mainLayer = MEMBER_BY_OFFSET(CCNode*, self, EndLevelLayer__m_mainLayer);
    auto children = mainLayer->getChildren();

    for (int i = 0 ; i < children->count() ; i++)
    {
        auto obj = children->objectAtIndex(i);
        if(auto label = dynamic_cast<TextArea*>(obj))
        {
            label->setScale(.5f);
            label->setString("Recorded with Retro Bot :)");
        }
    }
}

void levelcomplete_hook()
{
    HOOK("_ZN13EndLevelLayer11customSetupEv", EndLevelLayer_customSetup_H, EndLevelLayer_customSetup);
    // HOOK("_ZN13EndLevelLayer10getEndTextEv", EndLevelLayer_getEndText_H, EndLevelLayer_getEndText);
}