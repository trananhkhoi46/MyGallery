#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "BaseScene.h"

class HomeScene: public BaseScene,
		FacebookConnectDelegate,
		FirebaseDelegate,
		FirebaseTradeFeatureDelegate {
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
	void setVisibilityFreePacket();
	void earn3RandomStickers();
	void earn3Stickers(STICKER_RARITY rarity);
	void closeBlurLayer();
	void invalidateProgressBar();
	void onVideoAdsPlayed();
	void setVisibilityViewsOfTradingFeature();
	void openStickerDetailLayer(Sticker* sticker);
	void closeFriendLayer();

	int currentStickers;
	int maxStickers;
	int timeToGetFreeStickerInSecond;
	bool isMenuBarShowing;
	bool isFreePacketAvailable;
	bool isRequestDone;
	bool isOpeningAnotherScene;
	Vec2 menuBarVisiblePosition;
	Vec2 menuBarInvisiblePosition;

	LayerColor* backgroundLayer;
	Layer* blurLayer;
	Layer* friendLayer;
	Label* labelSticker;
	Label* labelTimeToGetFreeSticker;
	Sprite* menuBar;
	Sprite* cut;
	Sprite* spriteTimeFreeSticker;
	Animate* cut_animate;
	Button* btnFreePacketTop;
	Button* btnFreePacketBottom;
	Button* btnCommonPacketTop;
	Button* btnCommonPacketBottom;
	Button* btnUncommonPacketTop;
	Button* btnUncommonPacketBottom;
	Button* btnRarePacketTop;
	Button* btnRarePacketBottom;
	Button* btnBundlePacketTop;
	Button* btnBundlePacketBottom;
	Button* btnRewardedAds;
	Button* btnIAP;
	Button* btnContinue;
	Button* btnFacebookConnect;
	Button* btnFriend;
	Button* btnTrade;
	LoadingBar* progressBar;
	vector<BUserInfor*> vt_Friends;

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
};

#endif // __HOME_SCENE_H__
