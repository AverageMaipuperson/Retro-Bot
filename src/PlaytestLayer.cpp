#include "../vendor/robtop/PlaytestLayer.hpp"
#include "../vendor/robtop/EditorUI.hpp"
#include "../vendor/robtop/LevelEditorLayer.hpp"
#include "../vendor/robtop/PlayerObject.hpp"
#include "../vendor/robtop/GameManager.hpp"
#include <string>
#include <vector>
#include <cocos2d.h>
using namespace cocos2d;

PlaytestLayer* PlaytestLayer::self = nullptr;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

PlaytestLayer* PlaytestLayer::create(EditorUI* ui, PlayerObject* player) {
    auto ret = new PlaytestLayer();
    if(ret && ret->init(ui, player)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr; 
}

PlaytestLayer* PlaytestLayer::get() {
    return PlaytestLayer::self;
}

void PlaytestLayer::update(float dt) {
    MEMBER_BY_OFFSET(double, m_player, 0x390) = 5.77; 
    PlaytestLayer::self = this;
    // m_player->update(dt);
    m_player->setLastP(m_player->getRealPosition());

    // MEMBER_BY_OFFSET(double, m_player, 0x3a0) = 0.9582;
    // MEMBER_BY_OFFSET(int, m_player, 0x43b) = 0;

    if (m_jumping) m_player->pushButton(PlayerButton::Button1);

    CCLog("Y Acelleration: %.2f \n Gravity: %.2f \n Y Start: %.2f", MEMBER_BY_OFFSET(double, m_player, 0x3c0), MEMBER_BY_OFFSET(double, m_player, 0x3a0), MEMBER_BY_OFFSET(double, m_player, 0x398));

    MEMBER_BY_OFFSET(CCParticleSystem*, m_player, 0x410)->stopSystem();

    for (int i = 0; i < 4; i++) {
        m_player->update(dt * 60.0f / 4);

        if (m_player->getPositionY() <= 105.0f) {
            m_player->setPositionY(105.0f);
            m_player->hitGround(false);
            this->m_jumping = false;
            break; 
        }
    }

    // m_playerPos += dt * 311.58f;
    m_playerPos += 5.77;
    m_player->setPositionX(m_playerPos);

    auto editorLayer = MEMBER_BY_OFFSET(LevelEditorLayer*, m_ui, 0x1d0);
    CCArray* objects = *(CCArray**)((char*)editorLayer + 0x144);
    CCRect playerRect = m_player->getObjectRect();
    CCObject* obj;
    CCARRAY_FOREACH(objects, obj) {
    CCArray* section = (CCArray*)obj;
    CCObject* innerObj;
    CCARRAY_FOREACH(section, innerObj) {
    GameObject* gameObj = (GameObject*)innerObj;
    if (abs(gameObj->getPositionX() - m_player->getPositionX()) > 100) continue;
    CCRect objRect = gameObj->getObjectRect();
    if (cocos2d::CCRect::CCRectIntersectsRect(playerRect, objRect)) {
    int type = MEMBER_BY_OFFSET(int, gameObj, 0x2c8);
    if (type == 2) {
    m_player->setVisible(false);
    } else if (type == 0) {
    m_player->hitGround(false);
    }
    }
    }
    }
}

bool PlaytestLayer::init(EditorUI* ui, PlayerObject* player) {
    m_ui = ui;
    m_player = player;
    this->scheduleUpdate();
    return true;
}