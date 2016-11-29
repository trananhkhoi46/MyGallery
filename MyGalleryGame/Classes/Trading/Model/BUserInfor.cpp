
#include "BUserInfor.h"
#include "cocos2d.h"
BUserInfor::BUserInfor()
{
    
}

BUserInfor::~BUserInfor()
{
    
}

BUserInfor* BUserInfor::parseUserFrom(const rapidjson::Value& json) //Parse the data from Parse.com
{
    BUserInfor* user=new BUserInfor();
    user->setName(json[KEY_WORLD_NAME].GetString());
    user->setId(json[KEY_WORLD_ID].GetString());
    user->setAllStickers(json[KEY_WORLD_ALL_STICKERS].GetString());

    return user;
}

BUserInfor* BUserInfor::parseUserFrom(string message) //Parse the message from Facebook
{
    BUserInfor* user=BUserInfor::getMyInfor();

	rapidjson::Document document;
	document.Parse<0>(message.c_str());

    user->setName(document["name"].GetString());
    user->setId(document["id"].GetString());
    return user;
}

BUserInfor* BUserInfor::getMyInfor()
{
    static BUserInfor* user;
    if (!user) {
        user=new BUserInfor();
    }
    return user;
}
