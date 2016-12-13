#ifndef __StickerHelper__
#define __StickerHelper__

#include "cocos2d.h"
#include "../Constants/StickerResource.h"
#include "../Constants/Constants.h"
#include "../Helper/CppUtils.h"
#include "Trading/FirebaseHandler/FirebaseHandler.h"

USING_NS_CC;
using namespace std;
class StickerHelper {
public:
	StickerHelper() = delete; //Compiler will not compile constructor -> for optimizing point

	static bool isStickerHasAlreadyExisted(int stickerId);
	static bool isStickerHasNotSticked(int stickerId);
	static bool isStickerAbleToSell(int stickerId);
	static void saveToMyStickerList(int stickerId);
	static void saveToMyStickerList(string stickerIdString);
	static void saveToMyGluedStickerList(int stickerId);
	static void saveToMyGluedStickerList(string stickerIdString);
	static void sellSticker(Sticker* sticker);
	static int getCurrentExistStickerNumber(bool withUniqueElements);
	static vector<Sticker*> getCurrentExistSticker(bool withUniqueElements, bool getHasNotGluedStickersOnly);
	static vector<Sticker*> getCurrentExistSticker(bool withUniqueElements);
	static vector<Sticker*> getStickerOfPage(StickerPage* page);
	static vector<Sticker*> getStickerAvailableToGlueOfPage(StickerPage* page);

	static vector<STICKER_RARITY> getCurrentPacketsFromSharePreferences();
	static void appendAPacketToSharePreferences(STICKER_RARITY packet);
	static void removeAPacketFromSharePerferences(STICKER_RARITY packet);

	static int getStickerQuantityInMyList(int stickerId);
	static Sticker* getStickerFromId(int stickerId);
	static StickerPage* getStickerPageFromId(int stickerPageId);
	static string getRarityString(STICKER_RARITY rarity);
};

#endif
