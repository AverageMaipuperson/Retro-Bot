///////////////////////////////////////////////////////
/*                     PlayLayer                     */
///////////////////////////////////////////////////////

#define PlayLayer__m_hazards              0x190  // PlayLayer::checkCollisions
#define PlayLayer__m_isPractice           0x24d  // PlayLayer::getPracticeMode
#define PlayLayer__m_playerObject         0x224  // PlayLayer::getPlayer
#define PlayLayer__m_cameraPos            0x22c  // PlayLayer::getCameraPos
#define PlayLayer__m_realPosition         0x1e8  // PlayLayer::update (first cocos2d::CCPoint::operator= call)
#define PlayLayer__m_objects              0x18c  // PlayLayer::init (first CCArray::create call after setSecondColor)
#define PlayLayer__m_gameLayer            0x258  // PlayLayer::getGameLayer

///////////////////////////////////////////////////////
/*                    PlayerObject                   */
///////////////////////////////////////////////////////

#define PlayerObject__m_gravity           0x350  // PlayerObject::gravityUp
#define PlayerObject__m_yVelocity         0x370  // PlayerObject::hitGround (first line)

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

#define GameObject__m_type                0x2b8  // GameObject::getType
#define GameObject__m_key                 0x310  // GameObject::getObjectKey

///////////////////////////////////////////////////////
/*                      Patches                      */
///////////////////////////////////////////////////////

#define pause_restart_button           0x14D6DA  // PauseLayer::customSetup