#include "BaseScene.h"

Scene* BaseScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	BaseScene *layer = BaseScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
#ifdef SDKBOX_ENABLED
class IAdmobListener : public sdkbox::AdMobListener {
public:
	virtual void adViewDidReceiveAd(const std::string &name) {
		CCLog("bambi admob adViewDidReceiveAd %s: ", name.c_str());
		//sdkbox::PluginAdMob::show(name);
	}
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
		CCLog("bambi admob adViewDidFailToReceiveAdWithError %s: ", msg.c_str());
	}
	virtual void adViewWillPresentScreen(const std::string &name) {
		CCLog("bambi admob adViewWillPresentScreen %s: ", name.c_str());
	}
	virtual void adViewDidDismissScreen(const std::string &name) {
		CCLog("bambi admob adViewDidDismissScreen %s: ", name.c_str());
	}
	virtual void adViewWillDismissScreen(const std::string &name) {
		sdkbox::PluginAdMob::cache(name);
		CCLog("bambi admob adViewWillDismissScreen %s: ", name.c_str());
	}
	virtual void adViewWillLeaveApplication(const std::string &name) {
		CCLog("bambi admob adViewWillLeaveApplication %s: ", name.c_str());
	}
};
#endif
void BaseScene::cacheAds() {
	sdkbox::PluginAdMob::cache("gameover"); //Fullscreen ads
}
void BaseScene::showAds() {
	sdkbox::PluginAdMob::show("gameover"); //Fullscreen ads
}
// on "init" you need to initialize your instance
bool BaseScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	isSound = UserDefault::getInstance()->getBoolForKey(SOUND, true);
	UserDefault::getInstance()->setBoolForKey(SOUND, isSound);
	isMusic = UserDefault::getInstance()->getBoolForKey(MUSIC, true);
	UserDefault::getInstance()->setBoolForKey(MUSIC, isMusic);
	isAlert = UserDefault::getInstance()->getBoolForKey(ALERT, true);
	UserDefault::getInstance()->setBoolForKey(ALERT, isAlert);

	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();
#ifdef SDKBOX_ENABLED
	sdkbox::PluginAdMob::setListener(new IAdmobListener());
#endif
	return true;
}

