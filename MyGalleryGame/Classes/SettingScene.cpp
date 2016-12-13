#include "SettingScene.h"
#include "HomeScene.h"

#define kTagBack 0
#define kTagLoginLogoutFacebook 1
#define kTagMusic 2
#define kTagSound 3
#define kTagAlert 4

Scene* SettingScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	SettingScene *layer = SettingScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init() {
	bool result = BaseScene::init();

	FacebookHandler::getInstance()->setFacebookConnectDelegate(this);

	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	isTouchedOnFacebookConnect = false;
	TTFConfig config(s_font, 120 * s_font_ratio);

	//Add background
	Sprite* background = Sprite::create(s_settingscene_background);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//Add btn back
	Button* btnBack = Button::create(s_settingscene_btn_back);
	btnBack->setPosition(
			Vec2(btnBack->getContentSize().width / 2 + 15,
					winSize.height - btnBack->getContentSize().height / 2
							- 15));
	btnBack->setTouchEnabled(true);
	btnBack->setPressedActionEnabled(true);
	btnBack->addTouchEventListener(
			CC_CALLBACK_2(SettingScene::settingButtonsCallback, this));
	btnBack->setTag(kTagBack);
	this->addChild(btnBack);

	//Add btn music
	Button* btnMusic = Button::create(
			isMusic ?
					s_settingscene_rdb_selected :
					s_settingscene_rdb_unselected);
	btnMusic->setPosition(Vec2(winSize.width * 0.6, winSize.height * 0.8));
	btnMusic->setTouchEnabled(true);
	btnMusic->setPressedActionEnabled(true);
	btnMusic->addTouchEventListener(
			CC_CALLBACK_2(SettingScene::settingButtonsCallback, this));
	btnMusic->setTag(kTagMusic);
	this->addChild(btnMusic);

	Label* labelMusic = Label::createWithTTF(config, "MUSIC",
			TextHAlignment::CENTER);
	labelMusic->setPosition(
			Vec2(
					btnMusic->getPositionX()
							- labelMusic->getContentSize().width / 2 - 200,
					btnMusic->getPositionY()));
	labelMusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelMusic->setColor(Color3B::BLACK);
	this->addChild(labelMusic);

	//Add btn sound
	Button* btnSound = Button::create(
			isSound ?
					s_settingscene_rdb_selected :
					s_settingscene_rdb_unselected);
	btnSound->setPosition(Vec2(winSize.width * 0.6, winSize.height * 0.65));
	btnSound->setTouchEnabled(true);
	btnSound->setPressedActionEnabled(true);
	btnSound->addTouchEventListener(
			CC_CALLBACK_2(SettingScene::settingButtonsCallback, this));
	btnSound->setTag(kTagSound);
	this->addChild(btnSound);

	Label* labelSound = Label::createWithTTF(config, "SOUND",
			TextHAlignment::CENTER);
	labelSound->setPosition(
			Vec2(
					btnSound->getPositionX()
							- labelSound->getContentSize().width / 2 - 200,
					btnSound->getPositionY()));
	labelSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelSound->setColor(Color3B::BLACK);
	this->addChild(labelSound);

	//Add btn alert
	Button* btnAlert = Button::create(
			isAlert ?
					s_settingscene_rdb_selected :
					s_settingscene_rdb_unselected);
	btnAlert->setPosition(Vec2(winSize.width * 0.6, winSize.height * 0.5));
	btnAlert->setTouchEnabled(true);
	btnAlert->setPressedActionEnabled(true);
	btnAlert->addTouchEventListener(
			CC_CALLBACK_2(SettingScene::settingButtonsCallback, this));
	btnAlert->setTag(kTagAlert);
	this->addChild(btnAlert);

	Label* labelAlert = Label::createWithTTF(config, "ALERT",
			TextHAlignment::CENTER);
	labelAlert->setPosition(
			Vec2(
					btnAlert->getPositionX()
							- labelAlert->getContentSize().width / 2 - 200,
					btnAlert->getPositionY()));
	labelAlert->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelAlert->setColor(Color3B::BLACK);
	this->addChild(labelAlert);

	//Add btn login logout facebook
	Button* btnLoginLogoutFacebook = Button::create(s_settingscene_btn_loggout);
	btnLoginLogoutFacebook->setPosition(
			Vec2(winSize.width * 0.5, winSize.height * 0.3));
	btnLoginLogoutFacebook->setTouchEnabled(true);
	btnLoginLogoutFacebook->setPressedActionEnabled(true);
	btnLoginLogoutFacebook->addTouchEventListener(
			CC_CALLBACK_2(SettingScene::settingButtonsCallback, this));
	btnLoginLogoutFacebook->setTag(kTagLoginLogoutFacebook);
	this->addChild(btnLoginLogoutFacebook);

	labelLoginLogoutFacebook = Label::createWithTTF(config,
			FacebookHandler::getInstance()->isFacebookLoggedIn() ?
					"LOGOUT" : "LOGIN", TextHAlignment::CENTER);
	labelLoginLogoutFacebook->setPosition(
			btnLoginLogoutFacebook->getPosition());
	labelLoginLogoutFacebook->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelLoginLogoutFacebook->setColor(Color3B::BLACK);
	this->addChild(labelLoginLogoutFacebook);

	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(SettingScene::onKeyReleased,
			this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return result;
}
void SettingScene::responseWhenLoginOrLogoutFacebook() {
	if (FacebookHandler::getInstance()->isFacebookLoggedIn()) {
		CCLog(
				"bambi SettingScene -> responseWhenLoginOrLogoutFacebook logged in");
		labelLoginLogoutFacebook->setString("LOGOUT");
	} else {
		CCLog(
				"bambi SettingScene -> responseWhenLoginOrLogoutFacebook logged out");
		labelLoginLogoutFacebook->setString("LOGIN");
	}
}
void SettingScene::settingButtonsCallback(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		Button* button = dynamic_cast<Button*>(pSender);
		int tag = (int) button->getTag();
		switch (tag) {
		case kTagBack: {
			isTouchedOnFacebookConnect = true;
			if (!isTouchedOnFacebookConnect) {
				CustomDirector *director =
						(CustomDirector *) CustomDirector::getInstance();
				director->popSceneWithTransitionFade(1);
			} else {
				auto *newScene = HomeScene::scene();
				auto transition = TransitionFade::create(1.0, newScene);
				Director *pDirector = Director::getInstance();
				pDirector->replaceScene(transition);
			}

		}
			break;
		case kTagLoginLogoutFacebook: {
			isTouchedOnFacebookConnect = true;
			if (FacebookHandler::getInstance()->isFacebookLoggedIn()) {
				FacebookHandler::getInstance()->logoutFacebook();
			} else {
				FacebookHandler::getInstance()->loginFacebook();
			}
		}
			break;
		case kTagMusic: {
			isMusic = !isMusic;
			UserDefault::getInstance()->setBoolForKey(MUSIC, isMusic);
			button->loadTextureNormal(
					isMusic ?
							s_settingscene_rdb_selected :
							s_settingscene_rdb_unselected,
					TextureResType::LOCAL);
		}
			break;
		case kTagAlert: {
			isAlert = !isAlert;
			UserDefault::getInstance()->setBoolForKey(ALERT, isAlert);
			button->loadTextureNormal(
					isAlert ?
							s_settingscene_rdb_selected :
							s_settingscene_rdb_unselected,
					TextureResType::LOCAL);
		}
			break;
		case kTagSound: {
			isSound = !isSound;
			UserDefault::getInstance()->setBoolForKey(SOUND, isSound);
			button->loadTextureNormal(
					isSound ?
							s_settingscene_rdb_selected :
							s_settingscene_rdb_unselected,
					TextureResType::LOCAL);
		}
			break;

		}
	}
}
void SettingScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		isTouchedOnFacebookConnect = true;
		if (!isTouchedOnFacebookConnect) {
			CustomDirector *director =
					(CustomDirector *) CustomDirector::getInstance();
			director->popSceneWithTransitionFade(1);
		} else {
			auto *newScene = HomeScene::scene();
			auto transition = TransitionFade::create(1.0, newScene);
			Director *pDirector = Director::getInstance();
			pDirector->replaceScene(transition);
		}
	}
}
