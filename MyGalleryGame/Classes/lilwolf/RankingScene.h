#ifndef __RANKING_SCENE_H__
#define __RANKING_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include <curl/include/ios/curl/curl.h>
#include <curl/include/android/curl/curl.h>
using namespace cocos2d::network;
using namespace std;
using namespace rapidjson;
using namespace cocos2d::ui;
USING_NS_CC;

class RankingScene: public cocos2d::Layer{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	cocos2d::Size winSize;
	cocos2d::Point origin;
	CREATE_FUNC(RankingScene)


		void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
		void backToHome(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void playAgain(Ref* pSender,ui::Widget::TouchEventType eEventType);



		//Ranking
		void worldModeButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void friendModeButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void facebookConnectButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void loginFacebook();
		void logoutFacebook();
		void getFriendFromFacebook();
		void getMyInfoFromFacebook();
		void getScoreFromServer();
		void checkMyFacebookIDFromServer();
		void saveMyFacebookIDToServer();
		void getFriendFromFacebookCallback(HttpClient* client,HttpResponse* response);
		void getMyInfoFromFacebookCallback(HttpClient* client,HttpResponse* response);
		void getScoreFromServerCallback(HttpClient* client,HttpResponse* response);
		void checkMyFacebookIDFromServerCallback(HttpClient* client,HttpResponse* response);
		void saveMyFacebookIDToServerCallback(HttpClient* client,HttpResponse* response);
		void showScoreOnScreen();

		string myFacebookID;
		string myFacebookName;
		string myFacebookLocation;
		string allFriendsFacebookID;
		vector<int> vt_ScoreServer;
		vector<string> vt_NameServer;
		bool isWorldMode;

		Sprite* loadingSprite;
		Sprite* loadingSprite_child;
		Sprite* board;
		Button* btnConnectFacebook;
		Label* labelError;
		bool isRequestDone;


		void postScoreToServer();
		void postScoreToServerCallBack(HttpClient* client, HttpResponse* response);
};
#endif
