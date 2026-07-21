#pragma once

#include <cocos2d.h>
#include <vector>
#include <string>
using namespace cocos2d;

class DropdownDelegate {
public:
    virtual void onItemSelected(CCObject*) = 0;
};

class DropdownWidget; 
class CCDropdown : public CCMenu {
	public:
    inline static DropdownWidget* widget = nullptr;
    inline static std::vector<CCLabelBMFont*> m_labelVector = {};
	static CCMenu* createDropdown(const std::vector<std::string>& options, CCObject* target, SEL_MenuHandler selector, DropdownWidget*);
    void draw();
};

class DropdownWidget : public CCNode {
public:
    CCMenu* m_optionsMenu;
    CCLabelBMFont* m_boxLabel;
	CCSprite* m_arrowsprite;
    bool m_isOpen;
    int m_selectedOption;
    std::vector<CCLabelBMFont*> m_labelVector;

    static DropdownWidget* create(std::vector<std::string> options, CCObject* target, SEL_MenuHandler selector, int sel) {
        auto ret = new DropdownWidget();
        if (ret && ret->init(options, target, selector, sel)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(std::vector<std::string> options, CCObject* target, SEL_MenuHandler selector, int sel) {
        this->m_selectedOption = sel;
        CCDropdown::widget = this;
        m_isOpen = false;
        auto boxSprite = CCLayerColor::create(ccc4(0, 0, 0, 255), 150.f, 40.f);
        boxSprite->setContentSize(CCSizeMake(150, 40));
        
        m_boxLabel = CCLabelBMFont::create("Select", "chatFont.fnt", 20);
        m_boxLabel->setPosition(ccp(75, 20));
        boxSprite->addChild(m_boxLabel);

		m_arrowsprite = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
		m_arrowsprite->setPosition(ccp(130, 20));
		boxSprite->addChild(m_arrowsprite);

        auto boxBtn = CCMenuItemSprite::create(boxSprite, boxSprite, this, menu_selector(DropdownWidget::toggleDropdown));
        auto topMenu = CCMenu::create(boxBtn, NULL);
        topMenu->setPosition(CCPointZero);
        this->addChild(topMenu, 2);
        m_optionsMenu = CCDropdown::createDropdown(options, target, selector, this);
        m_optionsMenu->setPosition(ccp(0, -((40.f / 2) + (40.f * options.size() / 2)) - 10));
        m_optionsMenu->setVisible(false);
        this->addChild(m_optionsMenu, 1);

        return true;
    }

    void toggleDropdown(CCObject* pSender) {
        m_isOpen = !m_isOpen;
        m_optionsMenu->setVisible(m_isOpen);
        if(m_isOpen) {
			m_optionsMenu->setHandlerPriority(-129);
			m_arrowsprite->setFlipY(true);
            int x = 0;
            for (const auto& label : m_labelVector) {
                if(x == m_selectedOption) label->setColor({255, 255, 255});
                else label->setColor({127, 127, 127});
                x++;
            }
		} else m_arrowsprite->setFlipY(false);
    }

    void updateLabel(const char* text) {
        changeDisplay(text);
        toggleDropdown(nullptr);
    }

    virtual void setSelected(int option) {
        m_selectedOption = option;
    }

    virtual int getSelected() {
        return m_selectedOption;
    }

    virtual void changeDisplay(const char* text) {
        m_boxLabel->setString(text);
    }
};
