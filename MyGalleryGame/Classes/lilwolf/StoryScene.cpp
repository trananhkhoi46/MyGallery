#include "StoryScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "Resources.h"

using namespace CocosDenshion;
using namespace cocos2d;

const int kTagSound = 0;
const int kTagLike = 1;
const int kTagMore = 2;

Scene* StoryScene::scene() {
// 'scene' is an autorelease object
	Scene *scene = Scene::create();

// 'layer' is an autorelease object
	StoryScene *layer = StoryScene::create();
// add layer as a child to scene
	scene->addChild(layer);

// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StoryScene::init() {
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

whichStoreIsShowing=1;

	origin = Director::getInstance()->getVisibleOrigin();
	winSize = Director::getInstance()->getVisibleSize();

    Sprite* background = Sprite::create(s_bgstory);
	background->setPosition(winSize.width / 2 + origin.x,
			winSize.height / 2 + origin.y);
	this->addChild(background);
  
	   Sprite* story1 = Sprite::create(s_story1);
		story1->setPosition(winSize.width / 2 + origin.x,
				winSize.height / 2 + origin.y);
		this->addChild(story1);

		story2 = Sprite::create(s_story2);
		story2->setPosition(winSize.width / 2 + origin.x,
						winSize.height / 2 + origin.y);
				this->addChild(story2);
				story2->runAction(FadeOut::create(0));


				story3 = Sprite::create(s_story3);
				story3->setPosition(winSize.width / 2 + origin.x,
										winSize.height / 2 + origin.y);
								this->addChild(story3);
								story3->runAction(FadeOut::create(0));

								auto quote = Sprite::create(s_story_quote);
								quote->setPosition(winSize.width / 2 + origin.x,
										winSize.height *0.1f + origin.y);
								quote->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
								quote->setScaleX(1.3f);
															this->addChild(quote);

															   TTFConfig config(s_font_1, 52 * s_font_ratio);
																labelStory = Label::createWithTTF(config, "Once upon a time, deep into the wood, there was a little wolf...",
																		TextHAlignment::CENTER);
																labelStory->setPosition(
																		Vec2(quote->getContentSize().width/2,
																				quote->getContentSize().height/2));
																labelStory->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
																labelStory->setColor(Color3B(185,129,72));
																quote->addChild(labelStory);

	auto btn_play = Button::create(s_story_playbtn);
	btn_play->setPosition(
			Vec2(winSize.width + origin.x, winSize.height+ origin.y));
    btn_play->setTouchEnabled(true);
    btn_play->setPressedActionEnabled(true);
    btn_play->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	btn_play->addTouchEventListener(CC_CALLBACK_2(StoryScene::playButton, this));
	this->addChild(btn_play);



	 //Handling touch event
	    auto listener = EventListenerTouchOneByOne::create();
	    listener->setSwallowTouches(true);

	    listener->onTouchBegan = CC_CALLBACK_2(StoryScene::onTouchBegan, this);
	    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//Keyboard handling
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased =
			CC_CALLBACK_2(StoryScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
			this);

		return true;
}
bool StoryScene::onTouchBegan(Touch* touch, Event* event)
{
	if(whichStoreIsShowing == 1)
	{
		 TTFConfig config(s_font_1, 48 * s_font_ratio);
		 labelStory->setTTFConfig(config);
		story2->runAction(FadeIn::create(0.5));
		labelStory->setString("One day, he met a legendary that his ancestors were always talking about.");
        labelStory->runAction(Sequence::create(FadeOut::create(0),FadeIn::create(0.5),nullptr));
		whichStoreIsShowing++;
	}else if(whichStoreIsShowing == 2)
	{
		 TTFConfig config(s_font_1, 51 * s_font_ratio);
				 labelStory->setTTFConfig(config);
		labelStory->setString("The Red Riding Hood. She wasn't like what they said.");
		labelStory->runAction(Sequence::create(FadeOut::create(0),FadeIn::create(0.5),nullptr));
		whichStoreIsShowing++;
	}
	else if(whichStoreIsShowing == 3)
		{
		 TTFConfig config(s_font_1, 43 * s_font_ratio);
				 labelStory->setTTFConfig(config);
			story3->runAction(FadeIn::create(0.5));
			labelStory->setString("And now he had to run for his life, to pay back what his ancestors had done to her.");
            labelStory->runAction(Sequence::create(FadeOut::create(0),FadeIn::create(0.5),nullptr));
			whichStoreIsShowing++;
		}
	else if(whichStoreIsShowing == 4)
		{
		 TTFConfig config(s_font_1, 52 * s_font_ratio);
				 labelStory->setTTFConfig(config);
			labelStory->setString("Help him run away from the frenzy Red Riding Hood!");
			labelStory->runAction(Sequence::create(FadeOut::create(0),FadeIn::create(0.5),nullptr));
			whichStoreIsShowing++;
		}
	else
	{
		goToPlayScene();
	}



	return true;
}
void StoryScene::playButton(Ref* pSender, ui::Widget::TouchEventType eEventType) {
    if (eEventType == ui::Widget::TouchEventType::ENDED){
       goToPlayScene();
    }
}

void StoryScene::goToPlayScene() {
	 isSound = UserDefault::getInstance()->getBoolForKey(SOUND);
	        if (isSound){
	            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
	        }

	        auto *newScene = GameScene::scene();
	        auto transition = TransitionFade::create(1.0, newScene);
	        Director *pDirector = Director::getInstance();
	        pDirector->replaceScene(transition);
}

void StoryScene::onKeyReleased(EventKeyboard::KeyCode keycode,
		Event* event) {

	if (EventKeyboard::KeyCode::KEY_ESCAPE == keycode) {
		goToPlayScene();
	}
}
