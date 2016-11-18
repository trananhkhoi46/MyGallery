#include "HomeScene.h"
#include "SettingScene.h"

#define kTagFacebookPage 0
#define kTagMoreGame 1
#define kTagRating 2
#define kTagSetting 3

Scene* HomeScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	HomeScene *layer = HomeScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HomeScene::init() {
	bool result = BaseScene::init();

	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	isMenuBarShowing = false;

	//Add background
	Sprite* background = Sprite::create(s_homescene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Add menu bar
	menuBar = Sprite::create(s_homescene_menu_bar);
	menuBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(menuBar);

	//Set menubar position
	menuBarVisiblePosition = Vec2(5 + menuBar->getContentSize().width / 2,
			winSize.height - menuBar->getContentSize().height / 2);
	menuBarInvisiblePosition = Vec2(5 + menuBar->getContentSize().width / 2,
			winSize.height + menuBar->getContentSize().height / 2 - 170);
	menuBar->setPosition(menuBarVisiblePosition);

	//Add btn setting
	Button* btnSetting = Button::create(s_homescene_btn_setting);
	btnSetting->setPosition(
			Vec2(menuBar->getContentSize().width / 2,
					menuBar->getContentSize().height * 0.3));
	btnSetting->setTouchEnabled(true);
	btnSetting->setPressedActionEnabled(true);
	btnSetting->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::settingButtonsCallback, this));
	btnSetting->setTag(kTagSetting);
	menuBar->addChild(btnSetting);
	HomeScene::invalidateMenuBarPosition();

	//Add btn rating
	Button* btnRating = Button::create(s_homescene_btn_rating);
	btnRating->setPosition(
			Vec2(menuBar->getContentSize().width / 2,
					menuBar->getContentSize().height * 0.5));
	btnRating->setTouchEnabled(true);
	btnRating->setPressedActionEnabled(true);
	btnRating->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::settingButtonsCallback, this));
	btnRating->setTag(kTagRating);
	menuBar->addChild(btnRating);

	//Add btn more game
	Button* btnMoreGame = Button::create(s_homescene_btn_more_game);
	btnMoreGame->setPosition(
			Vec2(menuBar->getContentSize().width / 2,
					menuBar->getContentSize().height * 0.7));
	btnMoreGame->setTouchEnabled(true);
	btnMoreGame->setPressedActionEnabled(true);
	btnMoreGame->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::settingButtonsCallback, this));
	btnMoreGame->setTag(kTagMoreGame);
	menuBar->addChild(btnMoreGame);

	//Add btn rating
	Button* btnFacebookPage = Button::create(s_homescene_btn_facebook_page);
	btnFacebookPage->setPosition(
			Vec2(menuBar->getContentSize().width / 2,
					menuBar->getContentSize().height * 0.9));
	btnFacebookPage->setTouchEnabled(true);
	btnFacebookPage->setPressedActionEnabled(true);
	btnFacebookPage->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::settingButtonsCallback, this));
	btnFacebookPage->setTag(kTagFacebookPage);
	menuBar->addChild(btnFacebookPage);

	//Add btn sticker
	Button* btnSticker = Button::create(s_homescene_btn_sticker);
	btnSticker->setPosition(
			Vec2(winSize.width - btnSticker->getContentSize().width / 2 - 5,
					winSize.height - btnSticker->getContentSize().height / 2
							- 10));
	btnSticker->setTouchEnabled(true);
	btnSticker->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnSticker);

	//Add btn album
	Button* btnAlbum = Button::create(s_homescene_btn_album);
	btnAlbum->setPosition(
			Vec2(
					winSize.width - btnAlbum->getContentSize().width / 2
							- btnSticker->getContentSize().width - 5 - 20,
					winSize.height - btnAlbum->getContentSize().height / 2
							- 10));
	btnAlbum->setTouchEnabled(true);
	btnAlbum->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnAlbum);

	//Add btn home
	Button* btnHome = Button::create(s_homescene_btn_home);
	btnHome->setPosition(
			Vec2(
					winSize.width - btnHome->getContentSize().width / 2
							- btnAlbum->getContentSize().width
							- btnSticker->getContentSize().width - 5 - 40,
					winSize.height - btnHome->getContentSize().height / 2
							- 10));
	btnHome->setTouchEnabled(true);
	btnHome->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnHome);

	//Add btn friend
	Button* btnFriend = Button::create(s_homescene_btn_friend);
	btnFriend->setPosition(
			Vec2(winSize.width - btnFriend->getContentSize().width / 2 - 10,
					btnFriend->getContentSize().height / 2 + 10));
	btnFriend->setTouchEnabled(true);
	btnFriend->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnFriend);

	//Add btn trade
	Button* btnTrade = Button::create(s_homescene_btn_trade);
	btnTrade->setPosition(
			Vec2(
					winSize.width - btnTrade->getContentSize().width / 2
							- btnFriend->getContentSize().width - 20 - 10,
					btnTrade->getContentSize().height / 2 + 10));
	btnTrade->setTouchEnabled(true);
	btnTrade->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnTrade);

	//Add btn rewarded ads
	Button* btnRewardedAds = Button::create(s_homescene_btn_rewarded_ads);
	btnRewardedAds->setPosition(
			Vec2(
					winSize.width / 2
							- btnRewardedAds->getContentSize().width / 2 - 30,
					winSize.height * 0.5));
	btnRewardedAds->setTouchEnabled(true);
	btnRewardedAds->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnRewardedAds);

	//Add btn iap
	Button* btnIAP = Button::create(s_homescene_btn_iap);
	btnIAP->setPosition(
			Vec2(winSize.width / 2 + btnIAP->getContentSize().width / 2 + 30,
					winSize.height * 0.5));
	btnIAP->setTouchEnabled(true);
	btnIAP->setPressedActionEnabled(true);
	//	btnSetting->addTouchEventListener(CC_CALLBACK_2(HomeScene::playButton, this));
	this->addChild(btnIAP);

	//Sprite below iap to show time to get IAP
	Sprite* spriteTimeIAP = Sprite::create(
			s_homescene_sprite_time_to_get_iap_sticker);
	spriteTimeIAP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteTimeIAP->setPosition(btnIAP->getPositionX() - 30,
			btnIAP->getPositionY() - btnIAP->getContentSize().height / 2 - 50);
	this->addChild(spriteTimeIAP);

	//Sprite to show time to get free sticker
	Sprite* spriteTimeFreeSticker = Sprite::create(
			s_homescene_sprite_time_to_get_free_sticker);
	spriteTimeFreeSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteTimeFreeSticker->setPosition(
			spriteTimeFreeSticker->getContentSize().width / 2 + 10,
			spriteTimeFreeSticker->getContentSize().height / 2 + 10);
	this->addChild(spriteTimeFreeSticker);

	//Progress bar
	LoadingBar* loadingBar = LoadingBar::create();
	loadingBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	loadingBar->loadTexture(s_homescene_progress);
	loadingBar->setPercent(30);
	loadingBar->setPosition(
			Vec2(winSize.width - loadingBar->getContentSize().width / 2 - 40,
					winSize.height * 0.8));
	this->addChild(loadingBar);

	Sprite* progressBackground = Sprite::create(s_homescene_bg_progress);
	progressBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	progressBackground->setPosition(loadingBar->getPositionX(),
			loadingBar->getPositionY());
	this->addChild(progressBackground);

	//Handling touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HomeScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased =
			CC_CALLBACK_2(HomeScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	scheduleUpdate();

	return result;
}

void HomeScene::settingButtonsCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		int tag = (int) dynamic_cast<Button*>(pSender)->getTag();
		switch (tag) {
		case kTagFacebookPage:
			Application::getInstance()->openURL(s_linkToFacebookPage);
			break;
		case kTagMoreGame:
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			Application::getInstance()->openURL(s_linkToAppStoreMoreGame);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			Application::getInstance()->openURL(s_linkToGooglePlayMoreGame);
#endif
			break;
		case kTagRating:
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			Application::getInstance()->openURL(s_linkToAppStoreRating);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			Application::getInstance()->openURL(s_linkToGooglePlayRating);
#endif
			break;
		case kTagSetting: {
			auto *newScene = SettingScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}
			break;
		default:
			CCLOG("Error in Menu inflatten");
			break;
		}

	}
}

void HomeScene::invalidateMenuBarPosition() {
	if (menuBar -> numberOfRunningActions() == 0) {
	menuBar->runAction(
			MoveTo::create(0.5f,
					isMenuBarShowing ?
							menuBarVisiblePosition : menuBarInvisiblePosition));
	}
}

void HomeScene::update(float dt) {

}
bool HomeScene::onTouchBegan(Touch* touch, Event* event) {
	Rect rect = menuBar->getBoundingBox();
	rect.setRect(rect.origin.x, rect.origin.y, rect.size.width, 200);
	if (rect.containsPoint(touch->getLocation())) {
		isMenuBarShowing = !isMenuBarShowing;
		invalidateMenuBarPosition();
	}
	return true;
}
void HomeScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
	}
}

