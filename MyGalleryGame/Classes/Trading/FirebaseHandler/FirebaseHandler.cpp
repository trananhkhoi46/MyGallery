#include "FirebaseHandler.h"

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
	CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 1");

	//Clone request
	PendingRequest* newRequest = new PendingRequest();
	newRequest->setName(request->getName());
	newRequest->setObjectId(request->getObjectId());
	newRequest->setStickerId(request->getStickerId());

	//Remove this element from PendingRequest of logged in user
	denySendingSticker(vtPendingRequest, request);

	CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 2");


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
		CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase callback: %s", clearData.c_str());

			//Process data
			string pendingRequest = clearData.substr(1, clearData.length() - 1);

			std::string::size_type i2 = pendingRequest.find("\"");
			CCLog("bambi FirebaseHandler -> checkPendingRequest: i2 - %d", i2);

			if (i2 != std::string::npos) {
				CCLog("bambi FirebaseHandler -> checkPendingRequest: found i2 - %d", i2);

				pendingRequest = pendingRequest.substr(0, i2);
			}

			CCLog("bambi FirebaseHandler -> acceptSendingSticker - givenStickers: %s", pendingRequest.c_str());

			//Then add a new pending request to the string and put it to server
			string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + request->getObjectId()
			+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

			CCLog("bambi FirebaseHandler -> acceptSendingSticker Firebase 2 - stickerId: %s, url: %s",
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
						CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 2 - responding: %s",
								response->isSucceed() ? "success" : "failed");
						if (response->isSucceed()) {
							//Clear data (sometimes stranged characters be attached after the result)
							std::vector<char> *buffer = response->getResponseData();
							const char *data = reinterpret_cast<char *>(&(buffer->front()));
							std::string clearData(data);
							CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 2 callback: %s", clearData.c_str());

							//Remove the sticker from list
							string newCurrentStickerStringAfterRemoving =
							UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, "");
							std::string::size_type i = newCurrentStickerStringAfterRemoving.find(request->getStickerId());
							if (i != std::string::npos) {
								newCurrentStickerStringAfterRemoving.erase(i, request->getStickerId().length());
							}

							newCurrentStickerStringAfterRemoving = CppUtils::replaceString(newCurrentStickerStringAfterRemoving, ",,", ",");

							CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 2 callback: string sticker after removing: %s",
									UserDefault::getInstance()->getStringForKey(CURRENT_STICKER).c_str());

							UserDefault::getInstance()->setStringForKey(CURRENT_STICKER, newCurrentStickerStringAfterRemoving);

							string url =
							"https://gallerygame-fab40.firebaseio.com/json/users/" + UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID)
							+ "/All_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

							CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 2 call back - stickerId: %s, url: %s",
									request->getStickerId().c_str(), url.c_str());

							string dataString = "\"" + newCurrentStickerStringAfterRemoving + "\"";
							//Request http
							HttpRequest* httpRequest = new HttpRequest();
							httpRequest->setUrl(url.c_str());
							httpRequest->setRequestType(HttpRequest::Type::PUT);
							httpRequest->setRequestData(dataString.c_str(), dataString.length());
							httpRequest->setResponseCallback([this](HttpClient* client,
											HttpResponse* response) {
										CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 3 - responding: %s",
												response->isSucceed() ? "success" : "failed");
										if (response->isSucceed()) {
											//Clear data (sometimes stranged characters be attached after the result)
											std::vector<char> *buffer = response->getResponseData();
											const char *data = reinterpret_cast<char *>(&(buffer->front()));
											std::string clearData(data);
											CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 3 - 1 callback: %s", clearData.c_str());

											size_t pos = clearData.find("}");
											clearData = clearData.substr(0, pos + 1);
											CCLog("bambi FirebaseHandler -> acceptSendingSticker on Firebase 3 - 2 callback: %s", clearData.c_str());

											if (_firebaseTradeFeatureDelegate != nullptr) {
                                                try{
												_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(true);
                                                }catch(exception e){}
                                                
											}

										} else
										{
											if (_firebaseTradeFeatureDelegate != nullptr) {
                                                try{
												_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
                                                }catch(exception e){}
                                                
											}
										}
									});
							HttpClient::getInstance()->send(httpRequest);
							httpRequest->release();
						} else {
							if (_firebaseTradeFeatureDelegate != nullptr) {
                                try{
								_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
                                }catch(exception e){}
                                
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
	CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase");

	//Remove the request from vtRequest
	vtPendingRequest.erase(
			std::remove(vtPendingRequest.begin(), vtPendingRequest.end(),
					request), vtPendingRequest.end());

	CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase - remove from vector");

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

	CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase - repair data: %s", pendingRequestData.c_str());

	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase - url: %s", url.c_str());

	HttpRequest* httpRequest = new HttpRequest();
	httpRequest->setUrl(url.c_str());
	httpRequest->setRequestType(HttpRequest::Type::PUT);
	httpRequest->setRequestData(pendingRequestData.c_str(),
			pendingRequestData.length());
	httpRequest->setResponseCallback(
			[this](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					CCLog("bambi FirebaseHandler -> denySendingSticker on Firebase callback: %s", clearData.c_str());

					if (_firebaseTradeFeatureDelegate != nullptr) {
                        try{
						_firebaseTradeFeatureDelegate->responseAfterDenyingRequest(true);
                        }catch(exception e){}
                        
					}
				} else {
					if (_firebaseTradeFeatureDelegate != nullptr) {
                        try{
						_firebaseTradeFeatureDelegate->responseAfterDenyingRequest(false);
                        }catch(exception e){}
                        
					}
				}
			});
	HttpClient::getInstance()->send(httpRequest);
	httpRequest->release();
}

void FirebaseHandler::acceptReceivingSticker(
		vector<PendingRequest*> vtPendingRequest, PendingRequest* request) {
	CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase");

	//Remove the request from vtRequest
	vtPendingRequest.erase(
			std::remove(vtPendingRequest.begin(), vtPendingRequest.end(),
					request), vtPendingRequest.end());

	CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase - after removing from vector");

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

	CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase - repair data: %s",pendingRequestData.c_str());

	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/"
					+ UserDefault::getInstance()->getStringForKey(
					KEY_WORLD_OJECTID)
					+ "/Given_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";


	CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase - repair data: %s - url: %s",pendingRequestData.c_str(), url.c_str());
	HttpRequest* httpRequest = new HttpRequest();
	httpRequest->setUrl(url.c_str());
	httpRequest->setRequestType(HttpRequest::Type::PUT);
	httpRequest->setRequestData(pendingRequestData.c_str(),
			pendingRequestData.length());
	httpRequest->setResponseCallback(
			[this, request](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					CCLog("bambi FirebaseHandler -> acceptReceivingSticker on Firebase callback: %s", clearData.c_str());

					if (_firebaseTradeFeatureDelegate != nullptr) {
                        try{
						_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(true);
                        }catch(exception e){}
                        
					}

					StickerHelper::saveToMyStickerList(request->getStickerId());
				} else {
					if (_firebaseTradeFeatureDelegate != nullptr) {
                        try{
						_firebaseTradeFeatureDelegate->responseAfterAcceptingRequest(false);
                        }catch(exception e){}
                        
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

	CCLog("bambi FirebaseHandler -> checkPendingRequest Firebase - url: %s", url.c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi FirebaseHandler -> checkPendingRequest on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> checkPendingRequest - data: %s",clearData.c_str());

			std::string::size_type i3 = clearData.find("\"\"");
			if (i3 == std::string::npos) {
				CCLog("bambi FirebaseHandler -> checkPendingRequest on Firebase callback: %s", clearData.c_str());
				//Process data
				if (clearData.length() > 5) { //Has data
					string pendingRequest = clearData.substr(1, clearData.length() - 1);

					std::string::size_type i2 = pendingRequest.find("\"");
					CCLog("bambi FirebaseHandler -> checkPendingRequest: i2 - %d", i2);

					if (i2 != std::string::npos) {
						CCLog("bambi FirebaseHandler -> checkPendingRequest: found i2 - %d", i2);

						pendingRequest = pendingRequest.substr(0, i2);
					}

					CCLog("bambi FirebaseHandler -> checkPendingRequest: %s", pendingRequest.c_str());

					vector<PendingRequest*> vtPendingRequest;
					vector<string> listStringPendingRequest = CppUtils::splitStringByDelim(pendingRequest,',');
					for(string pendingRequestString : listStringPendingRequest)
					{
						CCLog("bambi FirebaseHandler -> checkPendingRequest after splitting: %s", pendingRequestString.c_str());
						PendingRequest* request = new PendingRequest();
						vector<string> requestData = CppUtils::splitStringByDelim(pendingRequestString, '#');
						request->setObjectId(requestData.at(0));
						request->setName(requestData.at(1));
						request->setStickerId(requestData.at(2));
						vtPendingRequest.push_back(request);
					}

					if(_firebaseTradeFeatureDelegate != nullptr)
					{
                        try{
						_firebaseTradeFeatureDelegate->responseAfterCheckingPendingRequest(vtPendingRequest);
                        }catch(exception e){}
                        
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

	CCLog("bambi FirebaseHandler -> checkGivenStickers Firebase - url: %s", url.c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi FirebaseHandler -> checkGivenStickers on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> checkGivenStickers - data: %s",clearData.c_str());

			CCLog("bambi FirebaseHandler -> checkGivenStickers on Firebase callback: %s", clearData.c_str());
			std::string::size_type i3 = clearData.find("\"\"");
			if (i3 == std::string::npos) {
				//Process data
				if (clearData.length() > 5) { //Has data
					string pendingRequest = clearData.substr(1, clearData.length() - 1);

					std::string::size_type i2 = pendingRequest.find("\"");
					CCLog("bambi FirebaseHandler -> checkPendingRequest: i2 - %d", i2);

					if (i2 != std::string::npos) {
						CCLog("bambi FirebaseHandler -> checkPendingRequest: found i2 - %d", i2);

						pendingRequest = pendingRequest.substr(0, i2);
					}

					CCLog("bambi FirebaseHandler -> checkGivenStickers: %s", pendingRequest.c_str());

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
                        try{
						_firebaseTradeFeatureDelegate->responseAfterCheckingGivenSticker(vtPendingRequest);
                        }catch(exception e){}
                        
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

	CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase - repair data: %s",url.c_str());

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this, stickerId, user](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase 1 - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase callback: %s", clearData.c_str());

			//Process data
			string pendingRequest = clearData.substr(1, clearData.length() - 1);

			std::string::size_type i2 = pendingRequest.find("\"");
			CCLog("bambi FirebaseHandler -> checkPendingRequest: i2 - %d", i2);

			if (i2 != std::string::npos) {
				CCLog("bambi FirebaseHandler -> checkPendingRequest: found i2 - %d", i2);

				pendingRequest = pendingRequest.substr(0, i2);
			}

			CCLog("bambi FirebaseHandler -> pendingRequest: %s", pendingRequest.c_str());

			//Then add a new pending request to the string and put it to server
			string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + user->getObjectId()
			+ "/Pending_Request.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

			CCLog("bambi FirebaseHandler -> askTheStickerOfUer Firebase 2 - stickerId: %d, url: %s",
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

			CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase 2 - repair data: %s",dataString.c_str());

			//Request http
			HttpRequest* request = new HttpRequest();
			request->setUrl(url.c_str());
			request->setRequestType(HttpRequest::Type::PUT);
			request->setRequestData(dataString.c_str(), dataString.length());
			request->setResponseCallback([this, stickerId](HttpClient* client,
							HttpResponse* response) {
						CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase 2 - responding: %s",
								response->isSucceed() ? "success" : "failed");
						if (response->isSucceed()) {
							//Clear data (sometimes stranged characters be attached after the result)
							std::vector<char> *buffer = response->getResponseData();
							const char *data = reinterpret_cast<char *>(&(buffer->front()));
							std::string clearData(data);
							CCLog("bambi FirebaseHandler -> askTheStickerOfUer on Firebase 2 callback: %s", clearData.c_str());

							if (_firebaseTradeFeatureDelegate != nullptr) {
                                try{
								_firebaseTradeFeatureDelegate->responseAfterAskingSticker(stickerId,
										true);
                                }catch(exception e){}
                                
							}
						} else {
							if (_firebaseTradeFeatureDelegate != nullptr) {
                                try{
								_firebaseTradeFeatureDelegate->responseAfterAskingSticker(stickerId,
										false);
                                }catch(exception e){}
                                
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
	CCLog(
			"bambi FirebaseHandler -> saveToMyStickedStickerList Firebase, getting my object id, sticker string: %s",
			stickerIdString.c_str());
	saveToMyStickedStickerList(
			UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID),
			stickerIdString);
}

void FirebaseHandler::saveToMyStickedStickerList(string objectID,
		string stickerIdString) {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + objectID
					+ "/Sticked_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi FirebaseHandler -> saveToMyStickedStickerList Firebase - stickerId: %s, url: %s",
			stickerIdString.c_str(), url.c_str());

	string data = "\"" + stickerIdString + "\"";
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::PUT);
	request->setRequestData(data.c_str(), data.length());
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi FirebaseHandler -> saveToMyStickedStickerList on Firebase - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> saveToMyStickedStickerList on Firebase 1 callback: %s", clearData.c_str());

			size_t pos = clearData.find("}");
			clearData = clearData.substr(0, pos + 1);
			CCLog("bambi FirebaseHandler -> saveToMyStickedStickerList on Firebase 2 callback: %s", clearData.c_str());
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
void FirebaseHandler::saveToMyStickerList(string stickerIdString) {
	CCLog("bambi FirebaseHandler -> saveToMyStickerList Firebase, getting my object id");
	saveToMyStickerList(
			UserDefault::getInstance()->getStringForKey(KEY_WORLD_OJECTID),
			stickerIdString);
}

void FirebaseHandler::saveToMyStickerList(string objectID,
		string stickerIdString) {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/users/" + objectID
					+ "/All_Stickers.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";

	CCLog("bambi FirebaseHandler -> saveToMyStickerList Firebase - stickerId: %s, url: %s",
			stickerIdString.c_str(), url.c_str());

	string data = "\"" + stickerIdString + "\"";
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::PUT);
	request->setRequestData(data.c_str(), data.length());
	request->setResponseCallback([this](HttpClient* client,
			HttpResponse* response) {
		CCLog("bambi FirebaseHandler -> saveToMyStickerList on Firebase - responding: %s",
				response->isSucceed() ? "success" : "failed");
		if (response->isSucceed()) {
			//Clear data (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> saveToMyStickerList on Firebase 1 callback: %s", clearData.c_str());

			size_t pos = clearData.find("}");
			clearData = clearData.substr(0, pos + 1);
			CCLog("bambi FirebaseHandler -> saveToMyStickerList on Firebase 2 callback: %s", clearData.c_str());
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::getProbability(string url, vector<string> probabilityKeys,
		STICKER_RARITY rarity) {
	CCLog("bambi FirebaseHandler -> get Probability from Firebase - calling");

	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			[this, probabilityKeys, rarity](HttpClient* client,
					HttpResponse* response) {
				CCLog("bambi FirebaseHandler -> get Probability from Firebase - responding: %s",
						response->isSucceed() ? "success" : "failed");
				if (response->isSucceed()) {
					//Clear data (sometimes stranged characters be attached after the result)
					std::vector<char> *buffer = response->getResponseData();
					const char *data = reinterpret_cast<char *>(&(buffer->front()));
					std::string clearData(data);
					CCLog("bambi FirebaseHandler -> get Probability from Firebase 1: %s", clearData.c_str());
					size_t pos = clearData.find("}");
					clearData = clearData.substr(0, pos + 1);
					CCLog("bambi FirebaseHandler -> get Probability from Firebase 2: %s", clearData.c_str());
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
							CCLog("bambi FirebaseHandler -> probability key: %s",userdefaultKey.c_str());
						}
					}
				}
			});
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::getProbabilityCommonPacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_commonpacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::COMMON);
}

void FirebaseHandler::getProbabilityUncommonPacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_uncommonpacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::UNCOMMON);
}

void FirebaseHandler::getProbabilityRarePacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_rarepacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys, STICKER_RARITY::RARE);
}
void FirebaseHandler::getProbabilityVeryRarePacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_veryrarepacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::VERYRARE);
}
void FirebaseHandler::getProbabilityRarestPacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_rarestpacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::RAREST);
}
void FirebaseHandler::getProbabilityFreePacket() {
	string url =
			"https://gallerygame-fab40.firebaseio.com/json/probability_freepacket.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
	vector<string> probabilityKeys = {KEY_PROBABILITY_COMMON, KEY_PROBABILITY_UNCOMMON,
		KEY_PROBABILITY_RARE, KEY_PROBABILITY_VERYRARE, KEY_PROBABILITY_RAREST};
	FirebaseHandler::getProbability(url, probabilityKeys,
			STICKER_RARITY::UNKNOWN);
}

void FirebaseHandler::checkFacebookIdExistOnFirebase() {
	CCLog("bambi FirebaseHandler -> checkFacebookIdExistOnFirebase ");
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			String::createWithFormat(
					"https://gallerygame-fab40.firebaseio.com/json/%sauth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir",
					CppUtils::encodeUrl(
							String::createWithFormat(
									"users.json?orderBy=\"FB_ID\"&equalTo=\"%s\"&",
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
	CCLog("bambi FirebaseHandler -> checkFacebookIdExistOnFirebaseCallBack");
	if (response->isSucceed()) {
		//Clear data (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);

		CCLog("bambi FirebaseHandler -> checkFacebookIdExistOnFirebaseCallBack 1: %s", clearData.c_str());
		size_t pos = clearData.find("}}");
		clearData = clearData.substr(0, pos + 2);

		CCLog("bambi FirebaseHandler -> checkFacebookIdExistOnFirebaseCallBack 2: %s",
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
						"bambi FirebaseHandler -> checkFacebookIdExistOnFirebaseCallBack, in the loop, facebookId: %s - %s, stickers: %s",
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
						"bambi FirebaseHandler -> checkFacebookIdExistOnFirebaseCallBack objectId: %s, name: %s",
						user->getObjectId().c_str(), user->getName().c_str());
				//Save to UserDefault (your will need objectId when posting data to Firebase)
				UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
						user->getObjectId());
				UserDefault::getInstance()->setStringForKey(KEY_WORLD_NAME,
						user->getName());
				break;
			}

			if (_firebaseDelegate != nullptr)
            {
                try{
				_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Response to home scene
                }catch(exception e){}
            }
        } else
			FacebookHandler::getInstance()->getMyProfile();
		//After getMyProfile, responseWhenGetMyInfoSuccessfully function will be called.
	}
}
void FirebaseHandler::responseWhenGetMyInfoSuccessfully(BUserInfor* user) {
	CCLog(
			"bambi FirebaseHandler -> responseWhenGetMyInfoSuccessfully, going to saveFacebookIdOnFirebase");
	saveFacebookIdOnFirebase(user);
}
void FirebaseHandler::saveFacebookIdOnFirebase(BUserInfor* user) {
	CCLog("bambi FirebaseHandler -> saveFacebookIdOnFirebase");
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			"https://gallerygame-fab40.firebaseio.com/json/users.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir");
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
	CCLog("bambi FirebaseHandler -> callBacksaveFacebookIdOnFirebase");
	if (response->isSucceed()) {
//Clear data (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		CCLog("bambi FirebaseHandler -> callBacksaveFacebookIdOnFirebase 1: %s", clearData.c_str());

		size_t pos = clearData.find("}");
		clearData = clearData.substr(0, pos + 1);

		CCLog("bambi FirebaseHandler -> callBacksaveFacebookIdOnFirebase 2: %s", clearData.c_str());
		if (clearData == "")
			return;

//Save to UserDefault (your will need objectId when posting data on Firebase)
		rapidjson::Document d;
		d.Parse<0>(clearData.c_str());

		if (d.HasMember("name")) {
			UserDefault::getInstance()->setStringForKey(KEY_WORLD_OJECTID,
					d["name"].GetString());
		}

        if (_firebaseDelegate != nullptr){
            try{
			_firebaseDelegate->responseAfterCheckFacebookIdExistOnFirebase(); //Respone to home scene
            }catch(exception e){}
            
        }
	}
}

void FirebaseHandler::fetchFriendsFromFirebase(string friendList) {
	this->friendList = friendList;
	CCLog("bambi FirebaseHandler -> fetchFriendsFromFirebase: %s", friendList.c_str());
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			"https://gallerygame-fab40.firebaseio.com/json/users.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(
			CC_CALLBACK_2(FirebaseHandler::callBackFetchFriendsFromFirebase,
					this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void FirebaseHandler::callBackFetchFriendsFromFirebase(HttpClient* client,
		HttpResponse* response) {
	CCLog("bambi FirebaseHandler -> callBackFetchFriendsFromFirebase");
	std::string error = response->getErrorBuffer();
	if (response->isSucceed() && error == "") {
//Clear old data
		_friendList.clear();

//Clear data that being got from Firebase (sometimes stranged characters be attached after the result)
		std::vector<char> *buffer = response->getResponseData();
		const char *data = reinterpret_cast<char *>(&(buffer->front()));
		std::string clearData(data);
		CCLog("bambi FirebaseHandler -> callBackFetchFriendsFromFirebase 1 - data: %s",clearData.c_str());

		size_t pos = clearData.find("}}");
		clearData = clearData.substr(0, pos + 2);
		CCLog("bambi FirebaseHandler -> callBackFetchFriendsFromFirebase 2 - data: %s",clearData.c_str());
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
					"bambi FirebaseHandler -> callBackFetchFriendsFromFirebase, in the loop, facebookId: %s - %s, stickers: %s",
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
		CCLog("bambi FirebaseHandler -> callBackFetchFriendsFromFirebase out the loop");
	}
	// Response to HomeScene
    if (_firebaseDelegate != nullptr){
        try{
		_firebaseDelegate->responseForQuerryTopFriend(_friendList);
        }catch(exception e){}
    }
	friendList = "";
}

void FirebaseHandler::fetchFriendsFromFacebook() {
	FacebookHandler::getInstance()->getAllFriendsID();
	//After get friends successfully responseWhenGetFriendsSuccessfully will be called.
}
void FirebaseHandler::responseWhenGetFriendsSuccessfully(string friendList) {
	CCLog("bambi FirebaseHandler->responseWhenGetFriendsSuccessfully");
	friendList += "\"" + FacebookHandler::getInstance()->getUserFacebookID()
			+ "\""; //Attach my FacebookID to friendList

	fetchFriendsFromFirebase(friendList);
}

void FirebaseHandler::getStickersDataFromFirebase() {
	getStickersDataFromFirebase(sdkbox::PluginFacebook::getUserID());
}

void FirebaseHandler::getStickersDataFromFirebase(string facebookID) {
	CCLog("bambi Firebase -> getStickersDataFromFirebase");
	//Request http
	HttpRequest* request = new HttpRequest();
	request->setUrl(
			"https://gallerygame-fab40.firebaseio.com/json/users.json?auth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([this, facebookID](HttpClient* client,
			HttpResponse* response) {
		std::string error = response->getErrorBuffer();
		if (response->isSucceed() && error == "") {
			//Clear data that being got from Firebase (sometimes stranged characters be attached after the result)
			std::vector<char> *buffer = response->getResponseData();
			const char *data = reinterpret_cast<char *>(&(buffer->front()));
			std::string clearData(data);
			CCLog("bambi FirebaseHandler -> getStickersDataFromFirebase 1 - data: %s",clearData.c_str());
			size_t pos = clearData.find("}}");
			clearData = clearData.substr(0, pos + 2);
			CCLog("bambi FirebaseHandler -> getStickersDataFromFirebase 2 - data: %s",clearData.c_str());
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
                        try{
						_firebaseDelegate->responseAfterGetStickersDataFromFirebase(facebookId, document[itr->name.GetString()][KEY_WORLD_ALL_STICKERS].GetString(),
								document[itr->name.GetString()][KEY_WORLD_STICKED_STICKERS].GetString());
                        }catch(exception e){}
					}
					break;
				}
			}
		}
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
