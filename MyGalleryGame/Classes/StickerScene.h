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
	static cocos2d::Scene* scene(int searchType);
	CREATE_FUNC(StickerScene)
	void initControlButtons();
	void initMenuBottom();
	void addAllStickersToScrollView();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void openStickerDetailLayer(Sticker* sticker);

	LayerColor* backgroundLayer;
	BScrollView* scrollview;
};



#endif // __STICKER_SCENE_H__
