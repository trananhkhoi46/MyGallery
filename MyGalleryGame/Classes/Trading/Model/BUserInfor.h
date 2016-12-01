
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
#include "../BLeaderBoardContanst.h"
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
    CC_SYNTHESIZE(string,_allStickers,AllStickers);
    static BUserInfor* parseUserFrom(const rapidjson::Value& json);
    static BUserInfor* parseUserFrom(string message);
};
#endif /* defined(__SonTinhThuyTinh__BUserInfor__) */
