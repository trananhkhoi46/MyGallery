
#ifndef __FacebookHandler__
#define __FacebookHandler__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "BUserInfor.h"
#include "PluginFacebook/PluginFacebook.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include <curl/include/ios/curl/curl.h>
#include <curl/include/android/curl/curl.h>
USING_NS_CC;
using namespace cocos2d::network;
using namespace std;
using namespace rapidjson;
using namespace cocos2d::ui;


class FacebookDelegate{
public:
    virtual void responseWhenGetFriendsSuccessfully(string friendList){
    };
    virtual void responseWhenGetMyInfoSuccessfully(BUserInfor* user){
	};
};

class FacebookConnectDelegate{
public:
    virtual void responseWhenLoginOrLogoutFacebook(){
    };
};


class FacebookHandler : public Node, sdkbox::FacebookListener{
public:
    FacebookHandler();
    ~FacebookHandler();
    static FacebookHandler* getInstance();

    void loginFacebook();
    void logoutFacebook();
    bool isFacebookLoggedIn();

    void getMyProfile();
    void getAllFriendsID();
    string getUserFacebookID();
    CC_SYNTHESIZE(FacebookDelegate*,_facebookDelegate,FacebookDelegate);
    CC_SYNTHESIZE(FacebookConnectDelegate*,_facebookConnectDelegate,FacebookConnectDelegate);

    //Facebook callback
    void onLogin(bool isLogin, const std::string& msg);
     void onSharedSuccess(const std::string& message);
     void onSharedFailed(const std::string& message);
     void onSharedCancel();
     void onAPI(const std::string& key, const std::string& jsonData);
     void onPermission(bool isLogin, const std::string& msg);
     void onFetchFriends(bool ok, const std::string& msg);
};

#endif
