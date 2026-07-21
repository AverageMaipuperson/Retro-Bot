#pragma once
#include <cocos2d.h>
using namespace cocos2d;

class ButtonSpriteExtra : public CCSprite {
public:
	CCSprite* m_sButtonSprite;
	CCLabelBMFont* m_sDisplayLabel;
	CCSprite* m_sDisplaySprite;
	static ButtonSpriteExtra* create(char const* displayName, char const* displaySprite, char const* buttonSprite, float length);
	static ButtonSpriteExtra* createWithSpriteFrameName(char const* displayName, char const* displaySprite, char const* buttonSprite, float length);

	bool init(char const* displayName, char const* displaySprite, char const* buttonSprite, float length);
	bool initWithSpriteFrameName(char const* displayName, char const* displaySprite, char const* buttonSprite, float length);

	void changeButtonSprite(char const*);
};