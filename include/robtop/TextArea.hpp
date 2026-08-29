#include "../cocos2dcpp.h"

class TextArea : public CCSprite {
public:
    static TextArea* create(char const*, float, int, CCPoint, char const*, float);
    void setString(char const*);
};