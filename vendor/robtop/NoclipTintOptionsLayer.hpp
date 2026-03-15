#pragma once
#include <cocos2d.h>
#include "CCBlockLayer.h"
#include "ColorSelectPopup.hpp"

class ColorPickerPopup;
class ColorPickerDelegate
{
public:
    virtual void colorPickerClosed(ColorPickerPopup *);
	static CCLayerColor* create();
};

class NoclipTintOptionsLayer : public ColorPickerDelegate {

public:
    static NoclipTintOptionsLayer* create(CCLayer* referrer);
	bool init(CCLayer* referrer);
	void onColorFinished(cocos2d::ccColor3B color);
	virtual void colorPickerClosed(ColorPickerPopup* picker) override;
};
