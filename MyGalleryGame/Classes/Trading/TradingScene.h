#ifndef __TRADING_SCENE_H__
#define __TRADING_SCENE_H__

#include "BaseScene.h"

class TradingScene: public BaseScene{
public:
		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init();

		// there's no 'id' in cpp, so we recommend returning the class instance pointer
		static cocos2d::Scene* scene(BUserInfor* userInfor);
		CREATE_FUNC(TradingScene)
		void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
		void backToHome(Ref* pSender,ui::Widget::TouchEventType eEventType);

		//Responsed function
//		virtual void responseForQuerryTopFriend(vector<BUserInfor*> worldList); //From FirebaseHandler
//		virtual void responseAfterCheckFacebookIdExistOnFirebase(); //From FirebaseHandler
//		virtual void responseAfterGetStickersDataFromFirebase(string facebookId, string stickerData); //From FirebaseHandler
};
#endif
