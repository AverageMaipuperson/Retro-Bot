///////////////////////////////////////////////////////
/*                     PlayLayer                     */
///////////////////////////////////////////////////////

#define PlayLayer__m_hazards              0x164  // PlayLayer::checkCollisions
#define PlayLayer__m_isPractice           0x27d  // PlayLayer::getPracticeMode
#define PlayLayer__m_playerObject         0x268  // PlayLayer::getPlayer
#define PlayLayer__m_cameraPos            0x274  // PlayLayer::getCameraPos
#define PlayLayer__m_objects              0x160  // PlayLayer::init (first CCArray::create call after updateGlowColor)
#define PlayLayer__m_gameLayer            0x28c  // PlayLayer::getGameLayer

///////////////////////////////////////////////////////
/*                    PlayerObject                   */
///////////////////////////////////////////////////////

#define PlayerObject__m_gravity           0x300  // PlayerObject::gravityUp
#define PlayerObject__m_yVelocity         0x328  // PlayerObject::hitGround (first line)

///////////////////////////////////////////////////////
/*                  CCTextInputNode                  */
///////////////////////////////////////////////////////

#define CCTextInputNode__m_charLimit      0x140  // CCTextInputNode::getCharLimit
#define CCTextInputNode__m_inputDelegate  0x13c  // CCTextInputNode::getDelegate
#define CCTextInputNode__m_textField      0x138  // CCTextInputNode::getTextField
#define CCTextInputNode__m_allowedChars   0x120  // CCTextInputNode::setAllowedChars

///////////////////////////////////////////////////////
/*                     GameObject                    */
///////////////////////////////////////////////////////

#define GameObject__m_radius              0x230  // GameObject::getRadius
#define GameObject__m_type                0x244  // GameObject::getType
#define GameObject__m_key                 0x290  // GameObject::getObjectKey

///////////////////////////////////////////////////////
/*                      Patches                      */
///////////////////////////////////////////////////////

#define pause_restart_button           0x1DFA6C  // PauseLayer::customSetup