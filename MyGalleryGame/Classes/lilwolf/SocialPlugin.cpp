#include "SocialPlugin.h"
#include "NDKHelper.h"
#include "FacebookAgent.h"
using namespace std;
using namespace cocos2d::plugin;

void SocialPlugin::shareFacebook()
{
	FacebookAgent::FBInfo params;
			params.insert(std::make_pair("dialog", "shareLink"));
			params.insert(std::make_pair("link", "http://www.boredninjas.com/games/RedHood.html"));
			params.insert(std::make_pair("caption", "The game of Bored Ninjas"));
			params.insert(std::make_pair("name", "Lil Wolf"));
			params.insert(
					std::make_pair("message", "It's a great game from Bored Ninjas you must try!"));
			params.insert(std::make_pair("title", "Lil Wolf"));
			params.insert(
					std::make_pair("picture",
							"http://www.boredninjas.com/games/RedHood.png"));
			params.insert(std::make_pair("description", "It's a great game from Bored Ninjas you must try!"));
			if (FacebookAgent::getInstance()->canPresentDialogWithParams(params)) {
				FacebookAgent::getInstance()->dialog(params,
						[=](int ret, std::string& msg) {
							CCLOG("%s", msg.c_str());
						});
			} else {
				//Show facebook feed if the user doesnt install facebook app
				FacebookAgent::FBInfo params2;
				params2.insert(std::make_pair("dialog", "feedDialog"));
				params2.insert(std::make_pair("link", "http://www.boredninjas.com/games/RedHood.html"));
				params2.insert(std::make_pair("caption", "The game of Bored Ninjas"));
				params2.insert(std::make_pair("name", "Lil Wolf"));
				params2.insert(
									std::make_pair("message", "It's a great game from Bored Ninjas you must try!"));
				params2.insert(std::make_pair("title", "Lil Wolf"));
				params2.insert(
									std::make_pair("picture",
											"http://www.boredninjas.com/games/RedHood.png"));
				params2.insert(std::make_pair("description", "It's a great game from Bored Ninjas you must try!"));
				FacebookAgent::getInstance()->dialog(params2,
						[=](int ret, std::string& msg) {
							CCLOG("%s", msg.c_str());
						});

			}
}
void SocialPlugin::showToast(std::string messageToShow)
{
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(messageToShow), "messageToShow");
	SendMessageWithParams(string("showToast"), prms);

}
void SocialPlugin::shareScreenshotTwitter_CopyThisCodeToYourClass()
{
	CCLog("Please click take a look at SocialPlugin.cpp and copy the code to shareTwitter to your class.");

//-------------------------------------------------
	//MyShareManager::getInstance()->loadSharePlugin();// Put this code to Init ---------------- important

//#include "PluginManager.h"
//#include "MyShareManager.h"
//-------------------------------------------------

//	Then add 2 function below to your class

//	void GameOverScene::takeScreenshotThenShareTwitter() {
//		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
//		CCRenderTexture * tex = CCRenderTexture::create(screenSize.width,
//				screenSize.height);
//		tex->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
//
//		tex->begin();
//		this->getParent()->visit();
//		tex->end();
//
//		tex->saveToFile("screenshot.png", Image::Format::PNG);
//
//		utils::captureScreen(CC_CALLBACK_2(GameOverScene::afterCaptured, this),
//				"screenshot.png");
//
//	}
//	void GameOverScene::afterCaptured(bool succeed, const std::string &outputFile) {
//
//		if (succeed) {
//			std::string imgPath = outputFile;
//
//			auto delay = DelayTime::create(1);
//			auto share =
//					CallFunc::create(
//							[=]() {
//
//
//				cocos2d::plugin::TShareInfo pInfo;
//				    pInfo["SharedText"] = "Triangle is a masterpiece of Bored Ninjas!";
//				    pInfo["SharedImagePath"] = imgPath;
//				    MyShareManager::getInstance()->shareByMode(pInfo);
//
//							});
//
//			auto seq = Sequence::create(delay, share, nullptr);
//			runAction(seq);
//		} else {
//			log("Capture screen failed.");
//		}
//	}

}
