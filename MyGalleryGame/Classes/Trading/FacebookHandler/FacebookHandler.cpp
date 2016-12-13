//
//  FacebookHandler.cpp
//  SonTinhThuyTinh
//
//  Created by cao Ky Han on 6/18/15.
//
//

#include "FacebookHandler.h"

#include "../FirebaseHandler/FirebaseHandler.h"
#include "BUserInfor.h"
FacebookHandler::FacebookHandler() {

}

FacebookHandler::~FacebookHandler() {

}

FacebookHandler* FacebookHandler::getInstance() {
	static FacebookHandler* instance;
	if (instance == nullptr) {
		instance = new FacebookHandler();
		sdkbox::PluginFacebook::setListener(instance);
	}
	return instance;
}
string FacebookHandler::getUserFacebookID() {
	return sdkbox::PluginFacebook::getUserID();
}
void FacebookHandler::getAllFriendsID() {
	sdkbox::PluginFacebook::fetchFriends();
}

void FacebookHandler::loginFacebook() {
	CCLog("bambi FacebookHandler -> loginFacebook");
	sdkbox::PluginFacebook::requestReadPermissions( { "public_profile",
			"user_friends" }); //Including login
	UserDefault::getInstance()->setBoolForKey(KEY_FACEBOOK_FIRST_TIME_LOGGING_IN, true);
}
void FacebookHandler::logoutFacebook() {
	sdkbox::PluginFacebook::logout();
	if (_facebookConnectDelegate != nullptr)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
	UserDefault::getInstance()->setBoolForKey(KEY_FACEBOOK_FIRST_TIME_LOGGING_IN, true);
}

bool FacebookHandler::isFacebookFirstTimeLoggingIn()
{
	return UserDefault::getInstance()->getBoolForKey(KEY_FACEBOOK_FIRST_TIME_LOGGING_IN, true);
}

bool FacebookHandler::isFacebookLoggedIn() {
	bool isLoggedIn = sdkbox::PluginFacebook::isLoggedIn();
	return isLoggedIn;
}

bool isGettingMyProfle = false;
void FacebookHandler::getMyProfile() {
	isGettingMyProfle = true;
	CCLog("bambi FacebookHandler -> getMyProfile");
	sdkbox::FBAPIParam params;
	params["fields"] = "name,id";
	sdkbox::PluginFacebook::api("/me", "GET", params, "/me");
	//FacebookHandler::onAPI will be involked next
}

/*********************
 * Facebook callbacks
 *********************/
void FacebookHandler::onLogin(bool isLogin, const std::string& error) {
	CCLog("bambi FacebookHandler -> onLogin success");
	if (_facebookConnectDelegate != nullptr && isLogin)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
}
void FacebookHandler::onAPI(const std::string& tag,
		const std::string& jsonData) {
	CCLog("bambi FacebookHandler -> onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
	if (isGettingMyProfle) {
		BUserInfor* user = BUserInfor::parseUserFrom(jsonData);
		user->setAllStickers(UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, ""));
		UserDefault::getInstance()->setStringForKey(KEY_WORLD_NAME, user->getName());
		if (_facebookDelegate != nullptr) {
			_facebookDelegate->responseWhenGetMyInfoSuccessfully(user);
		}
		isGettingMyProfle = false;
	}
}
void FacebookHandler::onSharedSuccess(const std::string& message) {
	CCLog("bambi FacebookHandler -> onSharedSuccess:%s", message.c_str());

	MessageBox(message.c_str(), "share success");
}
void FacebookHandler::onSharedFailed(const std::string& message) {
	CCLog("bambi FacebookHandler -> onSharedFailed:%s", message.c_str());

	MessageBox(message.c_str(), "share failed");
}
void FacebookHandler::onSharedCancel() {
	CCLog("bambi FacebookHandler -> onSharedCancel");
}
void FacebookHandler::onPermission(bool isLogin, const std::string& error) {
	if (_facebookConnectDelegate != nullptr && isLogin)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
}
void FacebookHandler::onFetchFriends(bool ok, const std::string& msg) {
	CCLog("bambi FacebookHandler -> %s: %d = %s", __FUNCTION__, ok, msg.data());

	string friendList = "";
	const std::vector<sdkbox::FBGraphUser>& friends =
			sdkbox::PluginFacebook::getFriends();
	for (int i = 0; i < friends.size(); i++) {
		const sdkbox::FBGraphUser& user = friends.at(i);
		friendList = friendList + "\"" + user.uid.data() + "\",";
	}

	if (_facebookDelegate != nullptr)
		_facebookDelegate->responseWhenGetFriendsSuccessfully(friendList);
}
void FacebookHandler::onRequestInvitableFriends(
		const sdkbox::FBInvitableFriendsInfo& friends) {
	CCLog("bambi FacebookHandler -> onRequestInvitableFriends");
}
void FacebookHandler::onInviteFriendsWithInviteIdsResult(bool result,
		const std::string& msg) {
	CCLog("bambi FacebookHandler -> onInviteFriendsWithInviteIdsResult");
}
void FacebookHandler::onInviteFriendsResult(bool result,
		const std::string& msg) {
	CCLog("bambi FacebookHandler -> onInviteFriendsResult");
}
void FacebookHandler::onGetUserInfo(const sdkbox::FBGraphUser& userInfo) {
	if (isGettingMyProfle) {
		CCLog("bambi FacebookHandler -> onGetUserInfo, name: %s, id: %s",
				userInfo.getName().c_str(), userInfo.getUserId().c_str());
		BUserInfor* user = new BUserInfor();
		user->setId(userInfo.getUserId());
		user->setName(userInfo.getName());
		user->setAllStickers(UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, ""));
		UserDefault::getInstance()->setStringForKey(KEY_WORLD_NAME, user->getName());
		if (_facebookDelegate != nullptr) {
			_facebookDelegate->responseWhenGetMyInfoSuccessfully(user);
		}
		isGettingMyProfle = false;
	}
}
