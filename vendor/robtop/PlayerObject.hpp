#pragma once

#include <cocos2d.h>
#include "EndPortalObject.hpp"
#include "GameObject.hpp"

using namespace cocos2d;

enum PlayerButton {
	Button1 = 1,
};

class PlayerObject : public GameObject {
public:
    static PlayerObject* create(int, int, cocos2d::CCLayer*);
    bool init(void*);
    void lockPlayer();
    void gravityDown();
    void gravityUp();
    void speedDown();
    void speedUp();
    void yStartDown();
    void yStartUp();
    void logValues();

    void pushButton(PlayerButton);
	void releaseButton(PlayerButton);

	void updateJump(float);
	void hitGround(bool);
	void setLastP(CCPoint);

    void deactivateParticle();
    void flipMod();
    virtual CCLayer* getGameLayer() const;
    virtual EndPortalObject* getPortalObject();
};
