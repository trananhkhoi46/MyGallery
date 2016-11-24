#include "BaseScene.h"
#include "HomeScene.h"

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
class IChartboostListener : public sdkbox::ChartboostListener {
public:
public:
	virtual void onChartboostCached(const std::string& name)
	{
		CCLog("bambi admob onChartboostCached %s: ", name.c_str());
	}
	virtual bool onChartboostShouldDisplay(const std::string& name)
	{
		CCLog("bambi admob onChartboostShouldDisplay %s: ", name.c_str());
	}
	virtual void onChartboostDisplay(const std::string& name)
	{
		CCLog("bambi admob onChartboostDisplay %s: ", name.c_str());
	}
	virtual void onChartboostDismiss(const std::string& name)
	{
		CCLog("bambi admob onChartboostDismiss %s: ", name.c_str());
	}
	virtual void onChartboostClose(const std::string& name)
	{
		CCLog("bambi admob onChartboostClose %s: ", name.c_str());
	}
	virtual void onChartboostClick(const std::string& name)
	{
		CCLog("bambi admob onChartboostClick %s: ", name.c_str());
	}
	virtual void onChartboostReward(const std::string& name, int reward)
	{
		CCLog("bambi admob onChartboostReward %s: ", name.c_str());

		//TODO return to the listener
		HomeScene::getInstance()->onVideoAdsPlayed();
	}
	virtual void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e)
	{
		CCLog("bambi admob onChartboostFailedToLoad %s: ", name.c_str());
	}
	virtual void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e)
	{
		CCLog("bambi admob onChartboostFailToRecordClick %s: ", name.c_str());
	}
	virtual void onChartboostConfirmation()
	{
		CCLog("bambi admob onChartboostConfirmation");
	}
	virtual void onChartboostCompleteStore()
	{
		CCLog("bambi admob onChartboostCompleteStore");
	}
};
#endif
void BaseScene::showFullscreenAds() {
#ifdef SDKBOX_ENABLED
	if(CppUtils::randomBetween(1,2) == 1)
	{
		sdkbox::PluginChartboost::show(kChartboostInstitialAds);
	} else {
		sdkbox::PluginAdMob::show(kAdmobInstitialAds);
	}
#endif
}
void BaseScene::showRewardedAds() {
#ifdef SDKBOX_ENABLED
	sdkbox::PluginChartboost::show(kChartboostRewardedAds);
#endif
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
	sdkbox::PluginChartboost::setListener(new IChartboostListener());
#endif
	return true;
}

