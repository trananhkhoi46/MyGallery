#include "SimpleAudioEngine.h"
#include "BLeaderBoardResources.h"
#include "CppUtils.h"
#include "TradingScene.h"
#include "../HomeScene.h"

BUserInfor* user;
Scene* TradingScene::scene(BUserInfor* userInfor) {
	user = userInfor;
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	TradingScene *layer = TradingScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void TradingScene::parseAllStickers() {
	vector < string > vtSplitAllStickerStrings = CppUtils::splitStringByDelim(
			user->getAllStickers(), ',');
	set < string
			> splitAllStickerStrings(vtSplitAllStickerStrings.begin(),
					vtSplitAllStickerStrings.end());
	vector < string > splitStickedStickerStrings = CppUtils::splitStringByDelim(
			user->getStickedStickers(), ',');
	string loggedInUserObjectId = UserDefault::getInstance()->getStringForKey(
	KEY_WORLD_OJECTID);
	vector<PendingRequest*> vtPendingRequest;
	vector < string > listStringPendingRequest = CppUtils::splitStringByDelim(
			user->getPendingRequest(), ',');

	//Remove all sticked stickers from vector stickers & pending request
	for (string record : splitAllStickerStrings) {
		Sticker* sticker = StickerHelper::getStickerFromId(
				CppUtils::stringToDouble(record));
		if (sticker != nullptr) {
			bool isRecordExistInStickedVector = false;
			for (string stickedRecord : splitStickedStickerStrings) {
				if (stickedRecord == record) {
					isRecordExistInStickedVector = true;
					splitStickedStickerStrings.erase(
							std::remove(splitStickedStickerStrings.begin(),
									splitStickedStickerStrings.end(),
									stickedRecord),
							splitStickedStickerStrings.end());
				}
			}
			for (PendingRequest* request : vtPendingRequest) {
				if (CppUtils::doubleToString(sticker->sticker_id)
						== request->getStickerId()) {
					if (request->getObjectId() != loggedInUserObjectId) {
						isRecordExistInStickedVector = true;
						vtPendingRequest.erase(
								std::remove(vtPendingRequest.begin(),
										vtPendingRequest.end(), request),
								vtPendingRequest.end());
					}
				}
			}

			if (!isRecordExistInStickedVector) {
				vt_stickers_of_user.push_back(sticker);
			}
		}
	}
}

// on "init" you need to initialize your instance
bool TradingScene::init() {
	bool result = BaseScene::init();
//////////////////////////////
// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	//Add listener
	FirebaseHandler::getInstance()->setFirebaseTradeFeatureDelegate(this);

	isDataChanged = false;
	parseAllStickers();
	TTFConfig config(s_font, 120 * s_font_ratio);

//Add background
	Sprite* background = Sprite::create(s_tradescene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

//Add all stickers to scrollview
	addAllStickersToScrollView();

	//Init views
	initControlButtons();

//Handling touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TradingScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(TradingScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);
	return result;
}
void TradingScene::initControlButtons() {
//Add btn back
	Button* btnBack = Button::create(s_tradescene_btn_back);
	btnBack->setPosition(
			Vec2(btnBack->getContentSize().width / 2 + 15,
					winSize.height - btnBack->getContentSize().height / 2
							- 15));
	btnBack->setTouchEnabled(true);
	btnBack->setPressedActionEnabled(true);
	btnBack->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::backToHome, this));
	this->addChild(btnBack);

	//Label user name
	TTFConfig configControlButton(s_font, 130 * s_font_ratio);
	Label* labelUserName = Label::createWithTTF(configControlButton,
			(user->getName() + "\'s stickers").c_str(), TextHAlignment::CENTER);
	labelUserName->setPosition(winSize.width / 2, btnBack->getPositionY());
	labelUserName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelUserName->setColor(Color3B::BLACK);
	this->addChild(labelUserName);
}
void TradingScene::addAllStickersToScrollView() {
	TTFConfig configStickerDetailLabel(s_font, 65 * s_font_ratio);

//Scrollview configuration
	int numberOfItems = vt_stickers_of_user.size();
	int scrollviewMarginTop = 185;
	float itemMargin = 600;
	Size scrollFrameSize = Size(winSize.width,
			winSize.height - scrollviewMarginTop);

//Create scrollview
	scrollview = BScrollView::createVertical(ceil(numberOfItems / 2.0f),
			itemMargin, scrollFrameSize);
	scrollview->setPosition(
			Vec2(winSize.width / 2,
					winSize.height / 2 - scrollviewMarginTop / 2));
	scrollview->setScrollBarEnabled(false);
	if (scrollview->getInnerContainerSize().height < scrollFrameSize.height) {
		scrollview->setBounceEnabled(false);
	}
	this->addChild(scrollview);

	//Get pending requests
	vector<PendingRequest*> vtPendingRequest;
	vector < string > listStringPendingRequest = CppUtils::splitStringByDelim(
			user->getPendingRequest(), ',');
	for (string pendingRequestString : listStringPendingRequest) {
		PendingRequest* request = new PendingRequest();
		vector < string > requestData = CppUtils::splitStringByDelim(
				pendingRequestString, '#');
		request->setObjectId(requestData.at(0));
		request->setName(requestData.at(1));
		request->setStickerId(requestData.at(2));
		vtPendingRequest.push_back(request);
	}

//Add sth to scroll view
	string loggedInUserObjectId = UserDefault::getInstance()->getStringForKey(
	KEY_WORLD_OJECTID);
	if (loggedInUserObjectId == "") {
		return;
	}
	float positionX = scrollview->leftPosition;
	float positionY = scrollview->topPosition + 30;
	for (int i = 0; i < numberOfItems; i++) {
		Sticker* sticker = vt_stickers_of_user.at(i);

		bool isStickerWaiting = false;
		for (PendingRequest* request : vtPendingRequest) {
			if (CppUtils::doubleToString(sticker->sticker_id)
					== request->getStickerId()) {
				if (request->getObjectId() == loggedInUserObjectId) {
					isStickerWaiting = true;
				}
				vtPendingRequest.erase(
						std::remove(vtPendingRequest.begin(),
								vtPendingRequest.end(), request),
						vtPendingRequest.end());
				break;
			}
		}

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
						int tag = (int) dynamic_cast<Button*>(pSender)->getTag();
						CCLog("bambi btnStickerScene->addTouchEventListener, tag: %d",tag);
						openStickerDetailLayer(StickerHelper::getStickerFromId(tag));
					}});
		scrollview->addChild(btnStickerScene);
		vt_btn_stickers_of_user.push_back(btnStickerScene);

		//Sticker detail sprite
		Sprite* itemDetailSprite = Sprite::create(
				s_tradescene_sprite_sticker_detail);
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
				configStickerDetailLabel, "1 sticker", TextHAlignment::CENTER);
		labelStickerQuantity->setPosition(
				Vec2(255, itemDetailSprite->getContentSize().height / 2 - 2));
		labelStickerQuantity->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelStickerQuantity->setColor(Color3B::BLACK);
		itemDetailSprite->addChild(labelStickerQuantity);

		if (isStickerWaiting) {
			Sprite* stickerWait = Sprite::create(s_tradescene_sprite_wait);
			stickerWait->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			stickerWait->setPosition(
					Vec2(0, btnStickerScene->getContentSize().height));
			btnStickerScene->addChild(stickerWait);
			stickerWait->runAction(
					RepeatForever::create(
							Sequence::create(ScaleTo::create(0.5, 0.9),
									ScaleTo::create(0.5, 1), nullptr)));
		} else {
			//Show ask sprite if the current user doesn't have this sticker -> suggest to ask this sticker
			if (!StickerHelper::isStickerHasAlreadyExisted(
					sticker->sticker_id)) {
				Sprite* stickerStick = Sprite::create(s_tradescene_sprite_ask);
				stickerStick->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
				stickerStick->setPosition(
						Vec2(0, btnStickerScene->getContentSize().height));
				btnStickerScene->addChild(stickerStick);

				stickerStick->runAction(
						RepeatForever::create(
								Sequence::create(ScaleTo::create(0.5, 0.9),
										ScaleTo::create(0.5, 1), nullptr)));
			}
		}

		if (i % 2 == 1) {
			positionY -= itemMargin;
		}
	}
}

void TradingScene::openStickerDetailLayer(Sticker* sticker) {
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

	//Add ask this sticker button
	Button* btnAsk = Button::create(s_tradescene_btn_ask);
	btnAsk->setPosition(
			Vec2(winSize.width - btnAsk->getContentSize().width / 2 - 20,
					winSize.height * 0.1f));
	btnAsk->setTouchEnabled(true);
	btnAsk->setPressedActionEnabled(true);
	btnAsk->addTouchEventListener(
			[this, sticker](Ref *pSender,
					Widget::TouchEventType type) {
				if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					CCLog("bambi btnStickerScene->addTouchEventListener, tag: %d - userobjectId: %s",sticker->sticker_id, user->getObjectId().c_str());
					FirebaseHandler::getInstance()->askTheStickerOfUer(sticker->sticker_id, user);
				}});
	backgroundLayer->addChild(btnAsk);

//Hide scrollview to make onTouchBegan works
	scrollview->setVisible(false);
}

bool TradingScene::onTouchBegan(Touch* touch, Event* event) {
	CCLog("bambi onTouchBegan");
	if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
		this->removeChild(backgroundLayer, false);
		backgroundLayer = nullptr;
		scrollview->setVisible(true);
	}
	return true;
}
void TradingScene::responseAfterCheckingPendingRequest(
		vector<PendingRequest*> vtPendingRequest) {

}
void TradingScene::responseAfterCheckingGivenSticker(
		vector<PendingRequest*> vtGivenStickers) {

}
void TradingScene::responseAfterAskingSticker(int stickerId, bool isSuccess) {
	CCLog("bambi responseAfterAskingSticker");
	if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
		this->removeChild(backgroundLayer, false);
		backgroundLayer = nullptr;
		scrollview->setVisible(true);
	}

	for (Button* button : vt_btn_stickers_of_user) {
		if (button->getTag() == stickerId) {
			button->removeAllChildren();
			if (!isSuccess) {
				//Show ask sprite if the current user doesn't have this sticker -> suggest to ask this sticker
				if (!StickerHelper::isStickerHasAlreadyExisted(stickerId)) {
					Sprite* stickerStick = Sprite::create(
							s_tradescene_sprite_ask);
					stickerStick->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
					stickerStick->setPosition(
							Vec2(0, button->getContentSize().height));
					button->addChild(stickerStick);

					stickerStick->runAction(
							RepeatForever::create(
									Sequence::create(ScaleTo::create(0.5, 0.9),
											ScaleTo::create(0.5, 1), nullptr)));
				}
			} else {
				isDataChanged = true;

				Sprite* stickerWait = Sprite::create(s_tradescene_sprite_wait);
				stickerWait->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
				stickerWait->setPosition(
						Vec2(0, button->getContentSize().height));
				button->addChild(stickerWait);
				stickerWait->runAction(
						RepeatForever::create(
								Sequence::create(ScaleTo::create(0.5, 0.9),
										ScaleTo::create(0.5, 1), nullptr)));
			}
			break;
		}
	}

	SocialPlugin::showToast(isSuccess ? "Successfully" : "Failed");
}

void TradingScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {
		if (backgroundLayer != nullptr && backgroundLayer->isVisible()) {
			this->removeChild(backgroundLayer, false);
			backgroundLayer = nullptr;
			scrollview->setVisible(true);
		} else {
			if (isDataChanged) {
				auto *newScene = HomeScene::scene();
				auto transition = TransitionFade::create(1.0, newScene);
				Director *pDirector = Director::getInstance();
				pDirector->replaceScene(transition);
			} else {
				CustomDirector *director =
						(CustomDirector *) CustomDirector::getInstance();
				director->popSceneWithTransitionFade(1);
			}
		}
	}
}
void TradingScene::backToHome(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		if (isDataChanged) {
			auto *newScene = HomeScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		} else {
			CustomDirector *director =
					(CustomDirector *) CustomDirector::getInstance();
			director->popSceneWithTransitionFade(1);
		}
	}
}

