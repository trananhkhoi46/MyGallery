#ifndef __FirebaseHandler__
#define __FirebaseHandler__
#include <cocos/network/HttpClient.h>
#include <cocos/network/HttpResponse.h>
#include "../Model/BUserInfor.h"
#include "../Model/PendingRequest.h"
#include "../FacebookHandler/FacebookHandler.h"
#include "../../Constants/Constants.h"
#include "../../Constants/Resources.h"
#include "../../Constants/StickerResource.h"
#include "../../Helper/StickerHelper.h"
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

	virtual void responseAfterDenyingRequest(bool isSuccess) {

	}
	;

	virtual void responseAfterAcceptingRequest(bool isSuccess) {

	}
	;

	virtual void responseAfterCheckingPendingRequest(
			vector<PendingRequest*> vtPendingRequest) {

	}
	;

	virtual void responseAfterCheckingGivenSticker(
			vector<PendingRequest*> vtGivenStickers) {

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

	void checkPendingRequest();

	void checkGivenStickers();

	void getStickersDataFromFirebase();
	void getStickersDataFromFirebase(string objectID);

	void acceptSendingSticker(vector<PendingRequest*> vtPendingRequest,
			PendingRequest* request);

	void denySendingSticker(vector<PendingRequest*> vtPendingRequest,
			PendingRequest* request);

	void acceptReceivingSticker(vector<PendingRequest*> vtPendingRequest,
			PendingRequest* request);

	void getProbabilityFreePacket();
	void getProbabilityCommonPacket();
	void getProbabilityUncommonPacket();
	void getProbabilityRarePacket();
	void getProbabilityVeryRarePacket();
	void getProbabilityRarestPacket();
	void getProbability(string url, vector<string> probabilityKeys,
			STICKER_RARITY rarity);

	void askTheStickerOfUer(int stickerId, BUserInfor* user);

	string friendList;

	//Facebook delegate
	virtual void responseWhenGetFriendsSuccessfully(string friendList);
	virtual void responseWhenGetMyInfoSuccessfully(BUserInfor* user);
};

#endif
