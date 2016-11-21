#include "StickerScene.h"
#include "HomeScene.h"
#include "AlbumScene.h"

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
	//Add btn album
	Button* btnAlbumScene = Button::create(s_stickerscene_btn_album);
	btnAlbumScene->setPosition(
			Vec2(
					winSize.width - btnAlbumScene->getContentSize().width / 2
							 - 5,
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
							- btnAlbumScene->getContentSize().width - 5 - 20,
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
	//Scrollview configuration
	int numberOfItems = 20;
	int scrollviewMarginTop = 140;
	float itemMargin = 600;
	Size scrollFrameSize = Size(winSize.width, winSize.height - scrollviewMarginTop);

	//Create scrollview
	BScrollView* scrollview = BScrollView::createVertical(numberOfItems,itemMargin,scrollFrameSize);
	scrollview->setPosition(
			Vec2(winSize.width / 2,winSize.height / 2 - scrollviewMarginTop / 2));
	this->addChild(scrollview);

	//Add sth to scroll view
	float positionX = scrollview->leftPosition;
	float positionY = scrollview->topPosition;
	for (int i = 0; i < numberOfItems; i++) {
		//Item background button
		Sprite* itemSprite = Sprite::create(s_sticker_image_1_animation.at(0));
		itemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		itemSprite->setPosition(Vec2(positionX + scrollFrameSize.width / 2 - itemMargin/2,positionY));
		scrollview->addChild(itemSprite);

		positionY -= itemMargin;
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
