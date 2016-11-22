#include "StickerScene.h"
#include "HomeScene.h"
#include "AlbumScene.h"
#include <math.h>

Scene* StickerScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	StickerScene *layer = StickerScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StickerScene::init() {
	bool result = BaseScene::init();
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	TTFConfig config(s_font, 120 * s_font_ratio);

	//Add background
	Sprite* background = Sprite::create(s_stickerscene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Init views
	initControlButtons();

	//Add all stickers to scrollview
	addAllStickersToScrollView();

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(StickerScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return result;
}
void StickerScene::initControlButtons() {
	TTFConfig configControlButton(s_font, 65 * s_font_ratio);
	//Add btn sticker scene
	Button* btnStickerScene = Button::create(s_stickerscene_btn_sticker);
	btnStickerScene->setPosition(
			Vec2(
					winSize.width - btnStickerScene->getContentSize().width / 2
							- 5,
					winSize.height
							- btnStickerScene->getContentSize().height / 2
							- 10));
	btnStickerScene->setZoomScale(0);
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
	Button* btnAlbumScene = Button::create(s_stickerscene_btn_album);
	btnAlbumScene->setPosition(
			Vec2(
					winSize.width - btnAlbumScene->getContentSize().width / 2
							- 5 - 20 - btnStickerScene->getContentSize().width,
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
	Button* btnHomeScene = Button::create(s_stickerscene_btn_home);
	btnHomeScene->setPosition(
			Vec2(
					winSize.width - btnHomeScene->getContentSize().width / 2
							- btnAlbumScene->getContentSize().width
							- btnStickerScene->getContentSize().width - 5 - 40,
					winSize.height - btnHomeScene->getContentSize().height / 2
							- 10));
	btnHomeScene->setTouchEnabled(true);
	btnHomeScene->setPressedActionEnabled(true);
	btnHomeScene->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			auto *newScene = HomeScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}});
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
void StickerScene::addAllStickersToScrollView() {
	TTFConfig configStickerDetailLabel(s_font, 65 * s_font_ratio);

	//Scrollview configuration
	vector<Sticker*> vt_current_exist_sticker =
			StickerHelper::getCurrentExistSticker(true);
	int numberOfItems = vt_current_exist_sticker.size();
	int scrollviewMarginTop = 150;
	float itemMargin = 600;
	Size scrollFrameSize = Size(winSize.width,
			winSize.height - scrollviewMarginTop);

	//Create scrollview
	BScrollView* scrollview = BScrollView::createVertical(
			ceil(numberOfItems / 2.0f), itemMargin, scrollFrameSize);
	scrollview->setPosition(
			Vec2(winSize.width / 2,
					winSize.height / 2 - scrollviewMarginTop / 2));
	this->addChild(scrollview);

	//Add sth to scroll view
	float positionX = scrollview->leftPosition;
	float positionY = scrollview->topPosition + 30;
	for (int i = 0; i < numberOfItems; i++) {
		Sticker* sticker = vt_current_exist_sticker.at(i);

		//Sticker
		Sprite* itemSprite = Sprite::create(sticker->sticker_image);
		itemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		itemSprite->setPosition(
				Vec2(
						positionX
								+ scrollFrameSize.width
										* (i % 2 == 0 ? 0.28 : 0.72)
								- itemMargin / 2, positionY));
		scrollview->addChild(itemSprite);

		//Sticker detail sprite
		Sprite* itemDetailSprite = Sprite::create(
				s_stickerscene_sprite_sticker_detail);
		itemDetailSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		itemDetailSprite->setPosition(
				Vec2(itemSprite->getPositionX(),
						itemSprite->getPositionY()
								- itemSprite->getContentSize().width / 2 - 50));
		scrollview->addChild(itemDetailSprite);

		//Add number
		BLabel* labelStickerNumber =
				BLabel::createWithTTF(configStickerDetailLabel,
						String::createWithFormat("%d", sticker->sticker_id)->getCString(),
						TextHAlignment::CENTER);
		labelStickerNumber->setPosition(
				Vec2(70, itemDetailSprite->getContentSize().height / 2 - 2));
		labelStickerNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelStickerNumber->setColor(Color3B::WHITE);
		itemDetailSprite->addChild(labelStickerNumber);

		//Add sticker quantity
		int stickerQuantity = StickerHelper::getStickerQuantityInMyList(
				sticker->sticker_id);
		BLabel* labelStickerQuantity =
				BLabel::createWithTTF(configStickerDetailLabel,
						String::createWithFormat(
								stickerQuantity == 1 ?
										"1 sticker" : "%d stickers", stickerQuantity)->getCString(),
						TextHAlignment::CENTER);
		labelStickerQuantity->setPosition(
				Vec2(255, itemDetailSprite->getContentSize().height / 2 - 2));
		labelStickerQuantity->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelStickerQuantity->setColor(Color3B::BLACK);
		itemDetailSprite->addChild(labelStickerQuantity);

		//Stick sprite if needed
		if (StickerHelper::isStickerHasNotSticked(1)) {
			Sprite* stickerStick = Sprite::create(s_stickerscene_sprite_stick);
			stickerStick->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			stickerStick->setPosition(
					Vec2(0, itemSprite->getContentSize().height));
			itemSprite->addChild(stickerStick);

			stickerStick->runAction(
					RepeatForever::create(
							Sequence::create(ScaleTo::create(0.5, 0.9),
									ScaleTo::create(0.5, 1), nullptr)));
		}

		if (i % 2 == 1) {
			positionY -= itemMargin;
		}
	}
}

bool firstClickInStickerScene = true;
void StickerScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (firstClickInStickerScene) {
			firstClickInStickerScene = false;
			SocialPlugin::showToast("Press back again to Exit!");

			auto func = CallFunc::create([=]() {
				firstClickInStickerScene = true;
			});
			this->runAction(
					Sequence::create(DelayTime::create(2), func, nullptr));
		} else {
			CCDirector::sharedDirector()->end();
		}
	}
}
