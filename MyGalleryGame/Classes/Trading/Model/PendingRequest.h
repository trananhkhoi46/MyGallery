#ifndef __PendingRequest__
#define __PendingRequest__

#include "../BLeaderBoardContanst.h"
using namespace std;
class PendingRequest {
public:

	PendingRequest(){

	};
	~PendingRequest();CC_SYNTHESIZE(string,_name,Name)
	;CC_SYNTHESIZE(string,_stickerId,StickerId)
	;CC_SYNTHESIZE(string,_objectId,ObjectId)
	;
};
#endif
