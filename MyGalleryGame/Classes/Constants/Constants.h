#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_


#include "cocos2d.h"
using namespace std;

//Global key
#define RANKINGWORLDMODE "isrankingworldmode"

//Define for BUserInfo
#define KEY_FACEBOOK_FIRST_TIME_LOGGING_IN "keyfacebookfirsttimeloggingin"
#define KEY_PROBABILITY_COMMON "common"
#define KEY_PROBABILITY_UNCOMMON "uncommon"
#define KEY_PROBABILITY_RARE "rare"
#define KEY_PROBABILITY_VERYRARE "veryrare"
#define KEY_PROBABILITY_RAREST "rarest"

#define KEY_WORLD_NAME       "FB_Name"
#define KEY_WORLD_LOCATION   "FB_Location"
#define KEY_WORLD_ID         "FB_ID"
#define KEY_WORLD_SCORE      "Coin"
#define KEY_WORLD_OJECTID    "ObjectID"
#define KEY_WORLD_INSTALLED_GAMES    "InstalledGame"
#define KEY_IS_FIRST_TIME_LOGIN_INTO_THIS_GAME "isfirsttimeloginintothisgame"
#define THIS_GAME_NAME "SpringNinja"
#define KEY_WORLD_ALL_STICKERS "All_Stickers"
#define KEY_WORLD_STICKED_STICKERS "Sticked_Stickers"
#define KEY_WORLD_PENDING_REQUEST "Pending_Request"
#define KEY_WORLD_GIVEN_STICKERS "Given_Stickers"



//Admob key (corresponding with .json file in Resources)
#define kAdmobInstitialAds "fullscreen_ads"
#define kChartboostInstitialAds "fullscreen_ads"
#define kVungleInstitialAds "video"

#define kChartboostRewardedAds "rewarded_ads"
#define kVungleRewardedAds "reward"

//Global KEY
#define SOUND "sound"
#define MUSIC "music"
#define ALERT "alert"
#define CURRENT_STICKER "currentsticker"  //An array of gotten sticker id: 1,2,4,6,3,8,5
#define CURRENT_PACKET "currentpacket"  //An array of packet rarity: 1,2,4
#define STICKED_STICKER "stickedsticker"  //An array of gotten sticker id: 1,2,4,6,3,8,5
#define TIME_TO_GET_FREE_STICKER_IN_SECOND "TIME_TO_GET_FREE_STICKER_IN_SECOND"

#define KEY_PROBABILITY_FREEPACKET_COMMON "common"
#define KEY_PROBABILITY_FREEPACKET_UNCOMMON "uncommon"
#define KEY_PROBABILITY_FREEPACKET_RARE "rare"
#define KEY_PROBABILITY_FREEPACKET_VERYRARE "veryrare"
#define KEY_PROBABILITY_FREEPACKET_RAREST "rarest"

//Sticker max number
#define MAX_STICKER 5
#define SELLING_STICKER_NUMBER_TO_GET_A_PACKET 4
#define TIME_TO_GET_FREE_PACKET_IN_SECOND 300
#define GIVEN_PACKETS_AT_FIRST_INSTALL "0,1,2,2,2"

//URL
#define s_linkToGooglePlayMoreGame "https://play.google.com/store/apps/developer?id=Khoi+Nguyen+Entertainment"
#define s_linkToAppStoreMoreGame "itms-apps://itunes.apple.com/apps/MySoftSource"
#define s_linkToGooglePlayRating "https://play.google.com/store/apps/details?id=com.khoinguyenentertainment.onetanimal"
#define s_linkToAppStoreRating "itms-apps://itunes.apple.com/apps/MySoftSource"
#define s_linkToFacebookPage "https://www.facebook.com/pages/Bored-Ninjas/804597329582378?ref=br_rs"

#endif
