///////////////////////////////////////////////////////
/*                     PlayLayer                     */
///////////////////////////////////////////////////////

#define PlayLayer__m_hazards              0x188  // PlayLayer::checkCollisions
#define PlayLayer__m_isPractice           0x255  // PlayLayer::getPracticeMode
#define PlayLayer__m_realPosition         0x1f0  // PlayLayer::update (second to last cocos2d::CCPoint::operator= call)
#define PlayLayer__m_playerObject         0x22c  // PlayLayer::getPlayer
#define PlayLayer__m_cameraPos            0x234  // PlayLayer::getCameraPos
#define PlayLayer__m_objects              0x184  // PlayLayer::init (first CCArray::create call after setSecondColor)
#define PlayLayer__m_gameLayer            0x260  // PlayLayer::getGameLayer
#define PlayLayer__m_isFlipped            0x21c  // PlayLayer::getIsFlipped

///////////////////////////////////////////////////////
/*                    PlayerObject                   */
///////////////////////////////////////////////////////

#define PlayerObject__m_gravity           0x358  // PlayerObject::gravityUp
#define PlayerObject__m_yVelocity         0x378  // PlayerObject::hitGround (first line)
#define PlayerObject__m_onGround          0x430  // PlayerObject::getOnGround

///////////////////////////////////////////////////////
/*                  CCTextInputNode                  */
///////////////////////////////////////////////////////

#define CCTextInputNode__m_charLimit      0x170  // CCTextInputNode::getCharLimit
#define CCTextInputNode__m_inputDelegate  0x16c  // CCTextInputNode::getDelegate
#define CCTextInputNode__m_textField      0x168  // CCTextInputNode::getTextField
#define CCTextInputNode__m_allowedChars   0x150  // CCTextInputNode::setAllowedChars

///////////////////////////////////////////////////////
/*                     GameObject                    */
///////////////////////////////////////////////////////

#define GameObject__m_type                0x2bc  // GameObject::getType
#define GameObject__m_key                 0x31c  // GameObject::getObjectKey

///////////////////////////////////////////////////////
/*                      Patches                      */
///////////////////////////////////////////////////////

#define pause_restart_button           0x159368  // PauseLayer::customSetup
#define level_flip                     0x1496EE  // PlayLayer::toggleFlipped if(param_1)