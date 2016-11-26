#ifndef _STICKER_RESOURCE_H_
#define _STICKER_RESOURCE_H_

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

//Enum
enum STICKER_RARITY {
	UNKNOWN, COMMON, UNCOMMON, RARE, VERYRARE, RAREST
};

class StickerPage {
public:
	inline static StickerPage* create(string sticker_page_name, string background_image,
			int sticker_page_id) {
		StickerPage* result = new StickerPage();
		result->background_image = background_image;
		result->sticker_page_id = sticker_page_id;
		result->sticker_page_name = sticker_page_name;
		return result;
	}
	string background_image;
	string sticker_page_name;
	int sticker_page_id;
};
class Sticker {
public:
	inline static Sticker* create(string sticker_name, string sticker_image,
			vector<string> sticker_image_animation, Vec2 position,
			string sticker_sound, STICKER_RARITY rarity, int sticker_id,
			int sticker_page_id) {
		Sticker* result = new Sticker();
		result->sticker_image = sticker_image;
		result->sticker_image_animation = sticker_image_animation;
		result->position = position;
		result->sticker_id = sticker_id;
		result->sticker_sound = sticker_sound;
		result->sticker_name = sticker_name;
		result->sticker_page_id = sticker_page_id;
		result->rarity = rarity;
		return result;
	}

	int sticker_id;
	int sticker_page_id;
	Vec2 position;
	STICKER_RARITY rarity;
	string sticker_sound;
	string sticker_image;
	string sticker_name;
	vector<string> sticker_image_animation;
};
//
//
//
//
//
//
//
//
//
//Sticker animation list
static vector<string> s_sticker_image_1_animation = {
		"sticker/page1/sticker_1_1.png", "sticker/page1/sticker_1_2.png" };
//
static vector<string> s_sticker_image_2_animation = {
		"sticker/page1/sticker_2_1.png", "sticker/page1/sticker_2_2.png" };
//
static vector<string> s_sticker_image_3_animation = {
		"sticker/page1/sticker_3_1.png", "sticker/page1/sticker_3_2.png" };
//
static vector<string> s_sticker_image_4_animation = {
		"sticker/page1/sticker_4_1.png", "sticker/page1/sticker_4_2.png" };
//
static vector<string> s_sticker_image_5_animation = {
		"sticker/page1/sticker_5_1.png", "sticker/page1/sticker_5_2.png" };
//
//
//
//
//
//
//
//
//
//Sticker information
static vector<Sticker*> vt_stickers = {
//
		Sticker::create("Jaguar","sticker/page1/sticker_1_1.png",
				s_sticker_image_1_animation, Vec2(100, 200),
				"sticker/page1/sound_1.ogg", STICKER_RARITY::COMMON, 1, 1),
//
		Sticker::create("Tom","sticker/page1/sticker_2_1.png",
				s_sticker_image_2_animation, Vec2(200, 200),
				"sticker/page1/sound_2.ogg", STICKER_RARITY::UNCOMMON, 2, 1),
//
		Sticker::create("Jerry","sticker/page1/sticker_3_1.png",
				s_sticker_image_3_animation, Vec2(300, 200),
				"sticker/page1/sound_3.ogg", STICKER_RARITY::RARE, 3, 1),
//
		Sticker::create("Harry","sticker/page1/sticker_4_1.png",
				s_sticker_image_4_animation, Vec2(400, 200),
				"sticker/page1/sound_4.ogg", STICKER_RARITY::VERYRARE, 4, 1),
//
		Sticker::create("Monster","sticker/page1/sticker_5_1.png",
				s_sticker_image_5_animation, Vec2(500, 200),
				"sticker/page1/sound_5.ogg", STICKER_RARITY::RAREST, 5, 1),
//
		};
//
//
//
//
//
//
//
//
//
static vector<StickerPage*> vt_sticker_pages = {
//
		StickerPage::create("Jungle 1","sticker/page1/bg_1.png", 1),
		StickerPage::create("Jungle 2","sticker/page2/bg_2.png", 2),
//
		};
#endif
