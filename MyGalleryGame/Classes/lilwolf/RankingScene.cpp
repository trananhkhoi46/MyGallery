#include "RankingScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "Resources.h"
#include "SocialPlugin.h"
#include "CppUtils.h"
#include "FacebookAgent.h"
#include "GameScene.h"
#include "SonarFrameworks.h"

using namespace cocos2d::plugin;
using namespace CocosDenshion;

Scene* RankingScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	RankingScene *layer = RankingScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


void RankingScene::postScoreToServer()
{
	if (FacebookAgent::getInstance()->isLoggedIn() && strstr(FacebookAgent::getInstance()->getPermissionList().c_str(),"user_friends"))
	{

		std::string myObjectID= UserDefault::getInstance()->getStringForKey("MY_OBJECT_ID");
		if(myObjectID == "")
			return;

		vector < string > h;
		h.push_back(
				"X-Parse-Application-Id: hYf8HYrrrvZTdUligYAdy5lKtqrzHg7i8XMR8Jig");
		h.push_back(
				"X-Parse-REST-API-Key: bVFb1yUe4n6NX4oSzW9UCy1FqwR1HsMjnA5EfGqs");
		h.push_back("Content-Type: application/json");

		auto r2 = new HttpRequest();

		char url[256];
		sprintf(url, "https://api.parse.com/1/classes/RedHood/%s",myObjectID.c_str());
		const char* urlOK = url;

		r2->setUrl(urlOK);

		r2->setHeaders(h);

		r2->setRequestType(HttpRequest::Type::PUT);


		char data[5500];
		int score = UserDefault::getInstance()->getIntegerForKey(HIGHSCORE,0);
		sprintf(data, "{\"Score\":%d}",score);
		const char* buffer = data;
		r2->setRequestData(buffer, strlen(buffer));

		r2->setResponseCallback(this, httpresponse_selector(RankingScene::postScoreToServerCallBack));

		HttpClient::getInstance()->send(r2);

		//Close connection
		r2->release();


	}
}
void RankingScene::postScoreToServerCallBack(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {
//			log("Success");
//
//			Director::getInstance()->replaceScene(RankingScene::scene());
		} else {
			log("Failed");
		}
}




// on "init" you need to initialize your instance
bool RankingScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}



	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();

    SonarCocosHelper::IOS::Setup();
    SonarCocosHelper::AdMob::hideBannerAd(SonarCocosHelper::AdBannerPosition::eBoth);





	postScoreToServer();

	TTFConfig config(s_font_1, 100 * s_font_ratio);
	//Set default value for UserDefault
	isWorldMode = UserDefault::getInstance()->getBoolForKey(RANKINGWORLDMODE,
			true);
	UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, isWorldMode);

	//Add loading Sprite
	loadingSprite = Sprite::create(s_ranking_loadingSprite);
	loadingSprite->setPosition(winSize.width / 2, winSize.height * 0.45f);
	auto rotation = RotateBy::create(0.25f, 60);
	loadingSprite->runAction(RepeatForever::create(rotation));
	this->addChild(loadingSprite, 200);
	loadingSprite_child = Sprite::create(s_ranking_loadingSprite_child);
	loadingSprite_child->setPosition(winSize.width / 2, winSize.height * 0.45f);
	this->addChild(loadingSprite_child, 200);

	//Add error label
	TTFConfig configError(s_font_1, 70 * s_font_ratio);
	labelError = Label::createWithTTF(configError,
			"Please check your connection\nand try again later.",
			TextHAlignment::CENTER, winSize.width * 0.7);
	labelError->setPosition(winSize.width / 2, winSize.height * 0.45f);
	labelError->setColor(Color3B(188, 132, 75));
	labelError->setVisible(false);
	this->addChild(labelError, 200);

	//Add background
	auto background = Sprite::create(s_ranking_background);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(background);

	//Add paper
	board = Sprite::create(s_ranking_board);
	board->setPosition(winSize.width / 2, winSize.height / 2);
	board->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(board);

	//Add title
	auto lTitle = Sprite::create(s_ranking_title);
	lTitle->setPosition(board->getContentSize().width / 2,
			board->getContentSize().height * 0.85f);
	lTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	board->addChild(lTitle);

	//Add worldMode button
	Button* btnWorldMode;
	if (!isWorldMode)
		btnWorldMode = Button::create(s_ranking_btnWorldMode_Clicked);
	else
		btnWorldMode = Button::create(s_ranking_btnWorldMode);
	btnWorldMode->setPosition(
			Vec2(board->getContentSize().width * 0.4,
					board->getContentSize().height * 0.7));
	btnWorldMode->setTouchEnabled(true);
	btnWorldMode->setPressedActionEnabled(true);
	btnWorldMode->addTouchEventListener(
			CC_CALLBACK_2(RankingScene::worldModeButton, this));
	board->addChild(btnWorldMode);

	//Add friendMode button
	Button* btnFriendMode;
	if (isWorldMode)
		btnFriendMode = Button::create(s_ranking_btnFriendMode);
	else
		btnFriendMode = Button::create(s_ranking_btnFriendMode_CLicked);
	btnFriendMode->setPosition(
			Vec2(board->getContentSize().width * 0.6,
					board->getContentSize().height * 0.7));
	btnFriendMode->setTouchEnabled(true);
	btnFriendMode->setPressedActionEnabled(true);
	btnFriendMode->addTouchEventListener(
			CC_CALLBACK_2(RankingScene::friendModeButton, this));
	board->addChild(btnFriendMode);

	CCLog("permission : %s",FacebookAgent::getInstance()->getPermissionList().c_str());
	//Add connectFacebook button
	if (FacebookAgent::getInstance()->isLoggedIn() && strstr(FacebookAgent::getInstance()->getPermissionList().c_str(),"user_friends"))
		btnConnectFacebook = Button::create(
				s_ranking_btnConnectFacebook_LogOut);
	else
		btnConnectFacebook = Button::create(s_ranking_btnConnectFacebook_LogIn);
	btnConnectFacebook->setPosition(Vec2(winSize.width-5, winSize.height-5));
	btnConnectFacebook->setTouchEnabled(true);
	btnConnectFacebook->setPressedActionEnabled(true);
	btnConnectFacebook->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	btnConnectFacebook->addTouchEventListener(
			CC_CALLBACK_2(RankingScene::facebookConnectButton, this));
	this->addChild(btnConnectFacebook);

	//Add home button
	auto btn_home = Button::create(s_ranking_btnHome);
	btn_home->setPosition(
			Vec2(winSize.width,
					winSize.height - btnConnectFacebook->getContentSize().height
							- 10));
	btn_home->setTouchEnabled(true);
	btn_home->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	btn_home->addTouchEventListener(
			CC_CALLBACK_2(RankingScene::backToHome, this));
	this->addChild(btn_home);

	//Add play again button
		auto btn_replay = Button::create(s_ranking_btnPlay);
		btn_replay->setPosition(
				Vec2(winSize.width,
						winSize.height - btnConnectFacebook->getContentSize().height - 10 - btn_home->getContentSize().height
								- 10));
		btn_replay->setTouchEnabled(true);
		btn_replay->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		btn_replay->addTouchEventListener(
				CC_CALLBACK_2(RankingScene::playAgain, this));
		this->addChild(btn_replay);

	auto func = CallFunc::create([=]() {
		if (FacebookAgent::getInstance()->isLoggedIn() && strstr(FacebookAgent::getInstance()->getPermissionList().c_str(),"user_friends"))
			checkMyFacebookIDFromServer();
		else
		{
			loadingSprite->setVisible(false);
			loadingSprite_child->setVisible(false);
			labelError->setString("Please login to facebook.");
			labelError->setVisible(true);
			isRequestDone = true;
		}

	});
	this->runAction(Sequence::create(DelayTime::create(0.5f), func, nullptr));





	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased =
			CC_CALLBACK_2(RankingScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

	return true;
}

void RankingScene::backToHome(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		auto *newScene = MenuScene::scene();
		auto transition = TransitionFade::create(1.0, newScene);
		Director *pDirector = Director::getInstance();
		pDirector->replaceScene(transition);
	}
}
void RankingScene::playAgain(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		auto *newScene = GameScene::scene();
		auto transition = TransitionFade::create(1.0, newScene);
		Director *pDirector = Director::getInstance();
		pDirector->replaceScene(transition);
	}
}
void RankingScene::facebookConnectButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		if (FacebookAgent::getInstance()->isLoggedIn() && strstr(FacebookAgent::getInstance()->getPermissionList().c_str(),"user_friends")) {
			logoutFacebook();
			Director::getInstance()->replaceScene(RankingScene::scene());
		} else
			loginFacebook();
	}
}
void RankingScene::logoutFacebook() {
	//CCLog("Logout ne");
	FacebookAgent::getInstance()->logout();
	btnConnectFacebook->loadTextureNormal(s_ranking_btnConnectFacebook_LogIn,
			TextureResType::LOCAL);
}
void RankingScene::loginFacebook() {

	std::string permissions = "public_profile,user_friends";
	FacebookAgent::getInstance()->login(permissions,
			[=](int ret, std::string& msg) {
				if(FacebookAgent::getInstance()->isLoggedIn())
				{

					btnConnectFacebook->loadTextureNormal(s_ranking_btnConnectFacebook_LogOut,TextureResType::LOCAL);
					Director::getInstance()->replaceScene(RankingScene::scene());

				}
			});

}
void RankingScene::worldModeButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		if (isRequestDone) {
			CCLog("Doi scene ne");
			UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, true);
			Director::getInstance()->replaceScene(RankingScene::scene());
		} else
			SocialPlugin::showToast(
					"We're getting your score from server.\nPlease wait for a few seconds.");
	}
}
void RankingScene::friendModeButton(Ref* pSender,
		ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {
		if (isRequestDone) {
			CCLog("Doi scene ne");
			UserDefault::getInstance()->setBoolForKey(RANKINGWORLDMODE, false);
			Director::getInstance()->replaceScene(RankingScene::scene());
		} else
			SocialPlugin::showToast(
					"We're getting your score from server.\nPlease wait for a few seconds.");
	}
}

void RankingScene::getFriendFromFacebook() {
	if (FacebookAgent::getInstance()->isLoggedIn()) {
		char url[55500];
		sprintf(url,
				"https://graph.facebook.com/v2.3/me/friends?fields=id,name&access_token=%s",
				FacebookAgent::getInstance()->getAccessToken().c_str());
		//CCLog("fullurl get friend: %s, facebookid: %s", url,myFacebookID.c_str());
		vector < string > header;
		header.push_back("Content-Type: application/json; charset=UTF-8");
		header.push_back("HTTP/1.1");

		HttpRequest* request = new HttpRequest();
		request->setUrl(url);
		request->setHeaders(header);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(
				CC_CALLBACK_2(RankingScene::getFriendFromFacebookCallback, this));
		HttpClient::getInstance()->send(request);

		request->release();

	}
}
void RankingScene::getFriendFromFacebookCallback(HttpClient* client,
		HttpResponse* response) {

	if (response->isSucceed()) {
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;
		CCLog("Get friend from facebook ne: %s", clearData.c_str());

		rapidjson::Document document;
		document.Parse<0>(clearData.c_str());
		const rapidjson::Value& users = document["data"];
		if (users.Size() > 0)
			for (rapidjson::SizeType i = 0; i < users.Size(); i++) {
				const rapidjson::Value& user = users[i];
				string id(user["id"].GetString());
				allFriendsFacebookID = allFriendsFacebookID + "\"" + id + "\",";
			}

		allFriendsFacebookID = allFriendsFacebookID + "\""
				+ FacebookAgent::getInstance()->getUserID() + "\"";

		CCLog("Sap vao get score from server");
		getScoreFromServer();

	} else {
		labelError->setVisible(true);
		loadingSprite->setVisible(false);
		loadingSprite_child->setVisible(false);
		isRequestDone = true;
	}
}

void RankingScene::getMyInfoFromFacebook() {
	allFriendsFacebookID = "";

	if (FacebookAgent::getInstance()->isLoggedIn()) {
		char url[55500];
		sprintf(url,
				"https://graph.facebook.com/v2.3/me?fields=id,name,locale&access_token=%s",
				FacebookAgent::getInstance()->getAccessToken().c_str());
		vector < string > header;
		header.push_back("Content-Type: application/json; charset=UTF-8");
		header.push_back("HTTP/1.1");

		HttpRequest* request = new HttpRequest();
		request->setUrl(url);
		request->setHeaders(header);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(
				CC_CALLBACK_2(RankingScene::getMyInfoFromFacebookCallback, this));
		HttpClient::getInstance()->send(request);

		request->release();

	}
}
void RankingScene::getMyInfoFromFacebookCallback(HttpClient* client,
		HttpResponse* response) {

	if (response->isSucceed()) {
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;
		CCLog("Get myinfo from facebook ne: %s", clearData.c_str());

		rapidjson::Document document;
		document.Parse<0>(clearData.c_str());

		myFacebookID = document["id"].GetString();
		myFacebookName = document["name"].GetString();
		UserDefault::getInstance()->setStringForKey("MY_FACEBOOK_NAME",
				myFacebookName);
		myFacebookLocation = document["locale"].GetString();

		CCLog("Sap chay vo save facebook");
		saveMyFacebookIDToServer();
	} else {
		labelError->setVisible(true);
		loadingSprite->setVisible(false);
		loadingSprite_child->setVisible(false);
		isRequestDone = true;
	}

}
void RankingScene::getScoreFromServer() {
	CCLog("Vao ham get score from server");

	char url[55500];
	if (!isWorldMode) {
		char whereUrl[55000];
		sprintf(whereUrl, "where={\"FB_ID\":{\"$in\":[%s]}}",
				allFriendsFacebookID.c_str());
		CURL *curl = curl_easy_init();
		char * encodeUrl = curl_easy_escape(curl, whereUrl, 0);
		sprintf(url, "https://api.parse.com/1/classes/RedHood?%s&order=-Score",
				encodeUrl);
	} else
		sprintf(url,
				"https://api.parse.com/1/classes/RedHood?&order=-Score");

	CCLog("getscore: urlfull: %s", url);
	vector < string > h;
	h.push_back(
			"X-Parse-Application-Id: hYf8HYrrrvZTdUligYAdy5lKtqrzHg7i8XMR8Jig");
	h.push_back(
			"X-Parse-REST-API-Key: bVFb1yUe4n6NX4oSzW9UCy1FqwR1HsMjnA5EfGqs");
	h.push_back("Content-Type: application/json");
	HttpRequest* r = new HttpRequest();
	r->setUrl(url);
	r->setHeaders(h);
	r->setRequestType(HttpRequest::Type::GET);
	r->setResponseCallback(this,
			httpresponse_selector(RankingScene::getScoreFromServerCallback));
	HttpClient::getInstance()->send(r);

	r->release();
}
void RankingScene::getScoreFromServerCallback(HttpClient* client,
		HttpResponse* response) {
	//CCLog("Callback get score from server");
	if (response->isSucceed()) {
		//CCLog("callback sucess luôn");
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;

		rapidjson::Document d;
		d.Parse<0>(clearData.c_str());
		const rapidjson::Value& mangJson = d["results"];
		if (mangJson.Size() > 0) {
			for (rapidjson::SizeType i = 0; i < mangJson.Size(); i++) {
				const rapidjson::Value& motDoiTuong = mangJson[i];
				CCLog("Add vtscore");
				vt_ScoreServer.push_back(motDoiTuong["Score"].GetInt());
				CCLog("Add vtname");
				vt_NameServer.push_back(motDoiTuong["FB_Name"].GetString());
			}
		}

		//	CCLog("Sap show len giao dien neeeee");
		showScoreOnScreen();
	} else {
		labelError->setVisible(true);
	}
	loadingSprite->setVisible(false);
	loadingSprite_child->setVisible(false);
	isRequestDone = true;
}

void RankingScene::showScoreOnScreen() {

	TTFConfig configUser(s_font_1, 80 * s_font_ratio);
	for (int i = 0; i < vt_NameServer.size(); i++) {
		if (i + 1 > 5)
			return;

		Color3B colorOfMyNumber = Color3B(188, 132, 75);
		Sprite* numberBackgroundSprite;
		if (vt_NameServer[i]
				== UserDefault::getInstance()->getStringForKey(
						"MY_FACEBOOK_NAME")) {
			numberBackgroundSprite = Sprite::create(s_ranking_myNumber);
			colorOfMyNumber = Color3B(248, 196, 140);

			int score = UserDefault::getInstance()->getIntegerForKey(HIGHSCORE,0);
				if(vt_ScoreServer[i] != score)
					Director::getInstance()->replaceScene(RankingScene::scene());

		} else
			numberBackgroundSprite = Sprite::create(s_ranking_number);
		numberBackgroundSprite->setPosition(
				board->getContentSize().width * 0.2f,
				board->getContentSize().height * 0.65
						- board->getContentSize().height / 10 * (i + 1));
		board->addChild(numberBackgroundSprite);

		auto lRanking = Label::createWithTTF(configUser,
				CppUtils::doubleToString(i + 1), TextHAlignment::CENTER);
		lRanking->setPosition(board->getContentSize().width * 0.2f,
				board->getContentSize().height * 0.65
						- board->getContentSize().height / 10 * (i + 1));
		lRanking->setColor(colorOfMyNumber);
		board->addChild(lRanking);

		auto lName = Label::createWithTTF(configUser, vt_NameServer[i],
				TextHAlignment::CENTER);
		lName->setPosition(board->getContentSize().width * 0.5f,
				board->getContentSize().height * 0.65
						- board->getContentSize().height / 10 * (i + 1));
		lName->setColor(Color3B(188, 132, 75));
		lName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		board->addChild(lName);

		auto lScore = Label::createWithTTF(configUser,
				CppUtils::doubleToString(vt_ScoreServer[i]),
				TextHAlignment::CENTER);
		lScore->setPosition(board->getContentSize().width * 0.8f,
				board->getContentSize().height * 0.65
						- board->getContentSize().height / 10 * (i + 1));
		lScore->setColor(Color3B(188, 132, 75));
		board->addChild(lScore);

	}
}

void RankingScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {
		auto *newScene = GameScene::scene();
		auto transition = TransitionFade::create(1.0, newScene);
		Director *pDirector = Director::getInstance();
		pDirector->replaceScene(transition);
	}
}

void RankingScene::saveMyFacebookIDToServer() {

	log("Chay ham saveuser");
	//Header for httprequest
	vector < string > h;
	h.push_back(
			"X-Parse-Application-Id: hYf8HYrrrvZTdUligYAdy5lKtqrzHg7i8XMR8Jig");
	h.push_back(
			"X-Parse-REST-API-Key: bVFb1yUe4n6NX4oSzW9UCy1FqwR1HsMjnA5EfGqs");
	h.push_back("Content-Type: application/json");

	//Post object
	auto r2 = new HttpRequest();

	r2->setUrl("https://api.parse.com/1/classes/RedHood");

	r2->setHeaders(h);

	r2->setRequestType(HttpRequest::Type::POST);

	// é€�ã‚Šã�Ÿã�„ãƒ‡ãƒ¼ã‚¿(JSONå½¢å¼�)ã‚’ã‚»ãƒƒãƒˆ
	char jsonData2[256];
	sprintf(jsonData2,
			"{\"FB_ID\":\"%s\",\"FB_Name\":\"%s\",\"FB_Location\":\"%s\", \"Score\":0}",
			myFacebookID.c_str(), myFacebookName.c_str(),
			myFacebookLocation.c_str());
	const char* buffer2 = jsonData2;
	CCLog("save user %s", buffer2);
	r2->setRequestData(buffer2, strlen(buffer2));

	r2->setResponseCallback(this,
			httpresponse_selector(RankingScene::saveMyFacebookIDToServerCallback));

	HttpClient::getInstance()->send(r2);

	//Close connection
	r2->release();

}
void RankingScene::saveMyFacebookIDToServerCallback(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {

		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;
		CCLog("Get Save my facebook ne: %s", clearData.c_str());

		rapidjson::Document d;
		d.Parse<0>(clearData.c_str());

		const rapidjson::Value& mangJson = d["results"];
		if (mangJson.Size() > 0)
			for (rapidjson::SizeType i = 0; i < mangJson.Size(); i++) {
				const rapidjson::Value& motDoiTuong = mangJson[i];
				UserDefault::getInstance()->setStringForKey("MY_OBJECT_ID",
						motDoiTuong["objectId"].GetString());

			}



		postScoreToServer();

		if (isWorldMode)
			getScoreFromServer();
		else
			getFriendFromFacebook();
	} else {
		labelError->setVisible(true);
		loadingSprite->setVisible(false);
		loadingSprite_child->setVisible(false);
		isRequestDone = true;
	}
}
void RankingScene::checkMyFacebookIDFromServer() {

//	auto delay = DelayTime::create(0.5f);
//	auto func =
//			CallFunc::create(
//					[=]() {
	char url[55500];
	char whereUrl[55000];
	sprintf(whereUrl, "where={\"FB_ID\":\"%s\"}",
			FacebookAgent::getInstance()->getUserID().c_str());
	CURL *curl = curl_easy_init();
	char * encodeUrl = curl_easy_escape(curl, whereUrl, 0);
	sprintf(url, "https://api.parse.com/1/classes/RedHood?%s", encodeUrl);

	//Header for httprequest
	vector < string > h;
	h.push_back(
			"X-Parse-Application-Id: hYf8HYrrrvZTdUligYAdy5lKtqrzHg7i8XMR8Jig");
	h.push_back(
			"X-Parse-REST-API-Key: bVFb1yUe4n6NX4oSzW9UCy1FqwR1HsMjnA5EfGqs");
	h.push_back("Content-Type: application/json");
	HttpRequest* r = new HttpRequest();
	r->setUrl(url);
	r->setHeaders(h);
	r->setRequestType(HttpRequest::Type::GET);

	// callback
	r->setResponseCallback(this,
			httpresponse_selector(RankingScene::checkMyFacebookIDFromServerCallback));

	// request
	HttpClient::getInstance()->send(r);

	r->release();

//					});
//
//	auto seq = Sequence::create(delay, func, nullptr);
//	runAction(seq);
}
void RankingScene::checkMyFacebookIDFromServerCallback(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;

		CCLog("Check facebook id Json tra ve %s", clearData.c_str());
		rapidjson::Document d;
		d.Parse<0>(clearData.c_str());
		const rapidjson::Value& mangJson = d["results"];

		postScoreToServer();

		if (mangJson.Size() == 0) {
			getMyInfoFromFacebook();
		} else
			for (rapidjson::SizeType i = 0; i < mangJson.Size(); i++) {
				const rapidjson::Value& motDoiTuong = mangJson[i];
				UserDefault::getInstance()->setStringForKey("MY_OBJECT_ID",
						motDoiTuong["objectId"].GetString());


				if (isWorldMode)
					getScoreFromServer();
				else
					getFriendFromFacebook();

			}

	} else {
		labelError->setVisible(true);
		loadingSprite->setVisible(false);
		loadingSprite_child->setVisible(false);
		isRequestDone = true;
	}
}
