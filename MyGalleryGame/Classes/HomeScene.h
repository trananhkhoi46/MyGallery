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
	void timer(float interval);
	void invalidateMenuBarPosition();
	void settingButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void iapButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void rewardedButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);

	int currentStickers;
	int maxStickers;
	int timeToGetFreeStickerInSecond;
	bool isMenuBarShowing;
	Vec2 menuBarVisiblePosition;
	Vec2 menuBarInvisiblePosition;

	Label* labelSticker;
	Label* labelTimeToGetFreeSticker;
	Sprite* menuBar;
};

#endif // __HOME_SCENE_H__
