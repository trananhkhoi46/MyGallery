#ifndef __FirebaseHandler__
#define __FirebaseHandler__
#include <cocos/network/HttpClient.h>
#include <cocos/network/HttpResponse.h>
#include "../Model/BUserInfor.h"
#include "../FacebookHandler/FacebookHandler.h"
#include "../../Constants.h"
#include "../../Resources.h"
#include "../../StickerResource.h"
#include "../../StickerHelper.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include <iostream>

USING_NS_CC;
using namespace cocos2d::network;
using namespace std;
using namespace rapidjson;

enum TAG {
	TAG_FRIEND, TAG_WORLD
};

class FirebaseDelegate {
public:
	virtual void responseForQuerryTopFriend(vector<BUserInfor*> friendList) {

	}
	;
	virtual void responseAfterCheckFacebookIdExistOnFirebase() {

	}
	;

	virtual void responseAfterGetStickersDataFromFirebase(string facebookId,
			string stickerData, string stickedData) {

	}
	;
};

class FirebaseTradeFeatureDelegate {
public:
	virtual void responseAfterAskingSticker(int stickerId, bool isSuccess) {

	}
	;

};

class FirebaseHandler: public FacebookDelegate {
private:
	TAG tag;
public:
	FirebaseHandler();
	~FirebaseHandler();CC_SYNTHESIZE(FirebaseDelegate*,_firebaseDelegate,FirebaseDelegate)
	;CC_SYNTHESIZE(vector<BUserInfor*>,_worldList,WorldList)
	;CC_SYNTHESIZE(vector<BUserInfor*>,_friendList,FriendList)
	;CC_SYNTHESIZE(FirebaseTradeFeatureDelegate*,_firebaseTradeFeatureDelegate,FirebaseTradeFeatureDelegate)
	;

	static FirebaseHandler* getInstance();
	void checkFacebookIdExistOnFirebase();
	void checkFacebookIdExistOnFirebaseCallBack(HttpClient* client,
			HttpResponse* response);

	void saveFacebookIdOnFirebase(BUserInfor* user);
	void callBacksaveFacebookIdOnFirebase(HttpClient* client,
			HttpResponse* response);

	void fetchFriendsFromFirebase(string friendList);
	void callBackFetchFriendsFromFirebase(HttpClient* client,
			HttpResponse* response);

	void fetchFriendsFromFacebook();

	void saveToMyStickedStickerList(string stickerIdString);

	void saveToMyStickedStickerList(string objectID, string stickerIdString);

	void saveToMyStickerList(string stickerIdString);

	void saveToMyStickerList(string objectID, string stickerIdString);

	void getStickersDataFromFirebase();
	void getStickersDataFromFirebase(string objectID);

	void getProbabilityFreePacket();
	void getProbability(string url, vector<string> probabilityKeys,
			STICKER_RARITY rarity);

	void askTheStickerOfUer(int stickerId, string objectIdOfUserWantToAsk);

	void submitScore(int score);
	void fetchScoreFromServer();
	void putScoreToSever(int score);
	int scoreToSubmit;
	string friendList;

	//Facebook delegate
	virtual void responseWhenGetFriendsSuccessfully(string friendList);
	virtual void responseWhenGetMyInfoSuccessfully(BUserInfor* user);
};

#endif
