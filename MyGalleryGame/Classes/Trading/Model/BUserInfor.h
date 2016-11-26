
#ifndef __BUserInfor__
#define __BUserInfor__
#include <json/writer.h>
#include <json/stringbuffer.h>
#include <json/rapidjson.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/prettywriter.h>
#include <json/filestream.h>
#include <json/document.h>
#include "BLeaderBoardContanst.h"
using namespace rapidjson;
using namespace std;
class BUserInfor{
public:
    template <typename Writer>  void Serialize(Writer &writer) const {
        writer.StartObject();
        writer.String(KEY_WORLD_NAME);
        writer.String(_name.c_str());
        writer.String(KEY_WORLD_ID);
        writer.String(_id.c_str());
		writer.String(KEY_WORLD_LOCATION);
        writer.String(_location.c_str());
        writer.String(KEY_WORLD_SCORE);
        writer.Int(_score);
        writer.String(KEY_WORLD_INSTALLED_GAMES);
        writer.String("BoredNinja");
        writer.EndObject();
    }
    std::string serialize(){
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        Serialize(writer);
        return  s.GetString();
    }
    BUserInfor();
    ~BUserInfor();
    CC_SYNTHESIZE(string,_name,Name);
    CC_SYNTHESIZE(string,_id,Id);
    CC_SYNTHESIZE(string,_email,Email);
    CC_SYNTHESIZE(string,_objectId,ObjectId);
    CC_SYNTHESIZE(string,_location,Location);
    CC_SYNTHESIZE(int,_score,Score);
    static BUserInfor* getMyInfor();
    static BUserInfor* parseUserFrom(const rapidjson::Value& json);
    static BUserInfor* parseUserFrom(string message);
};
#endif /* defined(__SonTinhThuyTinh__BUserInfor__) */