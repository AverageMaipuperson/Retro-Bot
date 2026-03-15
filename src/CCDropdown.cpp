#include "../vendor/robtop/CCDropdown.hpp"
#include <string>
#include <vector>
#include <cocos2d.h>
using namespace cocos2d;

void CCDropdown::draw() {
    /* ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f); 
    glLineWidth(2.0f); 
    ccDrawLine(ccp(0, 0), ccp(150, 0)); */
}



CCMenu* CCDropdown::createDropdown(const std::vector<std::string>& options, CCObject* target, SEL_MenuHandler selector, DropdownWidget* drwidget) {
    CCArray* menuItems = CCArray::create();
    float itemWidth = 150.0f;
    float itemHeight = 40.0f;

    m_labelVector.clear(); 

    for (int i = 0; i < options.size(); ++i) {
        auto background = CCLayerColor::create(ccc4(0, 0, 0, 255), itemWidth, itemHeight);
        background->setAnchorPoint(ccp(0.5f, 0.5f));
        auto label = CCLabelBMFont::create(options[i].c_str(), "goldFont.fnt", 0);
        label->setScale(0.5f);
        label->setPosition(ccp(itemWidth / 2, itemHeight / 2));
        if(drwidget->m_selectedOption != i) label->setColor(ccc3(127, 127, 127));
        m_labelVector.push_back(label);
        background->addChild(label);
        CCMenuItemSprite* item = CCMenuItemSprite::create(background, background, target, selector);
        item->setTag(i);
        
        menuItems->addObject(item);
    }

    drwidget->m_labelVector = m_labelVector;

    CCMenu* menu = CCMenu::menuWithArray(menuItems);
    menu->alignItemsVerticallyWithPadding(0);
    return menu;
}

