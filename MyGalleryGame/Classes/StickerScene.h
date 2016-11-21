#ifndef __STICKER_SCENE_H__
#define __STICKER_SCENE_H__

#include "BaseScene.h"
#include "BScrollView.h"

class StickerScene: public BaseScene{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	CREATE_FUNC(StickerScene)
	void initControlButtons();
	void addAllStickersToScrollView();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
};



#endif // __STICKER_SCENE_H__
