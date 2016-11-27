#ifndef __ALBUM_SCENE_H__
#define __ALBUM_SCENE_H__

#include "BaseScene.h"
#include "BScrollView.h"

class AlbumScene: public BaseScene{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	CREATE_FUNC(AlbumScene)
	void initControlButtons();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void initPageView();
	void initScrollView();
	void setVisibilityMenuBar();
	void getTouchingSprite(Vec2 touchingLocation);
	void tryToGlueSticker();
	void scrollToPageIndex(int index);
	void setVisibilityBtnLeftRight();
	void timer(float interval);

	int currentPage;
	bool isMenuBarShowing;
	Vec2 menuBarVisiblePosition;
	Vec2 menuBarInvisiblePosition;
	Vec2 touchingOffset;
	PageView* pageView;
	BScrollView* scrollview;
	Sprite* spriteArrowUpDown;
	Button* btnMenuBarTop;
	Sprite* spriteMenuBarBottom;
	vector<vector<Sprite*>> vtStickerSpritesAllPages;
	vector<vector<Sprite*>> vtStickerSpritesHolderAllPages;
	vector<Button*> vtPagesIconButtons;
	Sprite* touchingSprite;
	Button* btnLeft;
	Button* btnRight;
};



#endif // __ALBUM_SCENE_H__
