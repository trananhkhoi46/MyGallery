#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;


class MenuScene: public cocos2d::LayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static cocos2d::Scene* scene();

	CREATE_FUNC(MenuScene)

//	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
//
//
//	void menuCallback(Ref* sender);
//	void playMenuCallBack(Ref* sender);
//	void menuCallbackSound(Ref* sender);

	void distributeMenu(float dt);
	void menuCallbackSound(Ref* sender);
	void menuCallback(Ref* sender);
    void playButton(Ref* pSender, ui::Widget::TouchEventType eEventType);

	Size winSize;
	Point origin;

	Sprite* title;
	Button* btn_play;
	Layer* boardLayer;

	//Exit when click back 2 times
	void update(float delta);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	float time;
	bool firstClick;

};

#endif // __MENU_SCENE_H__
