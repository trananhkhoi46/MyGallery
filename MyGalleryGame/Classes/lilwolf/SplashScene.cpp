#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "MenuScene.h"
#include <thread>
#include <mutex>

using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;

Scene* SplashScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	SplashScene *layer = SplashScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	bool isSound = UserDefault::getInstance()->getBoolForKey(SOUND, true);
	UserDefault::getInstance()->setBoolForKey(SOUND, isSound);

	origin = Director::getInstance()->getVisibleOrigin();

	winSize = Director::getInstance()->getVisibleSize();
	Sprite* pSprite = Sprite::create(s_logo);
	// position the sprite on the center of the screen
	pSprite->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 2 + origin.y);
	this->addChild(pSprite, 0);


	SplashScene::PreloadSounds();

	return true;
}
void SplashScene::PreloadImages() {
	auto textureCache = Director::getInstance()->getTextureCache();
	int size = sizeof s_imgloading / sizeof s_imgloading[0];

	for (int var = 0; var < size; var++) {
		textureCache->addImage(s_imgloading[var]);
	}



	this->runAction(
		Sequence::create(DelayTime::create(0.5f),
		CallFunc::create(std::bind(&SplashScene::gameLogic, this)),
		nullptr));


}

void SplashScene::PreloadSounds() {
	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	int size = sizeof s_soundloading / sizeof s_soundloading[0];

	for (int var = 0; var < size; ++var) {
		audioEngine->preloadEffect(s_soundloading[var]);
	}


	this->runAction(CallFunc::create(std::bind(&SplashScene::PreloadImages, this)));
}

void SplashScene::gameLogic() {
	auto *newScene = MenuScene::scene();
	auto transition = TransitionFade::create(1.0, newScene);
	Director *pDirector = Director::getInstance();
	pDirector->replaceScene(transition);

}
