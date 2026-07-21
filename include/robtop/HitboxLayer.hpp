#include <cocos2d.h>
#include "LevelEditorLayer.hpp"
#include "GameObject.hpp"
#include "ModLayer.hpp"
using namespace Modules;
using namespace cocos2d;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

inline void drawRect(CCRect rect) {
    CCPoint bottomLeft = ccp(rect.origin.x + 0.75f, rect.origin.y + 0.75f);
    CCPoint topRight = ccp(rect.origin.x + rect.size.width - 0.75f, 
                           rect.origin.y + rect.size.height - 0.75f);
    
    ccDrawRect(bottomLeft, topRight);
}



class HitboxLayer : public cocos2d::CCLayer {
public:
    LevelEditorLayer* m_levelLayer;

    static HitboxLayer* create(LevelEditorLayer* layer) {
        HitboxLayer* pRet = new HitboxLayer();
        if (pRet && pRet->init()) {
            pRet->m_levelLayer = layer;
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }

virtual void draw() {
    if (!m_levelLayer) return;

    CCArray* masterArray = *(CCArray**)((char*)m_levelLayer + 0x144);
    if (!masterArray || masterArray->count() == 0) return;
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
    ccGLBindTexture2D(0);
    CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor)->use();
    glLineWidth(2.0f);
    /* kmGLPushMatrix();
    kmGLPopMatrix(); */

    CCObject* subObj = NULL;
    CCARRAY_FOREACH(masterArray, subObj) {
        CCArray* section = (CCArray*)subObj;
        if (!section || section->count() == 0) continue;

        CCObject* gameObj = NULL;
        CCARRAY_FOREACH(section, gameObj) {
            GameObject* obj = (GameObject*)gameObj;
            if (!obj->isVisible() || obj->getParent() == nullptr) continue;
            kmGLPushMatrix();
            // kmGLTranslatef(pos.x, pos.y, 0);
			CCPoint pos = obj->getPosition();
			int type = MEMBER_BY_OFFSET(int, obj, 0x2c8);
			float duration = MEMBER_BY_OFFSET(float, obj, 0x324);
			int typeID = MEMBER_BY_OFFSET(int, obj, 0x32c);
			if (typeID == 29 || typeID == 30 || typeID == 104 || typeID == 105) {
    			ccDrawColor4B(255, 255, 0, 255); 

    			float radians = -CC_DEGREES_TO_RADIANS(obj->getRotation());
    			CCPoint endPos = ccp(
        			pos.x + cosf(radians) * (duration * 311.58f),
        			pos.y + sinf(radians) * (duration * 311.58f)
    			);

    			if(MODULE_BY_ID(b, Editor::TRIGGER_LENGTH)) ccDrawLine(pos, endPos);
			}
            switch(type) {
                case 2:
                ccDrawColor4B(255, 0, 0, 255);
                break;
                case 0:
                case 22:
                ccDrawColor4B(0, 0, 255, 255);
                break;
                default:
                ccDrawColor4B(0, 255, 0, 255);
            }
            float rot = obj->getRotation();
            CCSize size = obj->getContentSize();
            CCPoint anchor = obj->getAnchorPoint();
            float width = size.width;
            float height = size.height;
            int angle = (int)rot % 360;
            if (angle == 90 || angle == 270 || angle == -90 || angle == -270) {
                float temp = width;
                width = height;
                height = temp;
            }
            float halfW = width / 2.0f;
            float halfH = height / 2.0f;

            CCPoint bottomLeft = ccp(pos.x - halfW, pos.y - halfH);
            CCPoint topRight = ccp(pos.x + halfW, pos.y + halfH);
            CCRect hitRect = obj->getObjectRect();
            CCRect acrect;
            acrect.size = hitRect.size;
            acrect.origin = ccp(pos.x - acrect.size.width * anchor.x, pos.y - acrect.size.height * anchor.y);
            
            float radius = MEMBER_BY_OFFSET(float, obj, 0x2b4);
			if(MODULE_BY_ID(b, Editor::EDITOR_HITBOXES)) {
            	if (radius > 0.0f) {
                	ccDrawCircle(obj->getPosition(), radius, 0, 24, false);
            	} else if(type == 0 || (type >= 2 && type <= 6) || (type >= 9 && type <= 20) || type == 22) drawRect(acrect);
			}

			kmGLPopMatrix();
        }
	}
}






};
