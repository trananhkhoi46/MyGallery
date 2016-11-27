#ifndef __StickerHelper__
#define __StickerHelper__

#include "cocos2d.h"
#include "StickerResource.h"
#include "Constants.h"
#include "CppUtils.h"

USING_NS_CC;
using namespace std;
class StickerHelper {
public:
	StickerHelper() = delete; //Ghi vay compiler ko khoi tao constructor

	static bool isStickerHasAlreadyExisted(int stickerId);
	static bool isStickerHasNotSticked(int stickerId);
	static void saveToMyStickerList(int stickerId);
	static void saveToMyStickerList(string stickerIdString);
	static void saveToMyGluedStickerList(int stickerId);
	static void saveToMyGluedStickerList(string stickerIdString);
	static int getCurrentExistStickerNumber(bool withUniqueElements);
	static vector<Sticker*> getCurrentExistSticker(bool withUniqueElements, bool getHasNotGluedStickersOnly);
	static vector<Sticker*> getCurrentExistSticker(bool withUniqueElements);
	static vector<Sticker*> getStickerOfPage(StickerPage* page);
	static vector<Sticker*> getStickerAvailableToGlueOfPage(StickerPage* page);
	static int getStickerQuantityInMyList(int stickerId);
	static Sticker* getStickerFromId(int stickerId);
	static StickerPage* getStickerPageFromId(int stickerPageId);
	static string getRarityString(STICKER_RARITY rarity);
};

#endif
