#include "StickerHelper.h"

vector<STICKER_RARITY> StickerHelper::getCurrentPacketsFromSharePreferences() {
	vector<STICKER_RARITY> result;
	string currentPacketString = UserDefault::getInstance()->getStringForKey(
	CURRENT_PACKET, "0,1,2");
	vector < string > vtPackets = CppUtils::splitStringByDelim(
			currentPacketString, ',');
	for (string packetString : vtPackets) {
		if (packetString != "") {
			STICKER_RARITY packetRarity =
					static_cast<STICKER_RARITY>(CppUtils::stringToDouble(
							packetString));
			result.push_back(packetRarity);
		}
	}
	CCLog("bambi StickerHelper there are %d packets available", result.size());
	return result;
}
void StickerHelper::appendAPacketToSharePreferences(STICKER_RARITY packet) {
	string packetString = CppUtils::doubleToString(static_cast<int>(packet));
	string currentPacketString = UserDefault::getInstance()->getStringForKey(
	CURRENT_PACKET, "0,1,2");

	currentPacketString = "," + packetString;

	std::string::size_type i2 = currentPacketString.find(",,"); //Replace ,, to ,
	if (i2 != std::string::npos) {
		currentPacketString.erase(i2, 1);
	}
	UserDefault::getInstance()->setStringForKey(
	CURRENT_PACKET, currentPacketString);
}
void StickerHelper::removeAPacketFromSharePerferences(STICKER_RARITY packet) {
	string packetString = CppUtils::doubleToString(static_cast<int>(packet));
	string currentPacketString = UserDefault::getInstance()->getStringForKey(
	CURRENT_PACKET, "0,1,2");

	std::string::size_type i = currentPacketString.find(packetString);
	if (i != std::string::npos) {
		currentPacketString.erase(i, packetString.length());
	}

	std::string::size_type i2 = currentPacketString.find(",,"); //Replace ,, to ,
	if (i2 != std::string::npos) {
		currentPacketString.erase(i2, 1);
	}
	UserDefault::getInstance()->setStringForKey(
	CURRENT_PACKET, currentPacketString);
}

bool StickerHelper::isStickerHasAlreadyExisted(int stickerId) {
	string stickerIdString = CppUtils::doubleToString(stickerId);
	vector < string > listString = CppUtils::splitStringByDelim(
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER), ',');
	for (int i = 0; i < listString.size(); i++) {
		if (listString.at(i) == stickerIdString) {
			return true;
		}
	}
	return false;
}

bool StickerHelper::isStickerHasNotSticked(int stickerId) {
	string stickerIdString = CppUtils::doubleToString(stickerId);
	vector < string > listString = CppUtils::splitStringByDelim(
			UserDefault::getInstance()->getStringForKey(STICKED_STICKER), ',');
	for (int i = 0; i < listString.size(); i++) {
		if (listString.at(i) == stickerIdString) {
			return false;
		}
	}
	return true;
}

Sticker* StickerHelper::getStickerFromId(int stickerId) {
	for (int i = 0; i < vt_stickers.size(); i++) {
		if (stickerId == vt_stickers.at(i)->sticker_id) {
			return vt_stickers.at(i);
		}
	}
	return nullptr;
}

StickerPage* StickerHelper::getStickerPageFromId(int stickerPageId) {
	for (int i = 0; i < vt_sticker_pages.size(); i++) {
		if (stickerPageId == vt_sticker_pages.at(i)->sticker_page_id) {
			return vt_sticker_pages.at(i);
		}
	}
	return nullptr;
}

string StickerHelper::getRarityString(STICKER_RARITY rarity) {
	switch (rarity) {
	case COMMON:
		return "Common";
	case UNCOMMON:
		return "Uncommon";
	case RARE:
		return "Rare";
	case VERYRARE:
		return "Very rare";
	case RAREST:
		return "Rarest";
	}
	return "";
}

void StickerHelper::saveToMyStickerList(string stickerIdString) {
	UserDefault::getInstance()->setStringForKey(CURRENT_STICKER,
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, "")
					+ "," + stickerIdString);
	CCLog("bambi saveToMyStickerList - after saving successfully: %s",
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER).c_str());

	//Save to server
	if (FacebookHandler::getInstance()->isFacebookLoggedIn()) {
		FirebaseHandler::getInstance()->saveToMyStickerList(
				UserDefault::getInstance()->getStringForKey(CURRENT_STICKER));
	}
}
void StickerHelper::saveToMyStickerList(int stickerId) {
	saveToMyStickerList(CppUtils::doubleToString(stickerId));
}

void StickerHelper::saveToMyGluedStickerList(string stickerIdString) {
	UserDefault::getInstance()->setStringForKey(STICKED_STICKER,
			UserDefault::getInstance()->getStringForKey(STICKED_STICKER, "")
					+ "," + stickerIdString);
	CCLog("bambi saveToMyStickerList - after saving successfully: %s",
			UserDefault::getInstance()->getStringForKey(STICKED_STICKER).c_str());

	//Save to server
	if (FacebookHandler::getInstance()->isFacebookLoggedIn()) {
		FirebaseHandler::getInstance()->saveToMyStickedStickerList(
				UserDefault::getInstance()->getStringForKey(STICKED_STICKER));
	}
}
void StickerHelper::saveToMyGluedStickerList(int stickerId) {
	saveToMyGluedStickerList(CppUtils::doubleToString(stickerId));
}

int StickerHelper::getStickerQuantityInMyList(int stickerId) {
	int quantity = 0;
	vector < string > vtCurrentSticker = CppUtils::splitStringByDelim(
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER), ',');

	for (int i = 0; i < vtCurrentSticker.size(); i++) {
		if ((int) CppUtils::stringToDouble(vtCurrentSticker.at(i))
				== stickerId) {
			quantity++;
		}
	}

	return quantity;
}

int StickerHelper::getCurrentExistStickerNumber(bool withUniqueElements) {
	vector < string > vtCurrentSticker = CppUtils::splitStringByDelim(
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER), ',');
	if (withUniqueElements) {
		set < string
				> setCurrentSticker(vtCurrentSticker.begin(),
						vtCurrentSticker.end());
		return setCurrentSticker.size();
	} else {
		return vtCurrentSticker.size();
	}
}

vector<Sticker*> StickerHelper::getStickerOfPage(StickerPage* page) {
	vector<Sticker*> result;
	for (Sticker* sticker : vt_stickers) {
		if (sticker->sticker_page_id == page->sticker_page_id) {
			result.push_back(sticker);
		}
	}
	return result;
}

vector<Sticker*> StickerHelper::getStickerAvailableToGlueOfPage(
		StickerPage* page) {
	vector<Sticker*> result;
	vector<Sticker*> vt_exist_sticker = getCurrentExistSticker(true);
	for (Sticker* sticker : vt_exist_sticker) {
		if (sticker->sticker_page_id == page->sticker_page_id
				&& isStickerHasNotSticked(sticker->sticker_id)) {
			result.push_back(sticker);
		}
	}
	return result;
}

vector<Sticker*> StickerHelper::getCurrentExistSticker(
		bool withUniqueElements) {
	return getCurrentExistSticker(withUniqueElements, false);
}
vector<Sticker*> StickerHelper::getCurrentExistSticker(bool withUniqueElements,
		bool getHasNotGluedStickersOnly) {
	vector<Sticker*> result;
	vector < string > vtCurrentSticker = CppUtils::splitStringByDelim(
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER), ',');
	if (withUniqueElements) {
		set < string
				> setCurrentSticker(vtCurrentSticker.begin(),
						vtCurrentSticker.end());

		for (string element : setCurrentSticker) {
			for (int i = 0; i < vt_stickers.size(); i++) {
				if (vt_stickers.at(i)->sticker_id
						== (int) CppUtils::stringToDouble(element)) {
					if (!getHasNotGluedStickersOnly
							|| (getHasNotGluedStickersOnly
									&& isStickerHasNotSticked(
											vt_stickers.at(i)->sticker_id))) {
						result.push_back(vt_stickers.at(i));
					}
					break;
				}
			}
		}
		return result;
	} else {
		for (string element : vtCurrentSticker) {
			for (int i = 0; i < vt_stickers.size(); i++) {
				if (vt_stickers.at(i)->sticker_id
						== (int) CppUtils::stringToDouble(element)) {
					if (!getHasNotGluedStickersOnly
							|| (getHasNotGluedStickersOnly
									&& isStickerHasNotSticked(
											vt_stickers.at(i)->sticker_id))) {
						result.push_back(vt_stickers.at(i));
					}
					break;
				}
			}
		}
		return result;
	}
}
