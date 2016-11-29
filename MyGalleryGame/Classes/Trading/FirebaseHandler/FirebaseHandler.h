
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
#include <iostream>

USING_NS_CC;
using namespace cocos2d::network;
using namespace std;

enum TAG{
    TAG_FRIEND,TAG_WORLD
};




class FirebaseDelegate{
public:
    virtual void responseForQuerryTopFriend(vector<BUserInfor*> friendList){

    };
    virtual void responseForQuerryTopWorld(vector<BUserInfor*> worldList){

    };
    virtual void responseAfterCheckFacebookIdExistOnFirebase(){

    };
};


class FirebaseHandler: public FacebookDelegate
{
private:
    TAG tag;
public:
    FirebaseHandler();
    ~FirebaseHandler();
    CC_SYNTHESIZE(FirebaseDelegate*,_firebaseDelegate,FirebaseDelegate);
    CC_SYNTHESIZE(vector<BUserInfor*>,_worldList,WorldList);
    CC_SYNTHESIZE(vector<BUserInfor*>,_friendList,FriendList);

    static FirebaseHandler* getInstance();
    void checkFacebookIdExistOnFirebase();
    void checkFacebookIdExistOnFirebaseCallBack(HttpClient* client,HttpResponse* response);

    void saveFacebookIdOnFirebase(BUserInfor* user);
    void callBacksaveFacebookIdOnFirebase(HttpClient* client,HttpResponse* response);

    void fetchBUserInforAt(char* querry);
    void callBackFetchBUserInforAt(HttpClient* client,HttpResponse* response);

    void fetchTopFriend();

    void fetchTopWorld();

    void getProbabilityFreePacket();
    void getProbability(string url, vector<string> probabilityKeys, STICKER_RARITY rarity);

    void submitScore(int score);
    void fetchScoreFromServer();
    void putScoreToSever(int score);
    int scoreToSubmit;

    //Facebook delegate
    virtual void responseWhenGetFriendsSuccessfully(string friendList);
    virtual void responseWhenGetMyInfoSuccessfully(BUserInfor* user);
};

#endif
