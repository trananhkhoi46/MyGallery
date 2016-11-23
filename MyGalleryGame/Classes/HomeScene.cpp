#include "HomeScene.h"
#include "SettingScene.h"
#include "StickerScene.h"
#include "AlbumScene.h"

#define kTagFacebookPage 0
#define kTagMoreGame 1
#define kTagRating 2
#define kTagSetting 3
#define kTagFreePacket 4
#define kTagBundlePacket 5
#define kTagCommonPacket 6
#define kTagUncommonPacket 7
#define kTagRarePacket 8
#define kTagNewSticker 9

TTFConfig configControlButton(s_font, 65 * s_font_ratio);
TTFConfig configLabelSticker(s_font, 60 * s_font_ratio);

//---------------------------------------------------------------------Constructor methods
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

	//Init default variables
	initDefaultVariables();

	//Add background
	Sprite* background = Sprite::create(s_homescene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Init views
	initPacketButtons();
	initOtherViews();
	initSettingMenu();
	initControlButtons();

	//Set visibily for some views at first time
	setVisibilityFreePacket();

	//Handling touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HomeScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HomeScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	//Schedule game loops
	scheduleUpdate();
	schedule(schedule_selector(HomeScene::timer), 1);
	return result;
}
//---------------------------------------------------------------------End of constructor methods
//---------------------------------------------------------------------Init methods
void HomeScene::initDefaultVariables() {
	currentStickers = StickerHelper::getCurrentExistStickerNumber(true);

	timeToGetFreeStickerInSecond = UserDefault::getInstance()->getIntegerForKey(
	TIME_TO_GET_FREE_STICKER_IN_SECOND, time(nullptr));

	isMenuBarShowing = false;
	isFreePacketAvailable = time(nullptr) >= timeToGetFreeStickerInSecond;

	//Cut animation
	int frameAmount_cut = 2;
	Vector<SpriteFrame*> animFrames_cut(frameAmount_cut);
	for (int i = 0; i < frameAmount_cut; i++) {
		Rect rect = Rect(0, 0, 144, 85);
		auto frame = SpriteFrame::create(s_homescene_cut_sheet[i], rect);
		animFrames_cut.pushBack(frame);
	}
	auto animation_cut = Animation::createWithSpriteFrames(animFrames_cut,
			0.1f);
	cut_animate = Animate::create(animation_cut);
	cut_animate->retain();
}

void HomeScene::initPacketButtons() {
	//Sprite to show time to get free sticker
	spriteTimeFreeSticker = Sprite::create(
			s_homescene_sprite_time_to_get_free_sticker);
	spriteTimeFreeSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteTimeFreeSticker->setPosition(
			spriteTimeFreeSticker->getContentSize().width / 2 + 10,
			spriteTimeFreeSticker->getContentSize().height / 2 + 10);
	this->addChild(spriteTimeFreeSticker);

	labelTimeToGetFreeSticker = Label::createWithTTF(configControlButton, "",
			TextHAlignment::CENTER);
	labelTimeToGetFreeSticker->setPosition(
			Vec2(spriteTimeFreeSticker->getContentSize().width / 2,
					spriteTimeFreeSticker->getContentSize().height / 2 - 120));
	labelTimeToGetFreeSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelTimeToGetFreeSticker->setColor(Color3B::BLACK);
	spriteTimeFreeSticker->addChild(labelTimeToGetFreeSticker);
	auto scale = ScaleBy::create(0.7f, 1.1f);
	auto scale2 = scale->reverse();
	auto seq = Sequence::create(scale, scale2, nullptr);
	auto repeat = RepeatForever::create(seq);
	spriteTimeFreeSticker->runAction(repeat);

	//Add btn free packet
	btnFreePacketTop = Button::create(s_homescene_btn_free_packet_top);
	btnFreePacketTop->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(btnFreePacketTop);
	btnFreePacketBottom = Button::create(s_homescene_btn_free_packet_bottom);
	btnFreePacketBottom->setZoomScale(0);
	btnFreePacketBottom->setPressedActionEnabled(false);
	btnFreePacketBottom->setPosition(
			Vec2(winSize.width / 2, winSize.height / 2));
	btnFreePacketBottom->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::packetButtonsCallback, this));
	btnFreePacketBottom->setTag(kTagFreePacket);
	this->addChild(btnFreePacketBottom);
}

void HomeScene::initOtherViews() {
	//Add btn friend
	Button* btnFriend = Button::create(s_homescene_btn_friend);
	btnFriend->setPosition(
			Vec2(winSize.width - btnFriend->getContentSize().width / 2 - 10,
					btnFriend->getContentSize().height / 2 + 10));
	btnFriend->setTouchEnabled(true);
	btnFriend->setPressedActionEnabled(true);
	btnFriend->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::friendButtonCallback, this));
	this->addChild(btnFriend);
	Label* labelButtonFriend = Label::createWithTTF(configControlButton,
			"FRIEND", TextHAlignment::CENTER);
	labelButtonFriend->setPosition(
			Vec2(btnFriend->getPositionX(), btnFriend->getPositionY() - 55));
	labelButtonFriend->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelButtonFriend->setColor(Color3B::BLACK);
	this->addChild(labelButtonFriend);

	//Add btn trade
	Button* btnTrade = Button::create(s_homescene_btn_trade);
	btnTrade->setPosition(
			Vec2(
					winSize.width - btnTrade->getContentSize().width / 2
							- btnFriend->getContentSize().width - 20 - 10,
					btnTrade->getContentSize().height / 2 + 10));
	btnTrade->setTouchEnabled(true);
	btnTrade->setPressedActionEnabled(true);
	btnTrade->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::tradeButtonCallback, this));
	this->addChild(btnTrade);
	Label* labelButtonTrade = Label::createWithTTF(configControlButton, "TRADE",
			TextHAlignment::CENTER);
	labelButtonTrade->setPosition(
			Vec2(btnTrade->getPositionX(), btnTrade->getPositionY() - 55));
	labelButtonTrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelButtonTrade->setColor(Color3B::BLACK);
	this->addChild(labelButtonTrade);

	//Add btn rewarded ads
	btnRewardedAds = Button::create(s_homescene_btn_rewarded_ads);
	btnRewardedAds->setPosition(
			Vec2(
					winSize.width / 2
							- btnRewardedAds->getContentSize().width / 2 - 30,
					winSize.height * 0.5));
	btnRewardedAds->setTouchEnabled(true);
	btnRewardedAds->setPressedActionEnabled(true);
	btnRewardedAds->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::rewardedButtonsCallback, this));
	this->addChild(btnRewardedAds);

	//Add btn iap
	btnIAP = Button::create(s_homescene_btn_iap);
	btnIAP->setPosition(
			Vec2(winSize.width / 2 + btnIAP->getContentSize().width / 2 + 30,
					winSize.height * 0.5));
	btnIAP->setTouchEnabled(true);
	btnIAP->setPressedActionEnabled(true);
	btnIAP->addTouchEventListener(
			CC_CALLBACK_2(HomeScene::iapButtonsCallback, this));
	this->addChild(btnIAP);

	//Progress bar
	progressBar = LoadingBar::create();
	progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	progressBar->loadTexture(s_homescene_progress);
	progressBar->setPercent(currentStickers * 100 / MAX_STICKER);
	progressBar->setPosition(
			Vec2(winSize.width - progressBar->getContentSize().width / 2 - 40,
					winSize.height * 0.8));
	this->addChild(progressBar);

	Sprite* progressBackground = Sprite::create(s_homescene_bg_progress);
	progressBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	progressBackground->setPosition(progressBar->getPositionX(),
			progressBar->getPositionY());
	this->addChild(progressBackground);

	labelSticker = Label::createWithTTF(configLabelSticker,
			String::createWithFormat("%d/%d stickers", currentStickers,
			MAX_STICKER)->getCString(), TextHAlignment::CENTER);
	labelSticker->setPosition(
			Vec2(progressBackground->getPositionX(),
					progressBackground->getPositionY()));
	labelSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelSticker->setColor(Color3B::BLACK);
	this->addChild(labelSticker);

	//Add cut sprite
	cut = Sprite::create(s_homescene_cut_sheet[0]);
	cut->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	cut->setPositionX(-cut->getContentSize().width / 2);
	cut->setPositionY(winSize.height / 2 + 200);
	this->addChild(cut);

	//Add blur layer
	blurLayer = LayerColor::create(Color4B(0, 0, 0, 255));
	blurLayer->setContentSize(winSize);
	blurLayer->setPosition(Vec2::ZERO);
	blurLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
	blurLayer->setVisible(false);
	this->addChild(blurLayer);

	//Add background to blurLayer
	Sprite* background = Sprite::create(s_homescene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	background->setOpacity(123);
	blurLayer->addChild(background);

	//Add btn continue
	btnContinue = Button::create(s_homescene_btn_continue);
	btnContinue->setPosition(
			Vec2(winSize.width - btnContinue->getContentSize().width / 2 - 30,
					btnContinue->getContentSize().height / 2 + 30));
	btnContinue->setTouchEnabled(true);
	btnContinue->setPressedActionEnabled(true);
	btnContinue->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			closeBlurLayer();
		}});
	blurLayer->addChild(btnContinue);

}

void HomeScene::initSettingMenu() {
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
}
void HomeScene::initControlButtons() {

	//Add btn sticker scene
	Button* btnStickerScene = Button::create(s_homescene_btn_sticker);
	btnStickerScene->setPosition(
			Vec2(
					winSize.width - btnStickerScene->getContentSize().width / 2
							- 5,
					winSize.height
							- btnStickerScene->getContentSize().height / 2
							- 10));
	btnStickerScene->setTouchEnabled(true);
	btnStickerScene->setPressedActionEnabled(true);
	btnStickerScene->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			auto *newScene = StickerScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}});
	this->addChild(btnStickerScene);
	Label* labelButtonSticker = Label::createWithTTF(configControlButton,
			"STICKER", TextHAlignment::CENTER);
	labelButtonSticker->setPosition(
			Vec2(btnStickerScene->getPositionX() + 33,
					btnStickerScene->getPositionY()));
	labelButtonSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelButtonSticker->setColor(Color3B::BLACK);
	this->addChild(labelButtonSticker);

	//Add btn album
	Button* btnAlbumScene = Button::create(s_homescene_btn_album);
	btnAlbumScene->setPosition(
			Vec2(
					winSize.width - btnAlbumScene->getContentSize().width / 2
							- btnStickerScene->getContentSize().width - 5 - 20,
					winSize.height - btnAlbumScene->getContentSize().height / 2
							- 10));
	btnAlbumScene->setTouchEnabled(true);
	btnAlbumScene->setPressedActionEnabled(true);
	btnAlbumScene->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			auto *newScene = AlbumScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}});
	this->addChild(btnAlbumScene);
	Label* labelButtonAlbum = Label::createWithTTF(configControlButton, "ALBUM",
			TextHAlignment::CENTER);
	labelButtonAlbum->setPosition(
			Vec2(btnAlbumScene->getPositionX() + 60,
					btnAlbumScene->getPositionY() - 10));
	labelButtonAlbum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelButtonAlbum->setColor(Color3B::BLACK);
	this->addChild(labelButtonAlbum);

	//Add btn home
	Button* btnHomeScene = Button::create(s_homescene_btn_home);
	btnHomeScene->setPosition(
			Vec2(
					winSize.width - btnHomeScene->getContentSize().width / 2
							- btnAlbumScene->getContentSize().width
							- btnStickerScene->getContentSize().width - 5 - 40,
					winSize.height - btnHomeScene->getContentSize().height / 2
							- 10));
	btnHomeScene->setZoomScale(0);
	this->addChild(btnHomeScene);
	Label* labelButtonHome = Label::createWithTTF(configControlButton, "HOME",
			TextHAlignment::CENTER);
	labelButtonHome->setPosition(
			Vec2(btnHomeScene->getPositionX() + 30,
					btnHomeScene->getPositionY() - 10));
	labelButtonHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelButtonHome->setColor(Color3B::BLACK);
	this->addChild(labelButtonHome);
}
//---------------------------------------------------------------------End of init methods
//---------------------------------------------------------------------Game loop methods
void HomeScene::update(float dt) {

}

void HomeScene::timer(float interval) {
	int currentTimeInSecond = time(nullptr);
	int secondLeft = timeToGetFreeStickerInSecond - currentTimeInSecond;
	int minuteLeft = secondLeft / 60;
	secondLeft = secondLeft % 60;
	if (secondLeft >= 0) {
		labelTimeToGetFreeSticker->setString(
				String::createWithFormat("FREE in\n%d:%d", minuteLeft,
						secondLeft)->getCString());
	}

	isFreePacketAvailable = time(nullptr) >= timeToGetFreeStickerInSecond;
	if (isFreePacketAvailable) {
		setVisibilityFreePacket();
		this->unschedule(schedule_selector(HomeScene::timer));
	}
}
//---------------------------------------------------------------------End of game loop methods
//---------------------------------------------------------------------Invalidate views methods
void HomeScene::setVisibilityFreePacket() {
	btnRewardedAds->setVisible(!isFreePacketAvailable);
	btnIAP->setVisible(!isFreePacketAvailable);
	spriteTimeFreeSticker->setVisible(!isFreePacketAvailable);
	btnFreePacketBottom->setVisible(isFreePacketAvailable);
	btnFreePacketTop->setVisible(isFreePacketAvailable);
}

void HomeScene::invalidateProgressBar() {
	currentStickers = StickerHelper::getCurrentExistStickerNumber(true);
	labelSticker->setString(
			String::createWithFormat("%d/%d stickers", currentStickers,
			MAX_STICKER)->getCString());
	progressBar->setPercent(currentStickers * 100 / MAX_STICKER);
}

void HomeScene::invalidateMenuBarPosition() {
	if (menuBar->numberOfRunningActions() == 0) {
		menuBar->runAction(
				MoveTo::create(0.15f,
						isMenuBarShowing ?
								menuBarVisiblePosition :
								menuBarInvisiblePosition));
	}
}
void HomeScene::closeBlurLayer() {
	Vector<Node*> layerChildren = blurLayer->getChildren();
	for (const auto child : layerChildren) {
		if (child && child->getTag() == kTagNewSticker) {
			blurLayer->removeChild(child, false);
		}
	}
	blurLayer->setVisible(false);
}

//---------------------------------------------------------------------End of invalidate views methods
//---------------------------------------------------------------------Game logic methods
void HomeScene::earn3RandomStickers() {
	timeToGetFreeStickerInSecond = time(
			nullptr) + TIME_TO_GET_FREE_PACKET_IN_SECOND;
	UserDefault::getInstance()->setIntegerForKey(
	TIME_TO_GET_FREE_STICKER_IN_SECOND, timeToGetFreeStickerInSecond);
	isFreePacketAvailable = time(nullptr) >= timeToGetFreeStickerInSecond;
	setVisibilityFreePacket();
	schedule(schedule_selector(HomeScene::timer), 1);

	earn3Stickers(STICKER_RARITY::COMMON, true);
}

void HomeScene::earn3Stickers(STICKER_RARITY rarity, bool isRandom) {
	blurLayer->setVisible(true);

	if (isRandom) {
		string stickerIdString = "";
		for (int i = 0; i < 3; i++) {
			//Determine position of the sticker
			Vec2 position;
			if (i == 0) {
				position = Vec2(winSize.width / 3 - 40, winSize.height * 0.65);
			} else if (i == 1) {
				position = Vec2(winSize.width * 2 / 3 + 40,
						winSize.height * 0.65);
			} else {
				position = Vec2(winSize.width / 2, winSize.height * 0.3);
			}

			//Add sticker sprite
			Sticker * sticker = vt_stickers.at(
					CppUtils::randomBetween(0, vt_stickers.size() - 1));

			Sprite* stickerSprite = Sprite::create(sticker->sticker_image);
			stickerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			stickerSprite->setTag(kTagNewSticker);

			//Add sprite_new if needed
			if (!StickerHelper::isStickerHasAlreadyExisted(
					sticker->sticker_id)) {
				Sprite* newSprite = Sprite::create(s_homescene_new);
				newSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				newSprite->setPosition(
						Vec2(
								stickerSprite->getContentSize().width
										- newSprite->getContentSize().width / 2,
								stickerSprite->getContentSize().height
										- newSprite->getContentSize().height
												/ 2));
				stickerSprite->addChild(newSprite);
			}

			//Add a light below the sticker if type != common
			if (sticker->rarity != STICKER_RARITY::COMMON) {
				Sprite* lightSprite = Sprite::create(s_homescene_uncommon);
				lightSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				lightSprite->setPosition(position);
				lightSprite->setTag(kTagNewSticker); //For being removed from blurLayer later
				blurLayer->addChild(lightSprite);

				lightSprite->runAction(
						RepeatForever::create(RotateBy::create(5, 180)));
				lightSprite->runAction(
						RepeatForever::create(
								Sequence::create(ScaleTo::create(0.5, 0.9),
										ScaleTo::create(0.5, 1), nullptr)));
			}

			stickerSprite->setPosition(position);
			blurLayer->addChild(stickerSprite);

			stickerIdString += CppUtils::doubleToString(sticker->sticker_id);
			if (i < 2) {
				stickerIdString += ",";
			}
		}
		StickerHelper::saveToMyStickerList(stickerIdString);
	} else {
		switch (rarity) {
		case STICKER_RARITY::COMMON: {
			CCLog("bambi earn3Stickers common");
		}
			break;
		case STICKER_RARITY::UNCOMMON: {
			CCLog("bambi earn3Stickers uncommon");
		}
			break;
		case STICKER_RARITY::RARE: {
			CCLog("bambi earn3Stickers rare");
		}
			break;
		}
	}

	invalidateProgressBar();
}
//---------------------------------------------------------------------End of game logic methods
//---------------------------------------------------------------------Callback methods
void HomeScene::packetButtonsCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::BEGAN
			&& !blurLayer->isVisible()) {
		int animationDuration = 3;
		Button* button = dynamic_cast<Button*>(pSender);
		int tag = (int) button->getTag();
		switch (tag) {
		case kTagFreePacket: {
			cut->runAction(
					Sequence::create(
							Spawn::createWithTwoActions(
									Repeat::create(cut_animate, 20),
									MoveBy::create(2,
											Vec2(
													winSize.width
															+ cut->getContentSize().width,
													0))),
							MoveBy::create(0,
									Vec2(
											-winSize.width
													- cut->getContentSize().width,
											0)), nullptr));

			btnFreePacketTop->runAction(
					Sequence::create(DelayTime::create(0.85),
							MoveBy::create(0.2, Vec2(0, 100)),
							DelayTime::create(2),
							MoveBy::create(0, Vec2(0, -100)), nullptr));

		}
			break;
		case kTagBundlePacket: {
		}
			break;
		case kTagCommonPacket: {
		}
			break;
		case kTagUncommonPacket: {
		}
			break;
		case kTagRarePacket: {
		}
			break;

		}

		//Cut animation

		auto funcResetScheduleGetFreeSticker = CallFunc::create([=]() {
			earn3RandomStickers();
		});
		button->runAction(
				Sequence::create(DelayTime::create(animationDuration),
						funcResetScheduleGetFreeSticker, nullptr));
	}
}

void HomeScene::iapButtonsCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED
			&& !blurLayer->isVisible()) {
		SocialPlugin::showToast("Doesn't support at the moment");
	}
}
void HomeScene::rewardedButtonsCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED
			&& !blurLayer->isVisible()) {
		SocialPlugin::showToast("Doesn't support at the moment");
	}
}

void HomeScene::tradeButtonCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED
			&& !blurLayer->isVisible()) {
		SocialPlugin::showToast("Doesn't support at the moment");
	}
}

void HomeScene::friendButtonCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED
			&& !blurLayer->isVisible()) {
		SocialPlugin::showToast("Doesn't support at the moment");
	}
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

bool HomeScene::onTouchBegan(Touch* touch, Event* event) {
	Rect rect = menuBar->getBoundingBox();
	rect.setRect(rect.origin.x, rect.origin.y, rect.size.width, 200);
	if (rect.containsPoint(touch->getLocation())) {
		isMenuBarShowing = !isMenuBarShowing;
		invalidateMenuBarPosition();
	}
	return true;
}

//TODO exist game if press back twice in 2 seconds
bool firstClickInHomeScene = true;
void HomeScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (firstClickInHomeScene) {
			firstClickInHomeScene = false;
			SocialPlugin::showToast("Press back again to Exit!");

			auto func = CallFunc::create([=]() {
				firstClickInHomeScene = true;
			});
			this->runAction(
					Sequence::create(DelayTime::create(2), func, nullptr));
		} else {
			CCDirector::sharedDirector()->end();
		}
	}
}
//---------------------------------------------------------------------End of callback methods
