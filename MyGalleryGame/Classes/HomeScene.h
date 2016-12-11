#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "BaseScene.h"

class HomeScene: public BaseScene,
		FacebookConnectDelegate,
		FirebaseDelegate,
		FirebaseTradeFeatureDelegate,
		sdkbox::IAPListener {
private:
	static HomeScene* instance;
public:

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static cocos2d::Scene* scene();
	static HomeScene* getInstance();CREATE_FUNC(HomeScene)

	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void update(float interval);
	void timer(float interval);
	void invalidateMenuBarPosition();
	void friendButtonCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void facebookConnectButtonCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void tradeButtonCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void settingButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void iapButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void rewardedButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void packetButtonsCallback(Ref* pSender,
			ui::Widget::TouchEventType eEventType);
	void initPacketButtons();
	void initSettingMenu();
	void initControlButtons();
	void initOtherViews();
	void initDefaultVariables();
	void setVisibilityPacket();
	void earn3RandomStickers();
	void earn3Stickers(STICKER_RARITY rarity);
	void closeBlurLayer();
	void invalidateProgressBar();
	void onVideoAdsPlayed();
	void setVisibilityViewsOfTradingFeature();
	void openStickerDetailLayer(Sticker* sticker);
	void closeFriendLayer();
	void closeTradeLayer();
	void addElementsToTradeLayer();
	bool isAPacketAvailableFunc();
	Sticker* getARandomSticker(STICKER_RARITY rarity);

	int currentStickers;
	int maxStickers;
	int timeToGetFreeStickerInSecond;
	bool isMenuBarShowing;
	bool isAPacketAvailable;
	bool isRequestDone;
	bool isOpeningAnotherScene;
	bool isTradeLayerHasContent;
	bool isPacketRunningTransactionDone;
	Vec2 menuBarVisiblePosition;
	Vec2 menuBarInvisiblePosition;

	LayerColor* backgroundLayer;
	Layer* blurLayer;
	Layer* tradeLayer;
	Layer* friendLayer;
	Layer* iapLayer;
	Label* labelSticker;
	Label* labelTimeToGetFreeSticker;
	Sprite* menuBar;
	Sprite* cut;
	Sprite* spriteTimeFreeSticker;
	Animate* cut_animate;

//	Button* btnFreePacketTop;
//	Button* btnFreePacketBottom;
//	Button* btnCommonPacketTop;
//	Button* btnCommonPacketBottom;
//	Button* btnUncommonPacketTop;
//	Button* btnUncommonPacketBottom;
//	Button* btnRarePacketTop;
//	Button* btnRarePacketBottom;
//	Button* btnBundlePacketTop;
//	Button* btnBundlePacketBottom;

	Button* btnRewardedAds;
	Button* btnIAP;
	Button* btnContinue;
	Button* btnFacebookConnect;
	Button* btnFriend;
	Button* btnTrade;
	LoadingBar* progressBar;
	vector<BUserInfor*> vt_Friends;
	vector<PendingRequest*> vtPendingRequest;
	vector<PendingRequest*> vtGivenSticker;
	vector<Button*> vtButtonTopPackets;
	vector<Button*> vtButtonBottomPackets;

	//FacebookConnectDelegate response
	virtual void responseWhenLoginOrLogoutFacebook();
	//FirebaseDelegate response
	virtual void responseForQuerryTopFriend(vector<BUserInfor*> friendList);
	virtual void responseAfterCheckFacebookIdExistOnFirebase();
	virtual void responseAfterGetStickersDataFromFirebase(string facebookId,
			string stickerData, string stickedData);
	virtual void responseAfterAskingSticker(int stickerId, bool isSuccess); //From FirebaseTradeFeatureDelegate
	virtual void responseAfterCheckingPendingRequest(
			vector<PendingRequest*> vtPendingRequest); //From FirebaseTradeFeatureDelegate
	virtual void responseAfterCheckingGivenSticker(
			vector<PendingRequest*> vtGivenStickers); //From FirebaseTradeFeatureDelegate
	virtual void responseAfterDenyingRequest(bool isSuccess); //From FirebaseTradeFeatureDelegate
	virtual void responseAfterAcceptingRequest(bool isSuccess); //From FirebaseTradeFeatureDelegate

	//IAP listener
	virtual void onInitialized(bool success);
	virtual void onSuccess(const sdkbox::Product& p);
	virtual void onFailure(const sdkbox::Product& p, const std::string& msg);
	virtual void onCanceled(const sdkbox::Product& p);
	virtual void onRestored(const sdkbox::Product& p);
	virtual void onProductRequestSuccess(const std::vector<sdkbox::Product>& products);
	virtual void onProductRequestFailure(const std::string& msg);
	virtual void onRestoreComplete(bool ok, const std::string &msg);
};

#endif // __HOME_SCENE_H__
