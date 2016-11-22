#include "StickerHelper.h"

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

void StickerHelper::saveToMyStickerList(string stickerIdString) {
	UserDefault::getInstance()->setStringForKey(CURRENT_STICKER,
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER, "")
					+ "," + stickerIdString);
	CCLog("bambi saveToMyStickerList - after saving successfully: %s",
			UserDefault::getInstance()->getStringForKey(CURRENT_STICKER).c_str());
}
void StickerHelper::saveToMyStickerList(int stickerId) {
	saveToMyStickerList(CppUtils::doubleToString(stickerId));
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

vector<Sticker*> StickerHelper::getCurrentExistSticker(
		bool withUniqueElements) {
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
					result.push_back(vt_stickers.at(i));
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
					result.push_back(vt_stickers.at(i));
					break;
				}
			}
		}
		return result;
	}
}
