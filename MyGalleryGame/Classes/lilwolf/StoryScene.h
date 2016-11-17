#ifndef __STORY_SCENE_H__
#define __STORY_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;


class StoryScene: public cocos2d::LayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static cocos2d::Scene* scene();

	CREATE_FUNC(StoryScene)

	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
    void playButton(Ref* pSender, ui::Widget::TouchEventType eEventType);
    void goToPlayScene();

	Size winSize;
	Point origin;
	bool isSound;


	int whichStoreIsShowing;
	Sprite* story2;
	Sprite* story3;
	Label* labelStory;


};

#endif // __STORY_SCENE_H__
