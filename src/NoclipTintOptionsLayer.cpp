#include "../vendor/robtop/NoclipTintOptionsLayer.hpp"
#include "../vendor/robtop/CCMenuItemSpriteExtra.hpp"
#include "../vendor/robtop/ButtonSprite.hpp"
#include "../vendor/robtop/EditorConfigurationsLayer.hpp"
#include "../vendor/robtop/CCMenuItemToggler.hpp"
#include <cocos2d.h>
#include "extensions\CCControlExtension\CCControlColourPicker.h"

/* CCSprite* getToggleSpritee(CCSprite* on, CCSprite* off, bool state) { return (state) ? on : off; }
CCMenuItemSprite* getMenuToggleSpritee(CCMenuItemSprite* on, CCMenuItemSprite* off, bool state) { return (state) ? on : off; } */

 NoclipTintOptionsLayer* NoclipTintOptionsLayer::create(CCLayer* referrer) {
    auto ret = new NoclipTintOptionsLayer();
    if (ret->init(referrer)) {
        return ret;
    }
    delete ret;
    return nullptr;
}


#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))
void NoclipTintOptionsLayer::colorPickerClosed(ColorPickerPopup* picker) {

}

class ColorPickerDelegate;
class ColorPickerPopup : public FLAlertLayer {
public:
    static ColorPickerPopup* create();
    bool init();
    _ccColor3B getColorValue();
    void selectColor(_ccColor3B color);
	virtual void setDelegate(ColorPickerDelegate* delegate);
	virtual void setColor(_ccColor3B color);
	virtual void getColor(_ccColor3B& color);

    void onCopy();
    void onPaste();
};

void NoclipTintOptionsLayer::onColorFinished(cocos2d::ccColor3B color) {
    ExtraLayer::m_flashColor = color;
    cocos2d::CCLog("NoclipTint: Color Saved - R:%d G:%d B:%d", color.r, color.g, color.b);

}

class ColorPickerProxy : public ColorPickerDelegate {
public:
    NoclipTintOptionsLayer* owner;
    virtual void colorPickerClosed(ColorPickerPopup* picker) override {
    if (owner && picker) {
        extension::CCControlColourPicker* thePicker = MEMBER_BY_OFFSET(cocos2d::extension::CCControlColourPicker*, picker, 0x1cc);
        cocos2d::ccColor3B color;
        if (thePicker) color = thePicker->getColorValue();
        else color = picker->getColorValue();
        cocos2d::CCLog("VIOLETPS: color: R:%d G:%d B:%d", color.r, color.g, color.b);

        owner->onColorFinished(color);
    }
    delete this;
}



};

bool NoclipTintOptionsLayer::init(CCLayer* referrer) {
  ColorPickerPopup* picker = ColorPickerPopup::create();
  ColorPickerProxy* proxy = new ColorPickerProxy();
  proxy->owner = this;
  picker->setDelegate(proxy);
  ccColor3B initialColor = ExtraLayer::m_flashColor;
  picker->selectColor(initialColor);
  picker->setTag(61);
  picker->show();

    return true;
}