#include "StickerScene.h"
#include "HomeScene.h"
#include "AlbumScene.h"
#include <math.h>

#define SEARCH_TYPE_ALL 0
#define SEARCH_TYPE_STICK 1
#define SEARCH_TYPE_SELL 2
Scene* StickerScene::scene() {
	return scene(SEARCH_TYPE_ALL);
}

int searchingType;
vector<Sticker*> vt_stickers_searching;
Scene* StickerScene::scene(int searchType) {
	searchingType = searchType;
	if (searchType == SEARCH_TYPE_ALL) //Type search all
	{
		vt_stickers_searching = StickerHelper::getCurrentExistSticker(true);
	} else if (searchType == SEARCH_TYPE_STICK) //Type search stick
	{
		vt_stickers_searching = StickerHelper::getCurrentExistSticker(true,
				true);
	} else if (searchType == SEARCH_TYPE_SELL) //Type sell
	{
		vt_stickers_searching.clear();
		vector<Sticker*> vt_stickers_temp =
				StickerHelper::getCurrentExistSticker(true);
		for (Sticker* sticker : vt_stickers_temp) {
			if (StickerHelper::isStickerAbleToSell(sticker->sticker_id)) {
				CCLog("bambi StickerScene -> Sell mode -> pushback: %d", sticker->sticker_id);
				vt_stickers_searching.push_back(sticker);
			}
		}
	}

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
	initMenuBottom();

	//Add all stickers to scrollview
	addAllStickersToScrollView();

	//Handling touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(StickerScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(StickerScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);
	return result;
}
void StickerScene::initMenuBottom() {
	//Menu bottom background
	Sprite* spriteMenuBottom = Sprite::create(s_stickerscene_menu_bottom);
	spriteMenuBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteMenuBottom->setPosition(winSize.width / 2,
			spriteMenuBottom->getContentSize().height / 2);
	this->addChild(spriteMenuBottom);

	//Sprite search
	Sprite* spriteSearch = Sprite::create(s_stickerscene_sprite_search);
	spriteSearch->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteSearch->setPosition(100,
			spriteMenuBottom->getContentSize().height / 2 - 10);
	this->addChild(spriteSearch);

	//Add btn search all
	Button* btnSearchAll = Button::create(s_stickerscene_btn_all);
	btnSearchAll->setPosition(
			Vec2(400, spriteMenuBottom->getContentSize().height / 2 - 10));
	btnSearchAll->setTouchEnabled(true);
	btnSearchAll->setPressedActionEnabled(true);
	btnSearchAll->setOpacity(123);
	btnSearchAll->addTouchEventListener(
			[this](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
						this->removeChild(backgroundLayer, false);
						backgroundLayer = nullptr;
						scrollview->setVisible(true);
					} else if(searchingType != SEARCH_TYPE_ALL)
					{
						auto *newScene = StickerScene::scene(SEARCH_TYPE_ALL);
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}
				}});
	this->addChild(btnSearchAll);
	if (searchingType == SEARCH_TYPE_ALL) {
		btnSearchAll->setOpacity(255);
	}

	//Add btn search stick
	Button* btnSearchStick = Button::create(s_stickerscene_btn_stick);
	btnSearchStick->setPosition(
			Vec2(700, spriteMenuBottom->getContentSize().height / 2 - 10));
	btnSearchStick->setTouchEnabled(true);
	btnSearchStick->setPressedActionEnabled(true);
	btnSearchStick->setOpacity(123);
	btnSearchStick->addTouchEventListener(
			[this](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
						this->removeChild(backgroundLayer, false);
						backgroundLayer = nullptr;
						scrollview->setVisible(true);
					} else if(searchingType != SEARCH_TYPE_STICK)
					{
						auto *newScene = StickerScene::scene(SEARCH_TYPE_STICK);
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}
				}});
	this->addChild(btnSearchStick);
	if (searchingType == SEARCH_TYPE_STICK) {
		btnSearchStick->setOpacity(255);
	}

	//Add btn sell
	Button* btnSell = Button::create(s_stickerscene_btn_sell);
	btnSell->setPosition(
			Vec2(1000, spriteMenuBottom->getContentSize().height / 2 - 10));
	btnSell->setTouchEnabled(true);
	btnSell->setPressedActionEnabled(true);
	btnSell->setOpacity(123);
	btnSell->addTouchEventListener([this](Ref *pSender,
			Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
		{
			auto *newScene = StickerScene::scene(SEARCH_TYPE_SELL);
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}});
	this->addChild(btnSell);
	if (searchingType == SEARCH_TYPE_SELL) {
		btnSell->setOpacity(255);
	}
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
	btnStickerScene->addTouchEventListener(
			[this](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
						this->removeChild(backgroundLayer, false);
						backgroundLayer = nullptr;
						scrollview->setVisible(true);
					}
				}});
	this->addChild(btnStickerScene);

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
	btnAlbumScene->addTouchEventListener(
			[this](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
						this->removeChild(backgroundLayer, false);
						backgroundLayer = nullptr;
						scrollview->setVisible(true);
					} else
					{
						auto *newScene = AlbumScene::scene();
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}
				}});
	this->addChild(btnAlbumScene);

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
	btnHomeScene->addTouchEventListener(
			[this](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
						this->removeChild(backgroundLayer, false);
						backgroundLayer = nullptr;
						scrollview->setVisible(true);
					} else
					{
						auto *newScene = HomeScene::scene();
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}
				}});
	this->addChild(btnHomeScene);
}

void StickerScene::addAllStickersToScrollView() {
	TTFConfig configStickerDetailLabel(s_font, 65 * s_font_ratio);

	//Scrollview configuration
	int numberOfItems = vt_stickers_searching.size();
	int scrollviewMarginTop = 150;
	int scrollviewMarginBottom = 122;
	float itemMargin = 600;
	Size scrollFrameSize = Size(winSize.width,
			winSize.height - scrollviewMarginTop - scrollviewMarginBottom);

	//Create scrollview
	scrollview = BScrollView::createVertical(ceil(numberOfItems / 2.0f),
			itemMargin, scrollFrameSize);
	scrollview->setPosition(
			Vec2(winSize.width / 2,
					winSize.height / 2 - scrollviewMarginTop / 2
							+ scrollviewMarginBottom / 2));
	scrollview->setScrollBarEnabled(false);
	CCLog("bambi AlbumScene - inner height: %f, frame hight: %f",
			scrollview->getInnerContainerSize().height, scrollFrameSize.height);
	if (scrollview->getInnerContainerSize().height <= scrollFrameSize.height) {
		scrollview->setBounceEnabled(false);
	}
	this->addChild(scrollview);

	//Add sth to scroll view
	float positionX = scrollview->leftPosition;
	float positionY = scrollview->topPosition + 30;
	for (int i = 0; i < numberOfItems; i++) {
		Sticker* sticker = vt_stickers_searching.at(i);

		//Add btn sticker
		Button* btnStickerScene = Button::create(sticker->sticker_image);
		btnStickerScene->setPosition(
				Vec2(
						positionX
								+ scrollFrameSize.width
										* (i % 2 == 0 ? 0.28 : 0.72)
								- itemMargin / 2, positionY));
		btnStickerScene->setTouchEnabled(true);
		btnStickerScene->setZoomScale(0);
		btnStickerScene->setPressedActionEnabled(true);
		btnStickerScene->setTag(sticker->sticker_id);
		btnStickerScene->addTouchEventListener(
				[this](Ref *pSender,
						Widget::TouchEventType type) {
					if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
					{
						if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
							this->removeChild(backgroundLayer, false);
							backgroundLayer = nullptr;
							scrollview->setVisible(true);
						} else
						{
							int tag = (int) dynamic_cast<Button*>(pSender)->getTag();
							CCLog("bambi btnStickerScene->addTouchEventListener, tag: %d",tag);
							openStickerDetailLayer(StickerHelper::getStickerFromId(tag));
						}
					}});
		scrollview->addChild(btnStickerScene);

		//Sticker detail sprite
		Sprite* itemDetailSprite = Sprite::create(
				s_stickerscene_sprite_sticker_detail);
		itemDetailSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		itemDetailSprite->setPosition(
				Vec2(btnStickerScene->getPositionX(),
						btnStickerScene->getPositionY()
								- btnStickerScene->getContentSize().width / 2
								- 50));
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
		BLabel* labelStickerQuantity = BLabel::createWithTTF(
				configStickerDetailLabel,
				String::createWithFormat(
						stickerQuantity == 1 ? "1 sticker" : "%d stickers",
						stickerQuantity)->getCString(), TextHAlignment::CENTER);
		labelStickerQuantity->setPosition(
				Vec2(255, itemDetailSprite->getContentSize().height / 2 - 2));
		labelStickerQuantity->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelStickerQuantity->setColor(Color3B::BLACK);
		itemDetailSprite->addChild(labelStickerQuantity);

		//Stick sprite if needed
		if (StickerHelper::isStickerHasNotSticked(sticker->sticker_id)) {
			Sprite* stickerStick = Sprite::create(s_stickerscene_sprite_stick);
			stickerStick->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			stickerStick->setPosition(
					Vec2(0, btnStickerScene->getContentSize().height));
			btnStickerScene->addChild(stickerStick);

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

void StickerScene::openStickerDetailLayer(Sticker* sticker) {
	if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
		return;
	}

	CCLog("bambi openStickerDetailLayer");
	TTFConfig configStickerDetailLabel(s_font, 100 * s_font_ratio);

	//Add blur layer
	backgroundLayer = LayerColor::create(Color4B(0, 0, 0, 255));
	backgroundLayer->setContentSize(winSize);
	backgroundLayer->setPosition(Vec2::ZERO);
	backgroundLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->addChild(backgroundLayer);

	//Add sticker sprite at center of the screen
	Sprite* stickerSprite = Sprite::create(sticker->sticker_image);
	stickerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	stickerSprite->setPosition(winSize.width / 2, winSize.height / 2);
	stickerSprite->setScale(1.5f);
	backgroundLayer->addChild(stickerSprite);

	//Add sticker id label
	BLabel* labelStickerId = BLabel::createWithTTF(configStickerDetailLabel,
			String::createWithFormat("#%d", sticker->sticker_id)->getCString(),
			TextHAlignment::CENTER);
	labelStickerId->setPosition(
			Vec2(winSize.width / 2,
					winSize.height - labelStickerId->getContentSize().height));
	labelStickerId->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelStickerId->setColor(Color3B::WHITE);
	backgroundLayer->addChild(labelStickerId);

	//Add sticker name label
	BLabel* labelStickerName = BLabel::createWithTTF(configStickerDetailLabel,
			sticker->sticker_name, TextHAlignment::CENTER);
	labelStickerName->setPosition(
			Vec2(winSize.width / 2,
					labelStickerId->getPositionY()
							- labelStickerName->getContentSize().height));
	labelStickerName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelStickerName->setColor(Color3B::YELLOW);
	backgroundLayer->addChild(labelStickerName);

	//Add sticker glued info label
	int stickerTotalNumber = StickerHelper::getStickerQuantityInMyList(
			sticker->sticker_id);
	int gluedNumber =
			StickerHelper::isStickerHasNotSticked(sticker->sticker_id) ? 0 : 1;
	int leftNumber = stickerTotalNumber - gluedNumber;
	BLabel* labelStickerGluedInfo = BLabel::createWithTTF(
			configStickerDetailLabel,
			String::createWithFormat("(%d glued, %d left)", gluedNumber,
					leftNumber)->getCString(), TextHAlignment::CENTER);
	labelStickerGluedInfo->setPosition(
			Vec2(winSize.width / 2,
					labelStickerName->getPositionY()
							- labelStickerGluedInfo->getContentSize().height));
	labelStickerGluedInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelStickerGluedInfo->setColor(Color3B::GRAY);
	backgroundLayer->addChild(labelStickerGluedInfo);

	//Add sticker page name label
	BLabel* labelStickerPageName =
			BLabel::createWithTTF(configStickerDetailLabel,
					StickerHelper::getStickerPageFromId(
							sticker->sticker_page_id)->sticker_page_name,
					TextHAlignment::CENTER);
	labelStickerPageName->setPosition(
			Vec2(winSize.width / 2, winSize.height * 0.2));
	labelStickerPageName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelStickerPageName->setColor(Color3B::WHITE);
	backgroundLayer->addChild(labelStickerPageName);

	//Add sticker rarity label
	BLabel* labelStickerRarity = BLabel::createWithTTF(configStickerDetailLabel,
			StickerHelper::getRarityString(sticker->rarity),
			TextHAlignment::CENTER);
	labelStickerRarity->setPosition(
			Vec2(winSize.width / 2,
					labelStickerPageName->getPositionY()
							- labelStickerRarity->getContentSize().height));
	labelStickerRarity->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelStickerRarity->setColor(Color3B::WHITE);
	backgroundLayer->addChild(labelStickerRarity);

	//Add button go to album scene
	if (StickerHelper::isStickerHasNotSticked(sticker->sticker_id)) {
		Button* btnAlbumScene = Button::create(s_albumscene_btn_sticker);
		btnAlbumScene->setPosition(
				Vec2(
						winSize.width
								- btnAlbumScene->getContentSize().width / 2
								- 20, winSize.height * 0.1f));
		btnAlbumScene->setTouchEnabled(true);
		btnAlbumScene->setPressedActionEnabled(true);
		btnAlbumScene->addTouchEventListener(
				[this, sticker](Ref *pSender,
						Widget::TouchEventType type) {
					if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
					{
						auto *newScene = AlbumScene::scene(sticker->sticker_page_id - 1);
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}});
		backgroundLayer->addChild(btnAlbumScene);
	}

	//Add button sell
	if (StickerHelper::isStickerAbleToSell(sticker->sticker_id)) {
		Button* btnSellSticker = Button::create(
				s_stickerscene_btn_sell_sticker);
		btnSellSticker->setPosition(
				Vec2(btnSellSticker->getContentSize().width / 2 - 20,
						winSize.height * 0.1f));
		btnSellSticker->setTouchEnabled(true);
		btnSellSticker->setPressedActionEnabled(true);
		btnSellSticker->addTouchEventListener([this, sticker](Ref *pSender,
				Widget::TouchEventType type) {
			if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
			{
				StickerHelper::sellSticker(sticker);
				auto *newScene = StickerScene::scene(searchingType);
				Director *pDirector = Director::getInstance();
				pDirector->replaceScene(newScene);
			}});
		backgroundLayer->addChild(btnSellSticker);
	}

	//Hide scrollview to make onTouchBegan works
	scrollview->setVisible(false);
}

bool StickerScene::onTouchBegan(Touch* touch, Event* event) {
	CCLog("bambi onTouchBegan");
	if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
		this->removeChild(backgroundLayer, false);
		backgroundLayer = nullptr;
		scrollview->setVisible(true);
	}
	return true;
}

bool firstClickInStickerScene = true;
void StickerScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
			this->removeChild(backgroundLayer, false);
			backgroundLayer = nullptr;
			scrollview->setVisible(true);
		} else {
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
}
