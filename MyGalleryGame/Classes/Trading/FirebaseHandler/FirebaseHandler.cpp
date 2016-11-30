#include "FirebaseHandler.h"

#include "BLeaderBoardContanst.h"

#include <json/rapidjson.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/prettywriter.h>
#include <json/filestream.h>
#include <json/document.h>
#include <json/stringbuffer.h>

FirebaseHandler::FirebaseHandler() {
	FacebookHandler::getInstance()->setFacebookDelegate(this);
	scoreToSubmit = 0;
}

FirebaseHandler::~FirebaseHandler() {

}

FirebaseHandler* FirebaseHandler::getInstance() {
	static FirebaseHandler* instance;
	if (!instance) {
		instance = new FirebaseHandler();

	}
	return instance;
}

void FirebaseHandler::getProbability(string url, vector<string> probabilityKeys,
		STICKER_RARITY rarity) {
	CCLog("bambi get Probability from Firebase - calling");

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			[this, probabilityKeys, rarity](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi get Probability from Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					//Clear data (sometimes stranged characters be attached after the result)
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					size_t pos = clearData.rfind("}");
					clearData = clearData.substr(0, pos + 1);
					CCLog("bambi get Probability from Firebase: %s", clearData.c_str());
					if (clearData == "")
					return;

					//Process data
					rapidjson::Document d;
					d.Parse<0>(clearData.c_str());
					for(string key : probabilityKeys)
					{
						if(d.HasMember(key.c_str()))
						{
							string userdefaultKey = StickerHelper::getRarityString(rarity) + key;
							UserDefault::getInstance()->setIntegerForKey(userdefaultKey.c_str(), d[key.c_str()].GetInt());
						}
					}
				}
			});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::getProbabilityFreePacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Freepacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::UNKNOWN);
}

void FirebaseHandler::checkFacebookIdExistOnFirebase() {
	CCLog("bambi inside FirebaseHandler->checkFacebookIdExistOnFirebase ");
	//query string
	char query[200];
	sprintf(query, firebaseQuerry_IsThisUserExistOnFirebase.c_str(),
	KEY_WORLD_ID, FacebookHandler::getInstance()->getUserFacebookID().c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(firebaseURL.c_str(),
					CppUtils::encodeUrl(
							String::createWithFormat(
									firebaseQuerry_IsThisUserExistOnFirebase.c_str(),
									KEY_WORLD_ID,
									FacebookHandler::getInstance()->getUserFacebookID().c_str())->getCString()).c_str())->getCString());

	CCLog(
			"bambi inside FirebaseHandler->checkFacebookIdExistOnFirebase, url: %s",
			request->getUrl());

	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			CC_CALLBACK_2(
					FirebaseHandler::checkFacebookIdExistOnFirebaseCallBack,
					this));
	HttpClient::getInstance()->send(request);
	request->release();

}
void FirebaseHandler::checkFacebookIdExistOnFirebaseCallBack(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {
		//Clear data (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);

		CCLog("bambi checkFacebookIdExistOnFirebaseCallBack: %s",
				clearData.c_str());
		if (clearData.find("error") != string::npos)
			return;

		//Process data
		if (clearData.length() > 5) { //User's already been added on Firebase
			int positionOfColon = clearData.find(":");
			if (positionOfColon == string::npos) {
				return;
			}
			string objectId = clearData.substr(1, positionOfColon - 1);
			CCLog("bambi objectId: %s", objectId.c_str());
			//Save to UserDefault (your will need objectId when posting score on Firebase)
			UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
					objectId);
			if (_firebaseDelegate != nullptr)
				_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Respone to ranking scene
		} else
			FacebookHandler::getInstance()->getMyProfile();
		//After getMyProfile, responseWhenGetMyInfoSuccessfully function will be called.

	}
}
void FirebaseHandler::responseWhenGetMyInfoSuccessfully(BUserInfor* user) {
	CCLog("bambi responseWhenGetMyInfoSuccessfully, going to saveFacebookIdOnFirebase");
	saveFacebookIdOnFirebase(user);
}
void FirebaseHandler::saveFacebookIdOnFirebase(BUserInfor* user) {
	CCLog("bambi saveFacebookIdOnFirebase");
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(firebaseURL.c_str(),
					firebaseQuerry_User.c_str())->getCString());
	request->setRequestData(user->serialize().c_str(),
			user->serialize().size());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(
			CC_CALLBACK_2(FirebaseHandler::callBacksaveFacebookIdOnFirebase,
					this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::callBacksaveFacebookIdOnFirebase(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {
//Clear data (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);

		CCLog("bambi callBacksaveFacebookIdOnFirebase, %s", clearData.c_str());
		if (clearData == "")
			return;

//Save to UserDefault (your will need objectId when posting score on Firebase)
		rapidjson::Document d;
		d.Parse<0>(clearData.c_str());

		if (d.HasMember(KEY_WORLD_OJECTID)) {
			UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
					d[KEY_WORLD_OJECTID].GetString());
		}

		if (_firebaseDelegate != nullptr)
			_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Respone to home scene

	}
}

void FirebaseHandler::fetchBUserInforAt(char* querry) {
	char url[55500];
	sprintf(url, "%s?%s&order=-Score&limit=5", classURL.c_str(), querry);

	//Header for httprequest
	std::vector < std::string > header;
	header.push_back(appID);
	header.push_back(restAPI);
	header.push_back("Content-Type: application/json");

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setHeaders(header);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			CC_CALLBACK_2(FirebaseHandler::callBackFetchBUserInforAt, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::callBackFetchBUserInforAt(HttpClient* client,
		HttpResponse* response) {
	std::string error = response->getErrorBuffer();
	if (response->isSucceed() && error == "") {
//Clear old data
		_friendList.clear();
		_worldList.clear();

//Clear data that being got from Firebase (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		size_t pos = clearData.rfind("}");
		clearData = clearData.substr(0, pos + 1);
		if (clearData == "")
			return;

//Process data
		rapidjson::Document document;
		document.Parse<0>(clearData.c_str());
		if (!document["results"].IsArray())
			return;
		const rapidjson::Value& jsonArray = document["results"];
		for (int k = 0; k < jsonArray.Size(); k++) {
			BUserInfor* user = BUserInfor::parseUserFrom(jsonArray[k]);
			switch (tag) {
			case TAG_FRIEND:
				_friendList.push_back(user);
				break;
			case TAG_WORLD:
				_worldList.push_back(user);
				break;
			}
		}
	}
	// Response to RankingScene
	if (!_firebaseDelegate)
		return;
	switch (tag) {
	case TAG_FRIEND:
		_firebaseDelegate->responseForQuerryTopFriend(_friendList);
		break;
	}
}

void FirebaseHandler::fetchTopFriend() {
	FacebookHandler::getInstance()->getAllFriendsID();
	//After get friends successfully responseWhenGetFriendsSuccessfully will be called.
}
void FirebaseHandler::responseWhenGetFriendsSuccessfully(string friendList) {
	friendList += "\"" + FacebookHandler::getInstance()->getUserFacebookID()
			+ "\""; //Attach my FacebookID to friendList

	CCLog("bambi get listfriend from facebook: %s", friendList.c_str());
//	char querry[55500];
//	tag = TAG_FRIEND;
//	sprintf(querry, "where={\"FB_ID\":{\"$in\":[%s]}}", friendList.c_str());
//	fetchBUserInforAt(querry);
}

void FirebaseHandler::fetchScoreFromServer() {
	//query string
	char query[200];
	sprintf(query, "where={\"%s\":\"%s\"}", KEY_WORLD_ID,
			FacebookHandler::getInstance()->getUserFacebookID().c_str());

	//format url
	char url[55500];
	sprintf(url, "%s?%s", classURL.c_str(), query);

	//Header for httprequest
	std::vector < std::string > header;
	header.push_back(appID);
	header.push_back(restAPI);
	header.push_back("Content-Type: application/json");

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setHeaders(header);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			size_t pos = clearData.rfind("}");
			clearData = clearData.substr(0, pos + 1);
			if (clearData == "")
			return;

			//Process data
			rapidjson::Document d;
			d.Parse<0>(clearData.c_str());
			const rapidjson::Value& mangJson = d["results"];
			if (clearData.length() > 15)//User's already been added on Firebase
			{
				int score = scoreToSubmit + mangJson[0][KEY_WORLD_SCORE].GetInt();
				putScoreToSever(score);
			}

		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
void FirebaseHandler::putScoreToSever(int score) {
	//Set url
	char url[55500];
	sprintf(url, "%s/%s", classURL.c_str(),
			UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID, "").c_str());

	//Set score data
	char data[100];
	sprintf(data, "{\"%s\":%d}", KEY_WORLD_SCORE, score);
	string dataStr(data);

	//Header for httprequest
	std::vector < std::string > header;
	header.push_back(appID);
	header.push_back(restAPI);
	header.push_back("Content-Type: application/json");

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setHeaders(header);
	request->setRequestData(dataStr.c_str(), dataStr.size());
	request->setRequestType(HttpRequest::Type::PUT);
	HttpClient::getInstance()->send(request);
	request->release();
}
void FirebaseHandler::submitScore(int score) {
	scoreToSubmit = score;
	fetchScoreFromServer();
}
