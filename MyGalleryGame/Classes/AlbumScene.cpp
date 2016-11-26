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

	//Init default variables
	menuBarVisiblePosition = Vec2(winSize.width / 2, 148);
	menuBarInvisiblePosition = Vec2(winSize.width / 2, -28);
	isMenuBarShowing = false;

	//Add background
	Sprite* background = Sprite::create(s_albumscene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Init views
	initPageView();
	initControlButtons();

	//Handling touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AlbumScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(AlbumScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return result;
}

void AlbumScene::initPageView() {
	TTFConfig configPageLabel(s_font, 90 * s_font_ratio);
	pageView = PageView::create();
	pageView->setContentSize(Size(winSize.width, winSize.height));
	pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pageView->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(pageView);

	for (int i = 0; i < vt_sticker_pages.size(); i++) {
		StickerPage* stickerPage = vt_sticker_pages.at(i);
		Layout* page = Layout::create();
		page->setContentSize(pageView->getContentSize());

		//Background
		Sprite* background = Sprite::create(stickerPage->background_image);
		background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		background->setPosition(pageView->getContentSize().width / 2,
				pageView->getContentSize().height / 2);
		page->addChild(background);

		//Sprite label
		Sprite* spriteLabel = Sprite::create(s_albumscene_sprite_label);
		spriteLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		spriteLabel->setPosition(pageView->getContentSize().width / 2,
				pageView->getContentSize().height - 300);
		page->addChild(spriteLabel);

		//Label page name
		Label* labelButtonSticker = Label::createWithTTF(configPageLabel,
				stickerPage->sticker_page_name.c_str(), TextHAlignment::CENTER);
		labelButtonSticker->setPosition(
				Vec2(spriteLabel->getContentSize().width / 2,
						spriteLabel->getContentSize().height / 2 + 5));
		labelButtonSticker->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelButtonSticker->setColor(Color3B::BLACK);
		spriteLabel->addChild(labelButtonSticker);

		//Show sticker in the page
		vector<Sticker*> vt_sticker_of_this_page =
				StickerHelper::getStickerOfPage(stickerPage);
		vector<Sticker*> vt_available_sticker_to_be_glued_of_this_page =
				StickerHelper::getStickerAvailableToGlueOfPage(stickerPage);

		for (Sticker* sticker : vt_sticker_of_this_page) {
			Sprite* stickerHolder = Sprite::create(sticker->sticker_image);
			stickerHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			stickerHolder->setPosition(sticker->position);
			stickerHolder->setColor(Color3B(0, 0, 0));
			page->addChild(stickerHolder);
		}

		int index = 0;
		for (Sticker* sticker : vt_available_sticker_to_be_glued_of_this_page) {
			Sprite* stickerSprite = Sprite::create(sticker->sticker_image);
			stickerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			stickerSprite->setPosition(100 + 50 * index,
					stickerSprite->getContentSize().height / 2 - 20);
			page->addChild(stickerSprite);

			index++;
		}

		pageView->insertPage(page, i);
	}

	//Btn menu bar bottom
	btnMenuBar = Button::create(s_albumscene_menu_bottom);
	btnMenuBar->setPosition(
			isMenuBarShowing ?
					menuBarVisiblePosition : menuBarInvisiblePosition);
	btnMenuBar->setTouchEnabled(true);
	btnMenuBar->setPressedActionEnabled(true);
	btnMenuBar->setZoomScale(0);
	btnMenuBar->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			isMenuBarShowing = !isMenuBarShowing;
			setVisibilityMenuBar();
		}});
	this->addChild(btnMenuBar);

	spriteArrowUpDown = Sprite::create(s_albumscene_sprite_arrow);
	spriteArrowUpDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteArrowUpDown->setPosition(Vec2(1000, 225));
	btnMenuBar->addChild(spriteArrowUpDown);

	setVisibilityMenuBar();

	pageView->addEventListener(
			[](Ref* sender, PageView::EventType type) {
				if(type == PageView::EventType::TURNING)
				{
					PageView* pageViewInCallback = dynamic_cast<PageView*>(sender);
					CCLog("bambi pageview index: %d", pageViewInCallback->getCurrentPageIndex());
				}
			});
}

void AlbumScene::setVisibilityMenuBar() {
	CCLog("bambi setVisibilityMenuBar, %s",
			isMenuBarShowing ? "visible" : "invisible");
	if (btnMenuBar->numberOfRunningActions() == 0
			&& spriteArrowUpDown->numberOfRunningActions() == 0) {
		btnMenuBar->runAction(
				MoveTo::create(0.3,
						isMenuBarShowing ?
								menuBarVisiblePosition :
								menuBarInvisiblePosition));
		spriteArrowUpDown->runAction(
				RotateTo::create(0.3, isMenuBarShowing ? 0 : 180));
	}
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

bool AlbumScene::onTouchBegan(Touch* touch, Event* event) {
	return true;
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
