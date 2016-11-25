#ifndef __TRADING_SCENE_H__
#define __TRADING_SCENE_H__

#include "BaseScene.h"

class TradingScene: public BaseScene, public FacebookConnectDelegate, public FirebaseDelegate{
public:
		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init();

		// there's no 'id' in cpp, so we recommend returning the class instance pointer
		static cocos2d::Scene* scene();
		cocos2d::Size winSize;
		cocos2d::Point origin;
		CREATE_FUNC(TradingScene)
		void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
		void backToHome(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void playAgain(Ref* pSender,ui::Widget::TouchEventType eEventType);



		void worldModeButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void friendModeButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void facebookConnectButton(Ref* pSender,ui::Widget::TouchEventType eEventType);
		void loadLeaderboardData();
		void showScoreOnScreen(vector<BUserInfor*> listUser);
		void updateUI(int tag);
		void resetTradingScene();


		bool isWorldMode;
		bool isGettingData;
		Sprite* board;
		Button* btnConnectFacebook;
		Button* btnWorldMode;
		Button* btnFriendMode;
		Sprite* loadingSprite;
		Label* labelError;

		//Responsed function
		virtual void responseWhenLoginOrLogoutFacebook(); //From FacebookHandler
		virtual void responseForQuerryTopWorld(vector<BUserInfor*> worldList); //From FirebaseHandler
		virtual void responseForQuerryTopFriend(vector<BUserInfor*> worldList); //From FirebaseHandler
		virtual void responseAfterCheckFacebookIdExistOnFirebase(); //From FirebaseHandler
};
#endif
