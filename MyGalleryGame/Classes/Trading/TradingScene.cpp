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

// on "init" you need to initialize your instance
bool TradingScene::init() {
	bool result = BaseScene::init();
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	CCLog("bambi TradingScene init");
	//Set delegate to get response from FacebookHandler and FirebaseHandler
//	FirebaseHandler::getInstance()->setFirebaseDelegate(this);

	CCLog("bambi TradingScene init - after setting callback");
	//Add background
	auto background = Sprite::create(s_tradescene_background);
	background->setPosition(winSize.width / 2, winSize.height * 0.5);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(background);


	CCLog("bambi TradingScene init - after setting background");
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


	CCLog("bambi TradingScene init - after setting btn back");
	//Label user name
	TTFConfig configControlButton(s_font, 65 * s_font_ratio);
	Label* labelUserName = Label::createWithTTF(configControlButton,
			user->getName().c_str(), TextHAlignment::CENTER);
	labelUserName->setPosition(winSize / 2);
	labelUserName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelUserName->setColor(Color3B::BLACK);
	this->addChild(labelUserName);

	CCLog("bambi TradingScene init - after setting label user name");
	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(TradingScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return true;
}

//void TradingScene::responseAfterCheckFacebookIdExistOnFirebase() {
//}
//void TradingScene::responseForQuerryTopFriend(vector<BUserInfor*> friendList) {
//}
//void TradingScene::responseAfterGetStickersDataFromFirebase(string facebookId,
//		string stickerData) {
//}
void TradingScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {
		auto *newScene = HomeScene::scene();
		auto transition = TransitionFade::create(1.0, newScene);
		Director *pDirector = Director::getInstance();
		pDirector->replaceScene(transition);
	}
}
void TradingScene::backToHome(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		auto *newScene = HomeScene::scene();
		auto transition = TransitionFade::create(1.0, newScene);
		Director *pDirector = Director::getInstance();
		pDirector->replaceScene(transition);
	}
}

