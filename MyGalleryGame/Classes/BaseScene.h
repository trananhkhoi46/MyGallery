#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "Constants.h"
#include "StickerResource.h"
#include "CppUtils.h"
#include "StickerHelper.h"
#include "SocialPlugin.h"
#include "BLabel.h"
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif


using namespace cocos2d::network;
using namespace cocos2d::ui;
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

class BaseScene: public cocos2d::LayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static cocos2d::Scene* scene();

	CREATE_FUNC(BaseScene)
	void showAds();
	void cacheAds();

	bool isSound;
	bool isAlert;
	bool isMusic;
	Size winSize;
	Point origin;
};

#endif // __BASE_SCENE_H__
