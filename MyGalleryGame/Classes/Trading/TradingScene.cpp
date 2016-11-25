#include "SimpleAudioEngine.h"
#include "BLeaderBoardResources.h"
#include "CppUtils.h"
#include "TradingScene.h"

using namespace CocosDenshion;

#define TAG_MODE_BUTTON 1231
#define TAG_FACEBOOK_BUTTON 1232

Scene* TradingScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	TradingScene *layer = TradingScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TradingScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
    
	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();
	isGettingData = false;

	//Set delegate to get response from FacebookHandler and FirebaseHandler
	FirebaseHandler::getInstance()->setFirebaseDelegate(this);
	FacebookHandler::getInstance()->setFacebookConnectDelegate(this);

	//Set default value for UserDefault
	isWorldMode = UserDefault::getInstance()->getBoolForKey(RANKINGWORLDMODE,true);
	UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, isWorldMode);

	//Add loading Sprite
	loadingSprite = Sprite::create(s_ranking_loadingSprite);
	loadingSprite->setPosition(winSize.width / 2, winSize.height * 0.45f);
	auto rotation = RotateBy::create(0.25f, 60);
	loadingSprite->runAction(RepeatForever::create(rotation));
	this->addChild(loadingSprite, 200);

	//Add error label
	TTFConfig configError(s_font_leaderboard, 70 );
	labelError = Label::createWithTTF(configError,
			"Check your connection\nand try again later.",
			TextHAlignment::CENTER, winSize.width * 0.7);
	labelError->setPosition(winSize.width / 2, winSize.height * 0.45f);
	labelError->setColor(Color3B(255, 255,255));
	labelError->setVisible(false);
	this->addChild(labelError, 200);

	//Add background
	auto background = Sprite::create(s_ranking_background);
	background->setPosition(winSize.width / 2, winSize.height *0.5);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(background);


	//Add home button
	auto btn_home = Button::create(s_ranking_btnHome);
	btn_home->setPosition(Vec2(winSize.width*0.95-20,winSize.height*0.52));
	btn_home->setTouchEnabled(true);
	btn_home->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btn_home->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::backToHome, this));
	this->addChild(btn_home);

	//Add play again button
	auto btn_replay = Button::create(s_ranking_btnPlay);
	btn_replay->setPosition(Vec2(winSize.width*0.95-20, winSize.height*0.52));
	btn_replay->setTouchEnabled(true);
	btn_replay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btn_replay->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::playAgain, this));
	this->addChild(btn_replay);

	if(UserDefault::getInstance()->getBoolForKey("isFromMenuScene",true))
		btn_replay->setVisible(false);

	//Add connectFacebook button
	btnConnectFacebook = Button::create(s_ranking_btnConnectFacebook_LogIn);
	btnConnectFacebook->setPosition(Vec2(winSize.width * 0.95-20,winSize.height * 0.74));
	btnConnectFacebook->setTouchEnabled(true);
	btnConnectFacebook->setPressedActionEnabled(true);
	btnConnectFacebook->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnConnectFacebook->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::facebookConnectButton, this));
	this->addChild(btnConnectFacebook);

	//Add board
	board = Sprite::create(s_ranking_board);
	board->setPosition(winSize.width / 2-15, winSize.height *0.5);
	board->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(board);


	//Add worldMode button
	btnWorldMode = Button::create(s_ranking_btnWorldMode);
	btnWorldMode->setPosition(Vec2(winSize.width * 0.38,winSize.height * 0.69));
	btnWorldMode->setTouchEnabled(true);
	btnWorldMode->setPressedActionEnabled(true);
	btnWorldMode->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::worldModeButton, this));
	this->addChild(btnWorldMode);

	//Add friendMode button
	btnFriendMode = Button::create(s_ranking_btnFriendMode);
	btnFriendMode->setPosition(Vec2(winSize.width * 0.62,winSize.height * 0.69));
	btnFriendMode->setTouchEnabled(true);
	btnFriendMode->setPressedActionEnabled(true);
	btnFriendMode->addTouchEventListener(
			CC_CALLBACK_2(TradingScene::friendModeButton, this));
	this->addChild(btnFriendMode);


	//Update all button image
	updateUI(TAG_MODE_BUTTON);
	updateUI(TAG_FACEBOOK_BUTTON);

    //After load check facebook on Firebase responseAfterCheckFacebookIDExistOnFirebase function will be called
	if(FacebookHandler::getInstance()->isFacebookLoggedIn())
		FirebaseHandler::getInstance()->checkFacebookIdExistOnFirebase();
	else
	{
		loadingSprite->setVisible(false);
		labelError->setVisible(true);
	}





	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased =
			CC_CALLBACK_2(TradingScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return true;
}

void TradingScene::facebookConnectButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		if (FacebookHandler::getInstance()->isFacebookLoggedIn())
			FacebookHandler::getInstance()->logoutFacebook();
		else
			FacebookHandler::getInstance()->loginFacebook();
		//After login or logout, responseWhenLoginOrLogoutFacebook will be called
	}
}
void TradingScene::worldModeButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED && !isWorldMode && !isGettingData) {
			UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, true);
			isWorldMode = true;
			updateUI(TAG_MODE_BUTTON);
			loadLeaderboardData();
	}
}
void TradingScene::friendModeButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED && isWorldMode && !isGettingData) {
			UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, false);
			isWorldMode = false;
			updateUI(TAG_MODE_BUTTON);
			loadLeaderboardData();
	}
}
void TradingScene::updateUI(int tag)
{
	switch (tag) {
		case TAG_MODE_BUTTON:
		{
			//Update image of mode button
			if (isWorldMode)
			{
				btnWorldMode->loadTextureNormal(s_ranking_btnWorldMode_Clicked);
				btnFriendMode->loadTextureNormal(s_ranking_btnFriendMode);
			}
			else
			{
				btnWorldMode->loadTextureNormal(s_ranking_btnWorldMode);
				btnFriendMode->loadTextureNormal(s_ranking_btnFriendMode_CLicked);
			}
		}
			break;
		case TAG_FACEBOOK_BUTTON:
		{
			//Update image of facebook button
			if (FacebookHandler::getInstance()->isFacebookLoggedIn())
				btnConnectFacebook->loadTextureNormal(s_ranking_btnConnectFacebook_LogOut);
			else
				btnConnectFacebook->loadTextureNormal(s_ranking_btnConnectFacebook_LogIn);
		}
			break;
	}
}

void TradingScene::responseWhenLoginOrLogoutFacebook()
{
	updateUI(TAG_FACEBOOK_BUTTON);
	//After load check facebook on Firebase responseAfterCheckFacebookIDExistOnFirebase function will be called
	if(FacebookHandler::getInstance()->isFacebookLoggedIn())
		FirebaseHandler::getInstance()->checkFacebookIdExistOnFirebase();
	else
		resetTradingScene(); //Reset trading scene.
}
void TradingScene::responseAfterCheckFacebookIDExistOnFirebase()
{
    //Submit score at first time
    submitScore(UserDefault::getInstance()->getIntegerForKey(CURRENT_STICKER,0));

	loadLeaderboardData();
}
void TradingScene::loadLeaderboardData()
{
	board->removeAllChildren();
	if(FacebookHandler::getInstance()->isFacebookLoggedIn())
	{
		isGettingData = true;
		labelError->setVisible(false);
		loadingSprite->setVisible(true);
		if(isWorldMode)
			FirebaseHandler::getInstance()->fetchTopWorld();
		else
			FirebaseHandler::getInstance()->fetchTopFriend();
		//After fetch data responseForQuerryTopWorld or responseForQuerryTopFriend will be called.
	}else
	{
		labelError->setVisible(true);
		isGettingData = false;
	}
}
void TradingScene::responseForQuerryTopWorld(vector<BUserInfor*> worldList)
{
	showScoreOnScreen(worldList);
}
void TradingScene::responseForQuerryTopFriend(vector<BUserInfor*> friendList)
{
	showScoreOnScreen(friendList);
}

void TradingScene::resetTradingScene()
{
	auto *newScene = TradingScene::scene();
			 Director *pDirector = Director::getInstance();
			 pDirector->replaceScene(newScene);
}
void TradingScene::showScoreOnScreen(vector<BUserInfor*> listUser) {

	loadingSprite->setVisible(false);
	isGettingData = false;

	TTFConfig configUser(s_font_leaderboard, 60);
	float positionYBegin = board->getContentSize().height * 0.6;
	float eachUserMargin = board->getContentSize().height * 0.12;
	for (int i = 0; i < listUser.size(); i++) {
		//Background and label of rank (at the left side)
		Sprite* numberBackgroundSprite;
		numberBackgroundSprite = Sprite::create(s_ranking_number);
		numberBackgroundSprite->setPosition(board->getContentSize().width * 0.15f,positionYBegin - eachUserMargin * i);
		board->addChild(numberBackgroundSprite);

		auto lRanking = Label::createWithTTF(configUser,
				CppUtils::doubleToString(i + 1), TextHAlignment::CENTER);
		lRanking->setPosition(numberBackgroundSprite->getContentSize().width/2,numberBackgroundSprite->getContentSize().height /2);
		lRanking->setColor(Color3B(255,255,255));
		numberBackgroundSprite->addChild(lRanking);


		//Split name if it's exessive and add label Name add the middle
		string stringName = listUser[i]->getName();
		if(stringName.length() > 12)
		{
			auto vt_splitedString = CppUtils::splitStringByEveryCharacter(listUser[i]->getName());
			stringName = vt_splitedString[vt_splitedString.size()-1];
		}
		auto lName = Label::createWithTTF(configUser, stringName,TextHAlignment::CENTER);
		lName->setPosition(board->getContentSize().width * 0.5f,positionYBegin - eachUserMargin * i);
		lName->setColor(Color3B(255,255,255));
		lName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		board->addChild(lName);


		//Label score (at the right side)
		auto lScore = Label::createWithTTF(configUser,
				CppUtils::doubleToString(listUser[i]->getScore()),
				TextHAlignment::CENTER);
		lScore->setPosition(board->getContentSize().width * 0.85f,positionYBegin - eachUserMargin * i);
		lScore->setColor(Color3B(255,255,255));
		board->addChild(lScore);
	}
}














void TradingScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {
//        auto *newScene = GameScene::createScene();
//        auto transition = TransitionFade::create(1.0, newScene);
//        Director *pDirector = Director::getInstance();
//        pDirector->replaceScene(transition);
	}
}
void TradingScene::backToHome(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
//		auto *newScene = MainMenuScene::scene();
//		auto transition = TransitionFade::create(1.0, newScene);
//		Director *pDirector = Director::getInstance();
//		pDirector->replaceScene(transition);
	}
}
void TradingScene::playAgain(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
//		auto *newScene = GameScene::createScene();
//		auto transition = TransitionFade::create(1.0, newScene);
//		Director *pDirector = Director::getInstance();
//		pDirector->replaceScene(transition);
	}
}


