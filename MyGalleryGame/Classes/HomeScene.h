#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "BaseScene.h"

class HomeScene: public BaseScene {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static cocos2d::Scene* scene();

	CREATE_FUNC(HomeScene)

	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void update(float interval);

	int currentStickers;
	int maxStickers;

	Label* labelSticker;
};

#endif // __HOME_SCENE_H__
