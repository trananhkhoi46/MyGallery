#ifndef _BLeaderBoardContanst_h
#define _BLeaderBoardContanst_h

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

//Key for Parse
static const string appID =
		"X-Parse-Application-Id: NowxMMqQy41HIZfgtSPK6fqSvxJGr2PS3XxwWHjf";
static const string restAPI =
		"X-Parse-REST-API-Key: D1IPNshGLcQTRmnwktdjFnyyJsnTFppBvskPGpLr";
static const string classURL = "https://api.parse.com/1/classes/BUser";
static const string firebaseURL =
		"https://gallerygame-fab40.firebaseio.com/json/%sauth=KKgD6eWhfoJC6KUCFwSwEGIJYzxkFAjnMOqNl6ir";
static const string firebaseQuerry_Probability_Freepacket =
		"probability_freepacket.json?";
static const string firebaseQuerry_Probability_Commonpacket =
		"probability_commonpacket.json?";
static const string firebaseQuerry_Probability_Uncommonpacket =
		"probability_uncommonpacket.json?";
static const string firebaseQuerry_Probability_Rarepacket =
		"probability_rarepacket.json?";
static const string firebaseQuerry_Probability_Veryrarepacket =
		"probability_veryrarepacket.json?";
static const string firebaseQuerry_Probability_Rarestpacket =
		"probability_rarestpacket.json?";
static const string firebaseQuerry_User = "users.json?";
static const string firebaseQuerry_Specific_User = "users/%s.json?";
static const string firebaseQuerry_IsThisUserExistOnFirebase =
		"users.json?orderBy=\"%s\"&equalTo=\"%s\"&";

#endif
