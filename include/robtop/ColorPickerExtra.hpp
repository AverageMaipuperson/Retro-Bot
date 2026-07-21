#pragma once
#include <cocos2d.h>
using namespace cocos2d;

class LayoutOptionsLayer : public CCLayer {
public:
    static ColorPickerExtra* create();
    virtual bool init();
    
    CCLayer* m_mainLayer;
    extension::CCControlColourPicker* m_picker;
    CCMenu* m_buttonMenu;
    
    void onCopy(CCObject* sender);
    void onPaste(CCObject* sender);
    void onClose(CCObject* sender);
};
