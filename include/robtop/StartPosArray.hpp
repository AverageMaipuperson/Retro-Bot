#pragma once
#include <cocos2d.h>
#include "GameObject.hpp"

class StartPosArray : public CCArray {
	public:
	static StartPosArray* self;
	CCArray* data;
	static StartPosArray* get() {
		if(!StartPosArray::self) {
			StartPosArray::self = new StartPosArray();
			StartPosArray::self->data = CCArray::create();
        	StartPosArray::self->data->retain(); 
		}
		return StartPosArray::self;
	}

	static void reset() {
		delete StartPosArray::self;
		StartPosArray::self = nullptr;
	}

	void insert(GameObject* object) {
		if(object == nullptr || MEMBER_BY_OFFSET(int, object, 0x2c8) != 31) return;
		StartPosArray::get()->addObject(object);
	}
};

StartPosArray* StartPosArray::self = nullptr;