#include "AlbumScene.h"
#include "HomeScene.h"
#include "StickerScene.h"

Scene* AlbumScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	AlbumScene *layer = AlbumScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool AlbumScene::init() {
	bool result = BaseScene::init();
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	TTFConfig config(s_font, 120 * s_font_ratio);

	//Add background
	Sprite* background = Sprite::create(s_albumscene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Init views
	initControlButtons();

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(AlbumScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return result;
}
void AlbumScene::initControlButtons() {
	TTFConfig configControlButton(s_font, 65 * s_font_ratio);
	//Add btn sticker
	Button* btnStickerScene = Button::create(s_albumscene_btn_sticker);
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
	Button* btnAlbumScene = Button::create(s_albumscene_btn_album);
	btnAlbumScene->setPosition(
			Vec2(
					winSize.width - btnAlbumScene->getContentSize().width / 2
							- btnStickerScene->getContentSize().width - 5 - 20,
					winSize.height - btnAlbumScene->getContentSize().height / 2
							- 10));
	btnAlbumScene->setZoomScale(0);
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
	Button* btnHomeScene = Button::create(s_albumscene_btn_home);
	btnHomeScene->setPosition(
			Vec2(
					winSize.width - btnHomeScene->getContentSize().width / 2
							- btnHomeScene->getContentSize().width
							- btnAlbumScene->getContentSize().width - 5 - 40,
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
void AlbumScene::addAllStickersToScrollView() {
	//Scrollview configuration
	int numberOfItems = 20;
	float itemMargin = 125;
	Size scrollFrameSize = Size(500, 650);

	//Create scrollview
	BScrollView* scrollview = BScrollView::createVertical(numberOfItems,
			itemMargin, scrollFrameSize);
	scrollview->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	scrollview->setBackGroundColorType(Layout::BackGroundColorType::SOLID); //Background
	scrollview->setBackGroundColor(Color3B(200, 200, 200)); //Background
	this->addChild(scrollview);

	//Add sth to scroll view
	float positionX = scrollview->leftPosition;
	float positionY = scrollview->topPosition;
	for (int i = 0; i < numberOfItems; i++) {
		//Item background button
		Sprite* itemSprite = Sprite::create(s_sticker_image_1_animation.at(0));
		itemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		itemSprite->setPosition(
				Vec2(positionX + scrollFrameSize.width / 2 - itemMargin / 2,
						positionY));
		scrollview->addChild(itemSprite);

		positionY -= itemMargin;
	}
}

bool firstClickInAlbumScene = true;
void AlbumScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (firstClickInAlbumScene) {
			firstClickInAlbumScene = false;
			SocialPlugin::showToast("Press back again to Exit!");

			auto func = CallFunc::create([=]() {
				firstClickInAlbumScene = true;
			});
			this->runAction(
					Sequence::create(DelayTime::create(2), func, nullptr));
		} else {
			CCDirector::sharedDirector()->end();
		}
	}
}
