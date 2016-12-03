#include "BUserInfor.h"
#include "cocos2d.h"
BUserInfor::BUserInfor() {

}

BUserInfor::~BUserInfor() {

}

BUserInfor* BUserInfor::parseUserFrom(const rapidjson::Value& json) //Parse the data from Firebase
		{
	BUserInfor* user = new BUserInfor();
	user->setName(json[KEY_WORLD_NAME].GetString());
	user->setId(json[KEY_WORLD_ID].GetString());
	user->setAllStickers(json[KEY_WORLD_ALL_STICKERS].GetString());
	user->setStickedStickers(json[KEY_WORLD_STICKED_STICKERS].GetString());
	user->setObjectId("");
	return user;
}

BUserInfor* BUserInfor::parseUserFrom(string json, string facebookIDToCheck) //Parse the data from Firebase
		{
	cocos2d::CCLog("bambi in BUserInfor parser");
	BUserInfor* user = new BUserInfor();
	//Process data
	rapidjson::Document document;
	document.Parse<0>(json.c_str());
	for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
			itr != document.MemberEnd(); ++itr) {
		string facebookId =
				document[itr->name.GetString()][KEY_WORLD_ID].GetString();
		if (facebookId.find(facebookIDToCheck) != std::string::npos) {
			user->setId(facebookId);
			user->setAllStickers(
					document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
			user->setStickedStickers(
								document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
			user->setObjectId(itr->name.GetString());
			user->setName(
					document[itr->name.GetString()][KEY_WORLD_NAME].GetString());
			cocos2d::CCLog("bambi in BUserInfor parser, objectId: %s", user->getObjectId().c_str());
			break;
		}
	}
	return user;
}

vector<BUserInfor*> BUserInfor::parseListUserFrom(string json) //Parse the data from Firebase
		{
	cocos2d::CCLog("bambi in BUserInfor parser");
	vector<BUserInfor*> result;
	//Process data
	rapidjson::Document document;
	document.Parse<0>(json.c_str());
	for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
			itr != document.MemberEnd(); ++itr) {
		BUserInfor* user = new BUserInfor();
		user->setId(document[itr->name.GetString()][KEY_WORLD_ID].GetString());
		user->setAllStickers(
				document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
		user->setStickedStickers(
					document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
			user->setObjectId(itr->name.GetString());
		user->setName(
				document[itr->name.GetString()][KEY_WORLD_NAME].GetString());
		result.push_back(user);

		cocos2d::CCLog("bambi in BUserInfor parser, objectId: %s", user->getObjectId().c_str());
	}
	return result;
}

BUserInfor* BUserInfor::parseUserFrom(string message) //Parse the message from Facebook
		{
	BUserInfor* user = new BUserInfor();

	rapidjson::Document document;
	document.Parse<0>(message.c_str());

	user->setName(document["name"].GetString());
	user->setId(document["id"].GetString());
	user->setStickedStickers("");
	user->setAllStickers("");
	user->setObjectId("");
	return user;
}
