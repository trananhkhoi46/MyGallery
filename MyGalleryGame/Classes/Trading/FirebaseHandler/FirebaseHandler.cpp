#include "FirebaseHandler.h"

#include "BLeaderBoardContanst.h"

#include <json/rapidjson.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/prettywriter.h>
#include <json/filestream.h>
#include <json/document.h>
#include <json/stringbuffer.h>
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

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
void FirebaseHandler::acceptSendingSticker(
		vector<PendingRequest*> vtPendingRequest, PendingRequest* request) {
	//Clone request
	PendingRequest* newRequest = new PendingRequest();
	newRequest->setName(request->getName());
	newRequest->setObjectId(request->getObjectId());
	newRequest->setStickerId(request->getStickerId());

	//Remove this element from PendingRequest of logged in user
	denySendingSticker(vtPendingRequest, request);

	//Save this element to GivenStickers of request's user
	//Get pendingRequest from server
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ request->getObjectId()
					+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	//Request http
	HttpRequest* httpRequest = new HttpRequest();
	httpRequest->setUrl(url.c_str());
	httpRequest->setRequestType(HttpRequest::Type::GET);
	httpRequest->setResponseCallback([this, request](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi acceptSendingSticker on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi acceptSendingSticker on Firebase callback: %s", clearData.c_str());

			//Process data
			string pendingRequest = clearData.substr(1, clearData.length() - 1);

								std::string::size_type i2 = pendingRequest.find("\"");
								CCLog("bambi checkPendingRequest: i2 - %d", i2);

								if (i2 != std::string::npos) {
									CCLog("bambi checkPendingRequest: found i2 - %d", i2);

									pendingRequest = pendingRequest.substr(0, i2);
								}

			CCLog("bambi acceptSendingSticker - givenStickers: %s", pendingRequest.c_str());

			//Then add a new pending request to the string and put it to server
			string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + request->getObjectId()
			+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

			CCLog("bambi acceptSendingSticker Firebase 2 - stickerId: %s, url: %s",
					request->getStickerId().c_str(), url.c_str());

			string dataString;
			if(pendingRequest == "")
			{
				dataString = "\""+UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID)+ "#"+UserDefault::getInstance()->getStringForKey(KEY_WORLD_NAME)+"#"+request->getStickerId() + "\"";
			} else
			{
				dataString = "\""+pendingRequest+"," +UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID)+ "#"+UserDefault::getInstance()->getStringForKey(KEY_WORLD_NAME)+"#"+request->getStickerId() + "\"";
			}

			//Request http
			HttpRequest* httpRequest = new HttpRequest();
			httpRequest->setUrl(url.c_str());
			httpRequest->setRequestType(HttpRequest::Type::PUT);
			httpRequest->setRequestData(dataString.c_str(), dataString.length());
			httpRequest->setResponseCallback([this, request](HttpClient* client,
							HttpResponse* response) {
						CCLog("bambi acceptSendingSticker on Firebase 2 - responding: %s",
								response->isSucceed() ? "success" : "failed");
						if (response->isSucceed()) {
							//Clear data (sometimes stranged characters be attached after the result)
							std::vector<char> *buffer = response->getResponseData();
							const char *data = reinterpret_cast<char *>(&(buffer->front()));
							std::string clearData(data);
							CCLog("bambi acceptSendingSticker on Firebase 2 callback: %s", clearData.c_str());

							//Remove the sticker from list
							string newCurrentStickerStringAfterRemoving =
							UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, "");
							std::string::size_type i = newCurrentStickerStringAfterRemoving.find(request->getStickerId());
							if (i != std::string::npos) {
								newCurrentStickerStringAfterRemoving.erase(i, request->getStickerId().length());
							}

							newCurrentStickerStringAfterRemoving = CppUtils::replaceString(newCurrentStickerStringAfterRemoving, ",,", ",");


							CCLog("bambi acceptSendingSticker on Firebase 2 callback: string sticker after removing: %s",
									UserDefault::getInstance()->getStringForKey(CURRENT_STICKER).c_str());

							UserDefault::getInstance()->setStringForKey(CURRENT_STICKER, newCurrentStickerStringAfterRemoving);

							string url =
							"https://gallerygame-fab40.firebaseio.com/json/users/" + UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID)
							+ "/All_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

							CCLog("bambi acceptSendingSticker on Firebase 2 call back - stickerId: %s, url: %s",
									request->getStickerId().c_str(), url.c_str());

							string dataString = "\"" + newCurrentStickerStringAfterRemoving + "\"";
							//Request http
							HttpRequest* httpRequest = new HttpRequest();
							httpRequest->setUrl(url.c_str());
							httpRequest->setRequestType(HttpRequest::Type::PUT);
							httpRequest->setRequestData(dataString.c_str(), dataString.length());
							httpRequest->setResponseCallback([this](HttpClient* client,
											HttpResponse* response) {
										CCLog("bambi acceptSendingSticker on Firebase 3 - responding: %s",
												response->isSucceed() ? "success" : "failed");
										if (response->isSucceed()) {
											//Clear data (sometimes stranged characters be attached after the result)
											std::vector<char> *buffer = response->getResponseData();
											const char *data = reinterpret_cast<char *>(&(buffer->front()));
											std::string clearData(data);
											size_t pos = clearData.rfind("}");
											clearData = clearData.substr(0, pos + 1);
											CCLog("bambi acceptSendingSticker on Firebase 3 callback: %s", clearData.c_str());

											if (_firebaseTradeFeatureDelegate != nullptr) {
												_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(true);
											}

										} else
										{
											if (_firebaseTradeFeatureDelegate != nullptr) {
												_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
											}
										}
									});
							HttpClient::getInstance()->send(httpRequest);
							httpRequest->release();
						} else {
							if (_firebaseTradeFeatureDelegate != nullptr) {
								_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
							}
						}
					});
			HttpClient::getInstance()->send(httpRequest);
			httpRequest->release();
		}
	});
	HttpClient::getInstance()->send(httpRequest);
	httpRequest->release();
}

void FirebaseHandler::denySendingSticker(
		vector<PendingRequest*> vtPendingRequest, PendingRequest* request) {
	//Remove the request from vtRequest
	vtPendingRequest.erase(
			std::remove(vtPendingRequest.begin(), vtPendingRequest.end(),
					request), vtPendingRequest.end());

	//Reupload data
	string pendingRequestData = "\"";
	for (PendingRequest* record : vtPendingRequest) {
		if (pendingRequestData == "\"") {
			pendingRequestData += record->getObjectId() + "#"
					+ record->getName() + "#" + record->getStickerId();
		} else {
			pendingRequestData += "," + record->getObjectId() + "#"
					+ record->getName() + "#" + record->getStickerId();
		}
	}
	pendingRequestData += "\"";

	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	HttpRequest* httpRequest = new HttpRequest();
	httpRequest->setUrl(url.c_str());
	httpRequest->setRequestType(HttpRequest::Type::PUT);
	httpRequest->setRequestData(pendingRequestData.c_str(),
			pendingRequestData.length());
	httpRequest->setResponseCallback(
			[this](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi denySendingSticker on Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					CCLog("bambi denySendingSticker on Firebase callback: %s", clearData.c_str());

					if (_firebaseTradeFeatureDelegate != nullptr) {
						_firebaseTradeFeatureDelegate->responseAfterDenyingRequest(true);
					}
				} else {
					if (_firebaseTradeFeatureDelegate != nullptr) {
						_firebaseTradeFeatureDelegate->responseAfterDenyingRequest(false);
					}
				}
			});
	HttpClient::getInstance()->send(httpRequest);
	httpRequest->release();
}

void FirebaseHandler::acceptReceivingSticker(
		vector<PendingRequest*> vtPendingRequest, PendingRequest* request) {
	//Remove the request from vtRequest
	vtPendingRequest.erase(
			std::remove(vtPendingRequest.begin(), vtPendingRequest.end(),
					request), vtPendingRequest.end());

	//Reupload data
	string pendingRequestData = "\"";
	for (PendingRequest* record : vtPendingRequest) {
		if (pendingRequestData == "") {
			pendingRequestData += record->getObjectId() + "#"
					+ record->getName() + "#" + record->getStickerId();
		} else {
			pendingRequestData += "," + record->getObjectId() + "#"
					+ record->getName() + "#" + record->getStickerId();
		}
	}
	pendingRequestData += "\"";

	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	HttpRequest* httpRequest = new HttpRequest();
	httpRequest->setUrl(url.c_str());
	httpRequest->setRequestType(HttpRequest::Type::PUT);
	httpRequest->setRequestData(pendingRequestData.c_str(),
			pendingRequestData.length());
	httpRequest->setResponseCallback(
			[this, request](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi acceptReceivingSticker on Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					CCLog("bambi acceptReceivingSticker on Firebase callback: %s", clearData.c_str());

					if (_firebaseTradeFeatureDelegate != nullptr) {
						_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(true);
					}

					StickerHelper::saveToMyStickerList(request->getStickerId());
				} else {
					if (_firebaseTradeFeatureDelegate != nullptr) {
						_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
					}
				}
			});
	HttpClient::getInstance()->send(httpRequest);
	httpRequest->release();
}

void FirebaseHandler::checkPendingRequest() {
	//Get pendingRequest from server
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi checkPendingRequest Firebase - url: %s", url.c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi checkPendingRequest on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			std::string::size_type i3 = clearData.find("\"\"");
			if (i3 == std::string::npos) {
				CCLog("bambi checkPendingRequest on Firebase callback: %s", clearData.c_str());
				//Process data
				if (clearData.length() > 5) { //Has data
					string pendingRequest = clearData.substr(1, clearData.length() - 1);

					std::string::size_type i2 = pendingRequest.find("\"");
					CCLog("bambi checkPendingRequest: i2 - %d", i2);

					if (i2 != std::string::npos) {
						CCLog("bambi checkPendingRequest: found i2 - %d", i2);

						pendingRequest = pendingRequest.substr(0, i2);
					}

					CCLog("bambi checkPendingRequest: %s", pendingRequest.c_str());

					vector<PendingRequest*> vtPendingRequest;
					vector<string> listStringPendingRequest = CppUtils::splitStringByDelim(pendingRequest,',');
					for(string pendingRequestString : listStringPendingRequest)
					{
						CCLog("bambi checkPendingRequest after splitting: %s", pendingRequestString.c_str());
						PendingRequest* request = new PendingRequest();
						vector<string> requestData = CppUtils::splitStringByDelim(pendingRequestString, '#');
						request->setObjectId(requestData.at(0));
						request->setName(requestData.at(1));
						request->setStickerId(requestData.at(2));
						vtPendingRequest.push_back(request);
					}

					if(_firebaseTradeFeatureDelegate != nullptr)
					{
						_firebaseTradeFeatureDelegate->responseAfterCheckingPendingRequest(vtPendingRequest);
					}
				}
			}
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::checkGivenStickers() {
	//Get pendingRequest from server
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi checkGivenStickers Firebase - url: %s", url.c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi checkGivenStickers on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi checkGivenStickers on Firebase callback: %s", clearData.c_str());
			std::string::size_type i3 = clearData.find("\"\"");
			if (i3 == std::string::npos) {
				//Process data
				if (clearData.length() > 5) { //Has data
					string pendingRequest = clearData.substr(1, clearData.length() - 1);

										std::string::size_type i2 = pendingRequest.find("\"");
										CCLog("bambi checkPendingRequest: i2 - %d", i2);

										if (i2 != std::string::npos) {
											CCLog("bambi checkPendingRequest: found i2 - %d", i2);

											pendingRequest = pendingRequest.substr(0, i2);
										}

					CCLog("bambi checkGivenStickers: %s", pendingRequest.c_str());

					vector<PendingRequest*> vtPendingRequest;
					vector<string> listStringPendingRequest = CppUtils::splitStringByDelim(pendingRequest,',');
					for(string pendingRequestString : listStringPendingRequest)
					{
						PendingRequest* request = new PendingRequest();
						vector<string> requestData = CppUtils::splitStringByDelim(pendingRequestString, '#');
						request->setObjectId(requestData.at(0));
						request->setName(requestData.at(1));
						request->setStickerId(requestData.at(2));
						vtPendingRequest.push_back(request);
					}

					if(_firebaseTradeFeatureDelegate != nullptr)
					{
						_firebaseTradeFeatureDelegate->responseAfterCheckingGivenSticker(vtPendingRequest);
					}
				}
			}
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
void FirebaseHandler::askTheStickerOfUer(int stickerId, BUserInfor* user) {
	//Get pendingRequest from server
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ user->getObjectId()
					+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this, stickerId, user](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi askTheStickerOfUer on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi askTheStickerOfUer on Firebase callback: %s", clearData.c_str());

			//Process data
			string pendingRequest = clearData.substr(1, clearData.length() - 1);

								std::string::size_type i2 = pendingRequest.find("\"");
								CCLog("bambi checkPendingRequest: i2 - %d", i2);

								if (i2 != std::string::npos) {
									CCLog("bambi checkPendingRequest: found i2 - %d", i2);

									pendingRequest = pendingRequest.substr(0, i2);
								}






			CCLog("bambi pendingRequest: %s", pendingRequest.c_str());


			//Then add a new pending request to the string and put it to server
			string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + user->getObjectId()
			+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

			CCLog("bambi askTheStickerOfUer Firebase 2 - stickerId: %d, url: %s",
					stickerId, url.c_str());

			string dataString;
			string loggedInUserObjectID = UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID);
			string loggedInUserName = UserDefault::getInstance()->getStringForKey(KEY_WORLD_NAME);
			if(pendingRequest == "")
			{
				dataString = "\""+loggedInUserObjectID+ "#"+loggedInUserName+"#"+CppUtils::doubleToString(stickerId) + "\"";
			} else
			{
				dataString = "\""+pendingRequest+"," +loggedInUserObjectID+ "#"+loggedInUserName+"#"+CppUtils::doubleToString(stickerId) + "\"";
			}
			//Request http
			HttpRequest* request = new HttpRequest();
			request->setUrl(url.c_str());
			request->setRequestType(HttpRequest::Type::PUT);
			request->setRequestData(dataString.c_str(), dataString.length());
			request->setResponseCallback([this, stickerId](HttpClient* client,
							HttpResponse* response) {
						CCLog("bambi askTheStickerOfUer on Firebase 2 - responding: %s",
								response->isSucceed() ? "success" : "failed");
						if (response->isSucceed()) {
							//Clear data (sometimes stranged characters be attached after the result)
							std::vector<char> *buffer = response->getResponseData();
							const char *data = reinterpret_cast<char *>(&(buffer->front()));
							std::string clearData(data);
							CCLog("bambi askTheStickerOfUer on Firebase 2 callback: %s", clearData.c_str());

							if (_firebaseTradeFeatureDelegate != nullptr) {
								_firebaseTradeFeatureDelegate->responseAfterAskingSticker(stickerId,
										true);
							}
						} else {
							if (_firebaseTradeFeatureDelegate != nullptr) {
								_firebaseTradeFeatureDelegate->responseAfterAskingSticker(stickerId,
										false);
							}
						}
					});
			HttpClient::getInstance()->send(request);
			request->release();
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::saveToMyStickedStickerList(string stickerIdString) {
	CCLog("bambi saveToMyStickedStickerList Firebase, getting my object id, sticker string: %s", stickerIdString.c_str());
	saveToMyStickedStickerList(
			UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID),
			stickerIdString);
}

void FirebaseHandler::saveToMyStickedStickerList(string objectID,
		string stickerIdString) {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + objectID
					+ "/Sticked_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi saveToMyStickedStickerList Firebase - stickerId: %s, url: %s",
			stickerIdString.c_str(), url.c_str());

	string data = "\"" + stickerIdString + "\"";
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::PUT);
	request->setRequestData(data.c_str(), data.length());
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi saveToMyStickedStickerList on Firebase - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			size_t pos = clearData.rfind("}");
			clearData = clearData.substr(0, pos + 1);
			CCLog("bambi saveToMyStickedStickerList on Firebase callback: %s", clearData.c_str());
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
void FirebaseHandler::saveToMyStickerList(string stickerIdString) {
	CCLog("bambi saveToMyStickerList Firebase, getting my object id");
	saveToMyStickerList(
			UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID),
			stickerIdString);
}

void FirebaseHandler::saveToMyStickerList(string objectID,
		string stickerIdString) {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + objectID
					+ "/All_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi saveToMyStickerList Firebase - stickerId: %s, url: %s",
			stickerIdString.c_str(), url.c_str());

	string data = "\"" + stickerIdString + "\"";
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::PUT);
	request->setRequestData(data.c_str(), data.length());
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi saveToMyStickerList on Firebase - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			size_t pos = clearData.rfind("}");
			clearData = clearData.substr(0, pos + 1);
			CCLog("bambi saveToMyStickerList on Firebase callback: %s", clearData.c_str());
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
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
							CCLog("bambi probability key: %s",userdefaultKey.c_str());
						}
					}
				}
			});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::getProbabilityCommonPacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Commonpacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::COMMON);
}

void FirebaseHandler::getProbabilityUncommonPacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Uncommonpacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::UNCOMMON);
}

void FirebaseHandler::getProbabilityRarePacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Rarepacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::RARE);
}
void FirebaseHandler::getProbabilityVeryRarePacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Veryrarepacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::VERYRARE);
}
void FirebaseHandler::getProbabilityRarestPacket() {
	string url = String::createWithFormat(firebaseURL.c_str(),
			firebaseQuerry_Probability_Rarestpacket.c_str())->getCString();
	vector<string> probabilityKeys = {KEY_PROBABILITY_FREEPACKET_COMMON, KEY_PROBABILITY_FREEPACKET_UNCOMMON,
		KEY_PROBABILITY_FREEPACKET_RARE, KEY_PROBABILITY_FREEPACKET_VERYRARE, KEY_PROBABILITY_FREEPACKET_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::RAREST);
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
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(firebaseURL.c_str(),
					CppUtils::encodeUrl(
							String::createWithFormat(
									firebaseQuerry_IsThisUserExistOnFirebase.c_str(),
									KEY_WORLD_ID,
									FacebookHandler::getInstance()->getUserFacebookID().c_str())->getCString()).c_str())->getCString());
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
			rapidjson::Document document;
			document.Parse<0>(clearData.c_str());
			for (rapidjson::Value::ConstMemberIterator itr =
					document.MemberBegin(); itr != document.MemberEnd();
					++itr) {
				string facebookId =
						document[itr->name.GetString()][KEY_WORLD_ID].GetString();
				CCLog(
						"bambi checkFacebookIdExistOnFirebaseCallBack, in the loop, facebookId: %s - %s, stickers: %s",
						facebookId.c_str(),
						document[itr->name.GetString()][KEY_WORLD_NAME].GetString(),
						document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
				BUserInfor* user = new BUserInfor();
				user->setName(
						document[itr->name.GetString()][KEY_WORLD_NAME].GetString());
				user->setId(facebookId);
				user->setAllStickers(
						document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
				user->setStickedStickers(
						document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
				user->setPendingRequest(
						document[itr->name.GetString()][KEY_WORLD_PENDING_REQUEST].GetString());
				user->setObjectId(itr->name.GetString());

				CCLog(
						"bambi checkFacebookIdExistOnFirebaseCallBack objectId: %s, name: %s",
						user->getObjectId().c_str(), user->getName().c_str());
				//Save to UserDefault (your will need objectId when posting score on Firebase)
				UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
						user->getObjectId());
				UserDefault::getInstance()->setStringForKey(KEY_WORLD_NAME,
						user->getName());
				break;
			}

			if (_firebaseDelegate != nullptr)
				_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Response to home scene
		} else
			FacebookHandler::getInstance()->getMyProfile();
		//After getMyProfile, responseWhenGetMyInfoSuccessfully function will be called.
	}
}
void FirebaseHandler::responseWhenGetMyInfoSuccessfully(BUserInfor* user) {
	CCLog(
			"bambi responseWhenGetMyInfoSuccessfully, going to saveFacebookIdOnFirebase");
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

		if (d.HasMember("name")) {
			UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
					d["name"].GetString());
		}

		if (_firebaseDelegate != nullptr)
			_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Respone to home scene

	}
}

void FirebaseHandler::fetchFriendsFromFirebase(string friendList) {
	this->friendList = friendList;
	CCLog("bambi fetchFriendsFromFirebase: %s", friendList.c_str());
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(firebaseURL.c_str(),
					firebaseQuerry_User.c_str())->getCString());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			CC_CALLBACK_2(FirebaseHandler::callBackFetchFriendsFromFirebase,
					this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::callBackFetchFriendsFromFirebase(HttpClient* client,
		HttpResponse* response) {
	std::string error = response->getErrorBuffer();
	if (response->isSucceed() && error == "") {
//Clear old data
		_friendList.clear();

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
		for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
				itr != document.MemberEnd(); ++itr) {
			string facebookId =
					document[itr->name.GetString()][KEY_WORLD_ID].GetString();
			CCLog(
					"bambi callBackFetchFriendsFromFirebase, in the loop, facebookId: %s - %s, stickers: %s",
					facebookId.c_str(),
					document[itr->name.GetString()][KEY_WORLD_NAME].GetString(),
					document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
			if (friendList.find(facebookId) != std::string::npos) {
				BUserInfor* user = new BUserInfor();
				user->setName(
						document[itr->name.GetString()][KEY_WORLD_NAME].GetString());
				user->setId(facebookId);
				user->setAllStickers(
						document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString());
				user->setStickedStickers(
						document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
				user->setPendingRequest(
						document[itr->name.GetString()][KEY_WORLD_PENDING_REQUEST].GetString());
				user->setObjectId(itr->name.GetString());
				_friendList.push_back(user);
			}
		}
		CCLog("bambi callBackFetchFriendsFromFirebase out the loop");
	}
	// Response to RankingScene
	if (_firebaseDelegate != nullptr)
		_firebaseDelegate->responseForQuerryTopFriend(_friendList);

	friendList = "";
}

void FirebaseHandler::fetchFriendsFromFacebook() {
	FacebookHandler::getInstance()->getAllFriendsID();
	//After get friends successfully responseWhenGetFriendsSuccessfully will be called.
}
void FirebaseHandler::responseWhenGetFriendsSuccessfully(string friendList) {
	friendList += "\"" + FacebookHandler::getInstance()->getUserFacebookID()
			+ "\""; //Attach my FacebookID to friendList

	fetchFriendsFromFirebase(friendList);
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

void FirebaseHandler::getStickersDataFromFirebase() {
	getStickersDataFromFirebase(sdkbox::PluginFacebook::getUserID());
}

void FirebaseHandler::getStickersDataFromFirebase(string facebookID) {
	CCLog("bambi Firebase -> getStickersDataFromFirebase");
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(firebaseURL.c_str(),
					firebaseQuerry_User.c_str())->getCString());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this, facebookID](HttpClient* client,
			HttpResponse* response) {
		std::string error = response->getErrorBuffer();
		if (response->isSucceed() && error == "") {
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
			for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
					itr != document.MemberEnd(); ++itr) {
				string facebookId = document[itr->name.GetString()][KEY_WORLD_ID].GetString();
				if (facebookID.find(facebookId)
						!= std::string::npos) {
					if(_firebaseDelegate)
					{
						_firebaseDelegate->responseAfterGetStickersDataFromFirebase(facebookId, document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString(),
								document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
					}
					break;
				}
			}
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
