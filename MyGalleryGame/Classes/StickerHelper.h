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
	static void saveToMyStickerList(int stickerId);
	static void saveToMyStickerList(string stickerIdString);
	static int getCurrentExistSticker(bool withUniqueElements);
};

#endif
