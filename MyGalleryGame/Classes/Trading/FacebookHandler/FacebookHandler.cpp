//
//  FacebookHandler.cpp
//  SonTinhThuyTinh
//
//  Created by cao Ky Han on 6/18/15.
//
//

#include "FacebookHandler.h"

#include "../FirebaseHandler/FirebaseHandler.h"
#include "BLeaderBoardContanst.h"
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
	sdkbox::PluginFacebook::requestReadPermissions( { "public_profile",
			"user_friends" }); //Including login

}
void FacebookHandler::logoutFacebook() {
	sdkbox::PluginFacebook::logout();
	if (_facebookConnectDelegate != nullptr)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
}
bool FacebookHandler::isFacebookLoggedIn() {
	if (sdkbox::PluginFacebook::isLoggedIn())
		return true;
	else
		return false;
}

bool isGettingMyProfle = false;
void FacebookHandler::getMyProfile() {
	isGettingMyProfle = true;
	CCLog("bambi FacebookHandler getMyProfile");
	sdkbox::FBAPIParam params;
	params["fields"] = "name,id";
	sdkbox::PluginFacebook::api("/me", "GET", params, "/me");
	//FacebookHandler::onAPI will be involked next
}

/*********************
 * Facebook callbacks
 *********************/
void FacebookHandler::onLogin(bool isLogin, const std::string& error) {
	CCLog("##FB onLogin success");
	if (_facebookConnectDelegate != nullptr && isLogin)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
}
void FacebookHandler::onAPI(const std::string& tag,
		const std::string& jsonData) {
	CCLog("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
	if (isGettingMyProfle) {
		BUserInfor* user = BUserInfor::parseUserFrom(jsonData);
		if (_facebookDelegate != nullptr) {
			_facebookDelegate->responseWhenGetMyInfoSuccessfully(user);
		}
		isGettingMyProfle = false;
	}
}
void FacebookHandler::onSharedSuccess(const std::string& message) {
	CCLog("##FB onSharedSuccess:%s", message.c_str());

	MessageBox(message.c_str(), "share success");
}
void FacebookHandler::onSharedFailed(const std::string& message) {
	CCLog("##FB onSharedFailed:%s", message.c_str());

	MessageBox(message.c_str(), "share failed");
}
void FacebookHandler::onSharedCancel() {
	CCLog("##FB onSharedCancel");
}
void FacebookHandler::onPermission(bool isLogin, const std::string& error) {
	if (_facebookConnectDelegate != nullptr && isLogin)
		_facebookConnectDelegate->responseWhenLoginOrLogoutFacebook();
}
void FacebookHandler::onFetchFriends(bool ok, const std::string& msg) {
	CCLog("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());

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
	CCLog("##FB onRequestInvitableFriends");
}
void FacebookHandler::onInviteFriendsWithInviteIdsResult(bool result,
		const std::string& msg) {
	CCLog("##FB onInviteFriendsWithInviteIdsResult");
}
void FacebookHandler::onInviteFriendsResult(bool result,
		const std::string& msg) {
	CCLog("##FB onInviteFriendsResult");
}
void FacebookHandler::onGetUserInfo(const sdkbox::FBGraphUser& userInfo) {
	if (isGettingMyProfle) {
		CCLog("##FB onGetUserInfo, name: %s, id: %s",
				userInfo.getName().c_str(), userInfo.getUserId().c_str());
		BUserInfor* user = new BUserInfor();
		user->setId(userInfo.getUserId());
		user->setName(userInfo.getName());
		if (_facebookDelegate != nullptr) {
			_facebookDelegate->responseWhenGetMyInfoSuccessfully(user);
		}
		isGettingMyProfle = false;
	}
}
