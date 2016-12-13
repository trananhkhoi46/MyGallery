#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "SimpleAudioEngine.h"
#include "Constants/Resources.h"
#include "Constants/Constants.h"
#include "Constants/StickerResource.h"
#include "Helper/CppUtils.h"
#include "Helper/StickerHelper.h"
#include "Helper/SocialPlugin.h"
#include "View/BLabel.h"
#include "View/BScrollView.h"
#include "Trading/FacebookHandler/FacebookHandler.h"
#include "Trading/FirebaseHandler/FirebaseHandler.h"
#include "Trading/Model/BUserInfor.h"
#include "Helper/CustomDirector.h"
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#include "PluginChartboost/PluginChartboost.h"
#include "PluginIAP/PluginIAP.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginVungle/PluginVungle.h"
#endif
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
	void showFullscreenAds();
	void showRewardedAds();
	void onVideoAdsPlayed();
    void setListenerNull();
	bool isRewardedAdsAvailable();

	bool isHomeScene;
	bool isChartboostAdsAvailable;
	bool isVungleAdsAvailable;
	bool isSound;
	bool isAlert;
	bool isMusic;
	Size winSize;
	Point origin;
};

#endif // __BASE_SCENE_H__
