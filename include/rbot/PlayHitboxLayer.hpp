#include <cocos2dcpp.h>
#include "../robtop/PlayLayer.hpp"
#include "../robtop/GameObject.hpp"
#include "mod.hpp"
#include "offsets.hpp"
#include "tools.hpp"
using namespace cocos2d;

#define MEMBER_BY_OFFSET(type, var, offset) \
    (*reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(var) + static_cast<uintptr_t>(offset)))

inline void drawRect(CCRect rect) {
    CCPoint bottomLeft = ccp(rect.origin.x + 0.75f, rect.origin.y + 0.75f);
    CCPoint topRight = ccp(rect.origin.x + rect.size.width - 0.75f, 
                           rect.origin.y + rect.size.height - 0.75f);
    
    ccDrawRect(bottomLeft, topRight);
}



class PlayHitboxLayer : public CCLayer {
public:
    PlayLayer* m_levelLayer;

    static PlayHitboxLayer* create(PlayLayer* layer) {
        PlayHitboxLayer* pRet = new PlayHitboxLayer();
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
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto masterArray = MEMBER_BY_OFFSET(CCArray*, m_levelLayer, PlayLayer__m_objects);
        if (!masterArray || masterArray->count() == 0) return;
        ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
        ccGLBindTexture2D(0);
        CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor)->use();
        glLineWidth(2.0f);

        auto cameraPos = MEMBER_BY_OFFSET(CCPoint, m_levelLayer, PlayLayer__m_cameraPos);

        CCObject* subObj = NULL;
        CCARRAY_FOREACH(masterArray, subObj) {
            auto section = (CCArray*)subObj;
            if (!section || section->count() == 0) continue;

            CCObject* gameObj = NULL;
            CCARRAY_FOREACH(section, gameObj) {
                GameObject* obj = (GameObject*)gameObj;
                if (!obj->isVisible() || obj->getParent() == nullptr) continue;
                CCPoint pos = obj->getPosition();
                int type = MEMBER_BY_OFFSET(int, obj, GameObject__m_type);
                int typeID = MEMBER_BY_OFFSET(int, obj, GameObject__m_key);
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

                auto cameraView = CCRect(cameraPos.x, cameraPos.y, winSize.width, winSize.height);
                #if GAME_VERSION > V1P3
                auto radius = MEMBER_BY_OFFSET(float, obj, GameObject__m_radius);

                #if GAME_VERSION < V1P7
                if (!CCRect::CCRectIntersectsRect(cameraView, acrect) && radius <= 0) {
                #else
                if (!cameraView.intersectsRect(acrect) && radius <= 0) {
                #endif
                    continue; 
                }

                if (radius > 0.0f) {
                    auto bounds = CCRect(pos.x - radius, pos.y - radius, radius * 2, radius * 2);
                    #if GAME_VERSION < V1P7
                    if (!CCRect::CCRectIntersectsRect(cameraView, acrect) && radius <= 0) {
                    #else
                    if (!cameraView.intersectsRect(bounds) && radius <= 0) {
                    #endif
                        continue;
                    }
                }
                #endif

                if(mod::module_by_id<bool>(id::show_hitboxes)) {
                    #if GAME_VERSION > V1P3
                    if (radius > 0.0f) {
                        ccDrawCircle(obj->getPosition(), radius, 0, 24, false);
                    } else 
                    #endif 
                    if(type == 0 || (type >= 2 && type <= 6) || (type >= 9 && type <= 20) || type == 22) drawRect(acrect);
                }

            }
        }

        auto player = getPlayer(m_levelLayer);
        if (player && mod::module_by_id<bool>(id::show_hitboxes)) {
            auto hazardRect = player->getObjectRect();
            hazardRect.origin = ccp(hazardRect.origin.x, hazardRect.origin.y);
            ccDrawColor4B(255, 255, 0, 255);
            drawRect(hazardRect);

            CCRect blockRect = player->getObjectRect(.3f, .3f);
            blockRect.origin = ccp(blockRect.origin.x, blockRect.origin.y);
            ccDrawColor4B(0, 0, 255, 255);
            drawRect(blockRect);

        }
    }
};

void drawHitboxes(PlayLayer* self) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto masterArray = MEMBER_BY_OFFSET(CCArray*, self, PlayLayer__m_objects);
    if (!masterArray || masterArray->count() == 0) return;
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
    ccGLBindTexture2D(0);
    CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor)->use();
    glLineWidth(2.0f);

    auto cameraPos = MEMBER_BY_OFFSET(CCPoint, self, PlayLayer__m_cameraPos);

    CCObject* subObj = NULL;
    CCARRAY_FOREACH(masterArray, subObj) {
        auto section = (CCArray*)subObj;
        if (!section || section->count() == 0) continue;

        CCObject* gameObj = NULL;
        CCARRAY_FOREACH(section, gameObj) {
            GameObject* obj = (GameObject*)gameObj;
            if (!obj->isVisible() || obj->getParent() == nullptr) continue;
            CCPoint pos = obj->getPosition();
            int type = MEMBER_BY_OFFSET(int, obj, GameObject__m_type);
            int typeID = MEMBER_BY_OFFSET(int, obj, GameObject__m_key);
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
            CCRect hitRect = obj->getObjectRect(obj->getScaleX(), obj->getScaleY());
            CCRect acrect;
            acrect.size = hitRect.size;
            acrect.origin = ccp(pos.x - acrect.size.width * anchor.x, pos.y - acrect.size.height * anchor.y);

            auto cameraView = CCRect(cameraPos.x, cameraPos.y, winSize.width, winSize.height);
            #if GAME_VERSION > V1P3
            auto radius = MEMBER_BY_OFFSET(float, obj, GameObject__m_radius);

            #if GAME_VERSION < V1P7
            if (!CCRect::CCRectIntersectsRect(cameraView, acrect) && radius <= 0) {
            #else
            if (!cameraView.intersectsRect(acrect) && radius <= 0) {
            #endif
                continue; 
            }

            if (radius > 0.0f) {
                auto bounds = CCRect(pos.x - radius, pos.y - radius, radius * 2, radius * 2);
                #if GAME_VERSION < V1P7
                if (!CCRect::CCRectIntersectsRect(cameraView, acrect) && radius <= 0.0f) {
                #else
                if (!cameraView.intersectsRect(bounds) && radius <= 0) {
                #endif
                    continue;
                }
            }
            #endif

            if(mod::module_by_id<bool>(id::show_hitboxes)) {
                #if GAME_VERSION > V1P3
                if (radius > 0) {
                    ccDrawCircle(obj->getPosition(), radius, 0, 24, false);
                } else 
                #endif
                if(type == 0 || (type >= 2 && type <= 6) || (type >= 9 && type <= 20) || type == 22) drawRect(acrect);
            }

        }
    }

    auto player = getPlayer(self);
    if (player && mod::module_by_id<bool>(id::show_hitboxes)) {
        auto hazardRect = player->getObjectRect();
        hazardRect.origin = ccp(hazardRect.origin.x, hazardRect.origin.y);
        ccDrawColor4B(255, 255, 0, 255);
        drawRect(hazardRect);

        CCRect blockRect = player->getObjectRect(0.3f, 0.3f);
        blockRect.origin = ccp(blockRect.origin.x, blockRect.origin.y);
        ccDrawColor4B(0, 0, 255, 255);
        drawRect(blockRect);

    }
}