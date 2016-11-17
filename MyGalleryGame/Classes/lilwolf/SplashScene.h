#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class SplashScene: public cocos2d::LayerColor{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	cocos2d::Size winSize;
	cocos2d::Point origin;
	float ratio;
	void gameLogic();
	void PreloadImages();
	void PreloadSounds();
	CREATE_FUNC(SplashScene)

};



#endif // __SPLASH_SCENE_H__
