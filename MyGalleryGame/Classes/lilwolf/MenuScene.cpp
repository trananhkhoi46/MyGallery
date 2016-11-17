#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "StoryScene.h"
#include "Resources.h"
#include "SplashScene.h"
#include "SocialPlugin.h"

using namespace CocosDenshion;

const int kTagSound = 0;
const int kTagLike = 1;
const int kTagMore = 2;
bool isSound;

Scene* MenuScene::scene() {
// 'scene' is an autorelease object
	Scene *scene = Scene::create();

// 'layer' is an autorelease object
	MenuScene *layer = MenuScene::create();
// add layer as a child to scene
	scene->addChild(layer);

// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init() {
//////////////////////////////
// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}


	isSound = UserDefault::getInstance()->getBoolForKey("isSound", true);
		if (isSound) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_gameon,true);
			UserDefault::getInstance()->setBoolForKey("isSound", true);
		}


	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();

    Sprite* background = Sprite::create(s_bgmenu);
	background->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 2 + origin.y);

	this->addChild(background, 0);
    boardLayer = Layer::create();
    boardLayer->setContentSize(CCSizeMake(winSize.width, winSize.height));
    boardLayer->setPosition(origin.x, origin.y);
    this->addChild(boardLayer, 1);
  
	title = Sprite::create(s_title);
	title->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 1.3 + origin.y);
	this->addChild(title, 2);
	title->setScale(0.3f, 0.3f);
    auto scaleTitle = ScaleTo::create(0.6f, 1.2f);
	auto scaleTitle2 = ScaleTo::create(0.4f, 1.0f);
	auto sequence = Sequence::create(scaleTitle, scaleTitle2, NULL);
	title->runAction(sequence);

	this->scheduleOnce(schedule_selector(MenuScene::distributeMenu), 1.5f);
	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased =
			CC_CALLBACK_2(MenuScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);
	time = 0;

	this->scheduleUpdate();
	firstClick = true;

		return true;
}
void MenuScene::distributeMenu(float dt) {

	btn_play = Button::create(s_mmplay_btn);
	btn_play->setPosition(
			Vec2(winSize.width /2 + origin.x, winSize.height *0.45 + origin.y));
    btn_play->setTouchEnabled(true);
    btn_play->setPressedActionEnabled(true);
	btn_play->addTouchEventListener(CC_CALLBACK_2(MenuScene::playButton, this));
	boardLayer->addChild(btn_play, 2);


	//add menu
	auto item1a = MenuItemImage::create(s_sound_on_btn, s_sound_on_btn, nullptr,
			nullptr);
    auto item1b = MenuItemImage::create(s_sound_off_btn, s_sound_off_btn,
			nullptr, nullptr);
	auto item1 = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(MenuScene::menuCallbackSound, this), item1a, item1b,
			nullptr);
    if(isSound){
        item1->setSelectedIndex(0);
    } else {
        item1->setSelectedIndex(1);
    }
    auto item2 = MenuItemImage::create(s_like_btn, s_like_btn,
			CC_CALLBACK_1(MenuScene::menuCallback, this));
	auto item3 = MenuItemImage::create(s_more_btn, s_more_btn,
			CC_CALLBACK_1(MenuScene::menuCallback, this));

	item1->setTag(kTagSound);
	item2->setTag(kTagLike);
	item3->setTag(kTagMore);

	auto menu = Menu::create(item1, item2, item3, nullptr);

	menu->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 5 + origin.y);
	menu->setTag(0);
	menu->alignItemsHorizontallyWithPadding(1);
	
    boardLayer->addChild(menu);
	boardLayer->setScale(0.0f, 0.0f);
    auto scaleTitle = ScaleTo::create(0.2f, 1.2f);
	auto scaleTitle2 = ScaleTo::create(0.1f, 1.0f);
	auto func = CallFunc::create([=]() {
		isSound = UserDefault::getInstance()->getBoolForKey(SOUND);
		        if (isSound){
		            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
		        }
		});
	auto func2 = CallFunc::create([=]() {
		auto scale = ScaleBy::create(0.7f, 1.1f);
		auto scale2 = scale->reverse();
		auto seq = Sequence::create(scale, scale2, nullptr);
		auto repeat = RepeatForever::create(seq);
		btn_play->runAction(repeat);
			});

	auto sequence = Sequence::create(scaleTitle, func,scaleTitle2,DelayTime::create(0.2f),func2, NULL);
	boardLayer->runAction(sequence);
}

void MenuScene::playButton(Ref* pSender, ui::Widget::TouchEventType eEventType) {
    if (eEventType == ui::Widget::TouchEventType::ENDED){
        isSound = UserDefault::getInstance()->getBoolForKey(SOUND);
        if (isSound){
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
        }
       
        auto *newScene = StoryScene::scene();
        auto transition = TransitionFade::create(1.0, newScene);
        Director *pDirector = Director::getInstance();
        pDirector->replaceScene(transition);
        
    }
}

void MenuScene::menuCallbackSound(Ref* sender) {
	isSound = UserDefault::getInstance()->getBoolForKey(SOUND);
	if (!isSound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_gameon,true);
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        
    }
	UserDefault::getInstance()->setBoolForKey(SOUND, !isSound);
}

void MenuScene::menuCallback(Ref* sender) {

	auto pMenuItem = dynamic_cast<MenuItem*>(sender);
	int tag = (int) pMenuItem->getTag();

	switch (tag) {
	case kTagLike:
			Application::getInstance()->openURL(
					"https://www.facebook.com/pages/Bored-Ninjas/804597329582378?ref=br_rs");
			break;
		case kTagMore:
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			Application::getInstance()->openURL(
					"itms-apps://itunes.apple.com/apps/MySoftSource");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			Application::getInstance()->openURL(
					"https://play.google.com/store/apps/developer?id=Bored+Ninjas");
#endif
			break;
	default:
		CCLOG("Error in Menu inflatten");
		break;
	}

}

void MenuScene::update(float delta) {

	time+=delta;

	if (time >= 2) {
		time = 0;
		firstClick = true;
	}
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keycode,
		Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {

		if (firstClick) {
			firstClick = false;
			time = 0;
			SocialPlugin::showToast("Press back again to Exit!");
		} else {
			if (time < 2) //2 clicks in "duration" for exit
			{
				CCDirector::sharedDirector()->end();
			} else
				firstClick = true;
		}

	}
}
