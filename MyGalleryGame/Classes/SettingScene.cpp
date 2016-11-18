#include "SettingScene.h"
#include "HomeScene.h"

Scene* SettingScene::scene() {
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	SettingScene *layer = SettingScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init() {
	bool result = BaseScene::init();
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	Sprite* pSprite = Sprite::create(s_logo);
	// position the sprite on the center of the screen
	pSprite->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 2 + origin.y);
	this->addChild(pSprite, 0);

	return result;
}
