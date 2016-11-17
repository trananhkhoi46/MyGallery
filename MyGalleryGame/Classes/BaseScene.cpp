#include "BaseScene.h"

Scene* BaseScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	BaseScene *layer = BaseScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BaseScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	isSound = UserDefault::getInstance()->getBoolForKey(SOUND, true);
	UserDefault::getInstance()->setBoolForKey(SOUND, isSound);
	isMusic = UserDefault::getInstance()->getBoolForKey(MUSIC, true);
	UserDefault::getInstance()->setBoolForKey(MUSIC, isMusic);
	isAlert = UserDefault::getInstance()->getBoolForKey(ALERT, true);
	UserDefault::getInstance()->setBoolForKey(ALERT, isAlert);

	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();

	return true;
}

