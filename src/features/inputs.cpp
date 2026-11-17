#include <cocos2dcpp.h>
#include "hooking.hpp"
#include "tools.hpp"

void (*CCTextInputNode_onClickTrackNode)(CCTextInputNode*, bool);
void CCTextInputNode_onClickTrackNode_H(CCTextInputNode* self, bool attachWithIME)
{
    if (self->isVisible() && self->getParent()->isVisible())
    {
        auto textField = MEMBER_BY_OFFSET(CCTextFieldTTF*, self, CCTextInputNode__m_textField);

        if (attachWithIME) textField->attachWithIME();
        else textField->detachWithIME();
    }
}

void inputs_hook()
{
    // HOOK("_ZN15CCTextInputNode16onClickTrackNodeEb", CCTextInputNode_onClickTrackNode_H, CCTextInputNode_onClickTrackNode);
}