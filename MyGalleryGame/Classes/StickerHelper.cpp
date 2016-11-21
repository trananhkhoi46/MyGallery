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
int StickerHelper::getCurrentExistSticker(bool withUniqueElements) {
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
