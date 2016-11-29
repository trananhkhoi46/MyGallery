#include "SocialPlugin.h"
#include "NDKHelper/NDKHelper.h"
using namespace std;

void SocialPlugin::showToast(std::string messageToShow) {
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(messageToShow), "messageToShow");
	SendMessageWithParams(string("showToast"), prms);
}
