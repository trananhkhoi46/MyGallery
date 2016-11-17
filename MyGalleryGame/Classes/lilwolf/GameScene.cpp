#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "CppUtils.h"
#include "SonarFrameworks.h"
#include "SocialPlugin.h"
#include "MenuScene.h"
#include "RankingScene.h"
#include "FacebookAgent.h"
#include "AdsLayer.h"

using namespace cocos2d::plugin;
using namespace CocosDenshion;

const int kTagDecorating = 50;
const int kTagNoneLand = 51;
const int kTagDangerousLand = 52;
const int kTagRealLand = 53;
const int kTagBonus = 54;
const int kTagPlayerAnimate = 2411;

Scene* GameScene::scene() {
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    // add layer as a child to scene
    layer->setTag(kTagLayerHavingPauseDialog);
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    isSound = UserDefault::getInstance()->getBoolForKey(SOUND,true);
    if (isSound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_gameon,true);
    }

    origin = Director::getInstance()->getVisibleOrigin();
    winSize = Director::getInstance()->getVisibleSize();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SonarCocosHelper::IOS::Setup();
#endif
    SonarCocosHelper::AdMob::hideBannerAd(SonarCocosHelper::AdBannerPosition::eBoth);
    
    SonarCocosHelper::GoogleAnalytics::setScreenName("PlayScene");
    SonarCocosHelper::GoogleAnalytics::sendEvent("LilWolf","Play game","User",0);

	//Default value of Game
	gameVelocity = 8; // velocity = 8px*60frames per second
	enemyVelocity = 3;
	playerLandIndex = 4;
	enemyLandIndex = -1;
	bonusLandIndex = -1;
	landToRemoveChild = NULL;
	animationInterval = Director::getInstance()->getAnimationInterval();

    //Add scrolling background
    background = Sprite::create(s_bgGameScene);
    background->setAnchorPoint(Vec2::ZERO);
    background->setPosition(0,origin.y);
    this->addChild(background);

    background2 = Sprite::create(s_bgGameScene);
    background2->setAnchorPoint(Vec2::ZERO);
    background2->setPosition(0 + background->getContentSize().width,origin.y);
    this->addChild(background2);

    //Add animal eyes into background
     addEyesToBackground(background);
     addEyesToBackground(background2);

     //Add waves into background
	  addWavesToBackground(background);
	  addWavesToBackground(background2);

     //Add land
     addLand();

     //Add player
    player = Sprite::create(s_player);
    player->setPosition(getLandOnScreen(playerLandIndex)->getPositionX() + getLandOnScreen(playerLandIndex)->getContentSize().width/2 ,getLandOnScreen(playerLandIndex+1)->getPositionY() + getLandOnScreen(playerLandIndex+1)->getContentSize().height-10);
    this->addChild(player);

	//Add Enemy
	enemy = Sprite::create(s_enemy);
	enemy->setPosition(-30 - vt_Lands[0]->getContentSize().width * 0.5,
			winSize.height * 0.08 + vt_Lands[0]->getContentSize().height + 60);
	this->addChild(enemy);

	//Add pause button
	auto btn_pause = Button::create(s_btn_pause);
	btn_pause->setPosition(Vec2(winSize.width + origin.x, winSize.height));
	btn_pause->setTouchEnabled(true);
	btn_pause->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	btn_pause->addTouchEventListener(CC_CALLBACK_2(GameScene::pauseGame, this));
	this->addChild(btn_pause);

	//Add paper score and label score
		auto paper_sprite = Sprite::create(s_paper_score);
		paper_sprite->setPosition(winSize.width / 2, winSize.height);
		paper_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		this->addChild(paper_sprite);

		TTFConfig config(s_font_1, 80 * s_font_ratio);
		labelScore = Label::createWithTTF(config, "0", TextHAlignment::CENTER);
		labelScore->setPosition(
				Vec2(winSize.width / 2,
						winSize.height
								- paper_sprite->getContentSize().height / 2));
		labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelScore->setColor(Color3B(185, 129, 72));
		this->addChild(labelScore);

		//Animation duration
			float velocityGameBySecond = gameVelocity / animationInterval;
			float scrollingOverLandsDuration = winSize.width / 10 / velocityGameBySecond; //1 land = 1/10 winSize.width

			//Player animation
			int frameAmount_player = 6;
			Vector<SpriteFrame*> animFrames_player(frameAmount_player);
			for (int i = 0; i < frameAmount_player; i++) {
				Rect rect = Rect(0, 0, 188, 203);
				auto frame = SpriteFrame::create(s_player_sheet[i], rect);
				animFrames_player.pushBack(frame);
			}
			float duration_player = scrollingOverLandsDuration / frameAmount_player;
			auto animation_player = Animation::createWithSpriteFrames(animFrames_player,duration_player*1.4f);
			animate_player = Animate::create(animation_player);
			animate_player->setTag(kTagPlayerAnimate);
			animate_player->retain();

			//Enemy animation
			int frameAmount_enemy = 7;
			Vector<SpriteFrame*> animFrames_enemy(frameAmount_enemy);
			for (int i = 0; i < frameAmount_enemy; i++) {
				Rect rect = Rect(0, 0, 163, 255);
				auto frame = SpriteFrame::create(s_enemy_sheet[i], rect);
				animFrames_enemy.pushBack(frame);
			}
			float duration_enemy = scrollingOverLandsDuration
					/ (frameAmount_enemy * 1.5f); //Nhanh them 1.5 lan`
			auto animation_enemy = Animation::createWithSpriteFrames(animFrames_enemy,
					duration_enemy);
			animate_enemy = Animate::create(animation_enemy);
			animate_enemy->retain();


    //Handling touch event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //Keyboard handling
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    scheduleUpdate();


    //Show tutorial
    	auto func = CallFunc::create([=]() {
    		if (UserDefault::getInstance()->getBoolForKey("isFirstUse", true))
    				showTutorial();
    						});
    	this->runAction(Sequence::create(DelayTime::create(1.0f),func,nullptr));

    return true;
}
void GameScene::showTutorial() {

	tutorialLayer = TutorialLayer::create();
	this->addChild(tutorialLayer);

	numberOfTutorialContent = 2;
	auto tutorialString = "Tap at the left side to jump 1 step\nTap at the right side to jump 2 steps";
	int fontSize = 60;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fontSize = 32;
#endif
	tutorialLayer->addDialog(tutorialString, winSize.width/2,winSize.height/2, fontSize,Color3B(185, 129, 72));
	isTuturialBeShowed = true;
}
void GameScene::addWavesToBackground(Sprite* background) {
	int posX = 0;
	int posY = 0;
	for (int i = 0; i < 3; i++) {
		Sprite* wave;
		wave = Sprite::create(s_wave_sheet[0]);
		int distanceX = CppUtils::randomBetween(
				background->getContentSize().width * 0.3,
				background->getContentSize().width * 0.36);
		posX += distanceX;
		posY = CppUtils::randomBetween(background->getContentSize().height * 0,
				background->getContentSize().height * 0.32);
		if (posX
				>= background->getContentSize().width
						- wave->getContentSize().width)
			posX = background->getContentSize().width
					- wave->getContentSize().width;
		wave->setPosition(posX, posY);
		wave->setTag(kTagDecorating);
		background->addChild(wave);

		//Wave animation
		int frameAmount_wave = 6;
		Vector<SpriteFrame*> animFrames_wave(frameAmount_wave);
		for (int i = 0; i < frameAmount_wave; i++) {
			Rect rect = Rect(0, 0, 384, 77);
			auto frame = SpriteFrame::create(s_wave_sheet[i], rect);
			animFrames_wave.pushBack(frame);
		}
		auto animation = Animation::createWithSpriteFrames(animFrames_wave,
				0.3f);
		auto animate = Animate::create(animation);
		wave->runAction(RepeatForever::create(animate));
	}

}
void GameScene::pauseGame(Ref* pSender, ui::Widget::TouchEventType eEventType) {
	if (eEventType == ui::Widget::TouchEventType::ENDED) {

		showPauseDialog();
	}
}

void GameScene::showPauseDialog() {
	if (isPauseGame || isGameOver || isTuturialBeShowed)
		return;
	isPauseGame = true;
	if (isSound) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	layerTrum = LayerColor::create(Color4B(0, 0, 0, 255 * 0.5f));
	layerTrum->setContentSize(visibleSize);
	layerTrum->setPosition(Vec2::ZERO);
	layerTrum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(layerTrum);

	pauseDialogLayer = Layer::create();
	pauseDialogLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pauseDialogLayer->setContentSize(visibleSize);
	pauseDialogLayer->setPosition(Vec2(origin.x, origin.y));

	Sprite* board = Sprite::create(s_dialog);
	board->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	board->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height / 2));
	pauseDialogLayer->addChild(board);

	auto pauseTxt = Sprite::create(s_pausetext);
	pauseTxt->setAnchorPoint(Point(0.5f, 0.5f));
	pauseTxt->setPosition(
			Vec2(pauseDialogLayer->getContentSize().width * 0.5 + origin.x,
					pauseDialogLayer->getContentSize().height * 0.7
							+ origin.y));
	pauseDialogLayer->addChild(pauseTxt);

	auto pauseImage = Sprite::create(s_pauseimage);
	pauseImage->setAnchorPoint(Point(0.5f, 0.5f));
	pauseImage->setPosition(
			Vec2(pauseDialogLayer->getContentSize().width * 0.5 + origin.x,
					pauseDialogLayer->getContentSize().height * 0.25
							+ origin.y));
	pauseImage->setScale(1.5f);
	pauseDialogLayer->addChild(pauseImage);

	//Replay button
	Button* replayBtn = cocos2d::ui::Button::create(s_replay_btn);
	replayBtn->setTouchEnabled(true);
	replayBtn->setPressedActionEnabled(true);
	replayBtn->setAnchorPoint(Point(0.5f, 0.5f));
	replayBtn->setPosition(
			Vec2(pauseDialogLayer->getContentSize().width * 0.38f + origin.x,
					pauseDialogLayer->getContentSize().height * 0.45
							+ origin.y));

	replayBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
					}

					auto *newScene = GameScene::scene();
					auto transition = TransitionFade::create(1.0, newScene);
					Director *pDirector = Director::getInstance();
					pDirector->replaceScene(transition);
				}
			});
	pauseDialogLayer->addChild(replayBtn);

	//Continue button
	Button* continueBtn = cocos2d::ui::Button::create(s_play_btn);
	continueBtn->setTouchEnabled(true);
	continueBtn->setPressedActionEnabled(true);
	continueBtn->setAnchorPoint(Point(0.5f, 0.5f));
	continueBtn->setPosition(
			Vec2(pauseDialogLayer->getContentSize().width * 0.5 + origin.x,
					pauseDialogLayer->getContentSize().height * 0.45
							+ origin.y));

	continueBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
					}
					continutePlay();
				}
			});
	pauseDialogLayer->addChild(continueBtn);

	//Back to Menu button
	Button* menuBtn = cocos2d::ui::Button::create(s_home_btn);
	menuBtn->setTouchEnabled(true);
	menuBtn->setPressedActionEnabled(true);
	menuBtn->setAnchorPoint(Point(0.5f, 0.5f));
	menuBtn->setPosition(
			Vec2(pauseDialogLayer->getContentSize().width * 0.62f + origin.x,
					pauseDialogLayer->getContentSize().height * 0.45
							+ origin.y));

	menuBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
					}
					auto *newScene = MenuScene::scene();
					auto transition = TransitionFade::create(1.0, newScene);
					Director *pDirector = Director::getInstance();
					pDirector->replaceScene(transition);
				}
			});
	pauseDialogLayer->addChild(menuBtn);

	pauseDialogLayer->setScale(0.0f, 0.0f);
	auto scale = ScaleTo::create(0.2f, 0.9f);
	pauseDialogLayer->runAction(scale);
	layerTrum->addChild(pauseDialogLayer);
}
void GameScene::continutePlay() {
	if(isPauseGame == false)
		return;

	auto func = CallFunc::create([=]() {
		isPauseGame = false;
		pauseDialogLayer->setVisible(false);
		layerTrum->setVisible(false);
	});
	auto scale = ScaleTo::create(0.1f, 0.0f);
	auto seq2 = Sequence::create(scale, func, nullptr);

	pauseDialogLayer->runAction(seq2);

	if (isSound) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
	}
}

void GameScene::showGameoverDialog() {
	isGameOver = true;



	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	layerTrum = LayerColor::create(Color4B(0, 0, 0, 255 * 0.5f));
	layerTrum->setContentSize(visibleSize);
	layerTrum->setPosition(Vec2::ZERO);
	layerTrum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(layerTrum);

	gameoverDialogLayer = Layer::create();
	gameoverDialogLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gameoverDialogLayer->setContentSize(visibleSize);
	gameoverDialogLayer->setPosition(Vec2(origin.x, origin.y));

	//Board of dialog
	Sprite* board = Sprite::create(s_dialog);
	board->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	board->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height / 2));
	gameoverDialogLayer->addChild(board);

	//Dialog title
	auto gameoverTxt = Sprite::create(s_gameovertext);
	gameoverTxt->setAnchorPoint(Point(0.5f, 0.5f));
	gameoverTxt->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.5 + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.8
							+ origin.y));
	gameoverDialogLayer->addChild(gameoverTxt);

	//Image at dialog's bottom
	auto gameoverImage = Sprite::create(s_gameoverimage);
	gameoverImage->setAnchorPoint(Point(0.5f, 0.5f));
	gameoverImage->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.5 + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.22
							+ origin.y));
	gameoverImage->setScale(1.2f);
	gameoverDialogLayer->addChild(gameoverImage);

	//Label high score
	TTFConfig config(s_font_1, 80 * s_font_ratio);
	int highScore = UserDefault::getInstance()->getIntegerForKey(HIGHSCORE,
			score);
	if(score>=highScore){
		highScore = score;
		UserDefault::getInstance()->setIntegerForKey(HIGHSCORE, highScore);
	}
    auto func = CallFunc::create([=]() {
        int gameOverTimes = UserDefault::getInstance()->getIntegerForKey("GAME_OVER_TIMES",1);
        if(gameOverTimes >= 3)
        {
        	int rand = CCRANDOM_0_1()*3;
        	if(rand == 1)
        		SonarCocosHelper::AdMob::showFullscreenAd();
        	else if(rand == 2)
        		SonarCocosHelper::Chartboost::showFullscreenAd();
        	else
        	{
				AdsLayer* adsLayer = AdsLayer::create();
				adsLayer->setPosition(Vec2(winSize.width/2,winSize.height/2));
				this->addChild(adsLayer);
				adsLayer->addDialog();
        	}
            UserDefault::getInstance()->setIntegerForKey("GAME_OVER_TIMES",1);
        }
        else
            UserDefault::getInstance()->setIntegerForKey("GAME_OVER_TIMES",gameOverTimes+1);
    });
    auto func2 = CallFunc::create([=]() {
        SonarCocosHelper::AdMob::showBannerAd(SonarCocosHelper::AdBannerPosition::eBoth);
    });
    auto funcPostScore = CallFunc::create([=]() {
        postScoreToServer(highScore);
    });
    
    this->runAction(Sequence::create(func,funcPostScore,func2,nullptr));
    
    
    
    
    
    
	auto labelHighScoreInDialog = Label::createWithTTF(config,
			"High score " + CppUtils::doubleToString(highScore),
			TextHAlignment::CENTER);
	labelHighScoreInDialog->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.5 + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.4
							+ origin.y));
	labelHighScoreInDialog->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelHighScoreInDialog->setColor(Color3B(185, 129, 72));
	gameoverDialogLayer->addChild(labelHighScoreInDialog);

	//Label Score
	auto labelScoreInDialog = Label::createWithTTF(config,
			"Your score " + CppUtils::doubleToString(score),
			TextHAlignment::CENTER);
	labelScoreInDialog->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.5 + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.48
							+ origin.y));
	labelScoreInDialog->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelScoreInDialog->setColor(Color3B(185, 129, 72));
	gameoverDialogLayer->addChild(labelScoreInDialog);

	//Ranking button
	Button* rankingBtn = cocos2d::ui::Button::create(s_ranking_btn);
	rankingBtn->setTouchEnabled(true);
	rankingBtn->setPressedActionEnabled(true);
	rankingBtn->setAnchorPoint(Point(0.5f, 0.5f));
	rankingBtn->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.38f + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.6
							+ origin.y));

	rankingBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound)
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);

					if(isRankingTouchable)
					{
						auto *newScene = RankingScene::scene();
						auto transition = TransitionFade::create(1.0, newScene);
						Director *pDirector = Director::getInstance();
						pDirector->replaceScene(transition);
					}else
					{
						SocialPlugin::showToast("We're posting your score onto server.\nPlease wait for a few seconds then try again.");
					}
				}
			});
	gameoverDialogLayer->addChild(rankingBtn);

	//Replay button
	Button* replayBtn = cocos2d::ui::Button::create(s_replay_btn);
	replayBtn->setTouchEnabled(true);
	replayBtn->setPressedActionEnabled(true);
	replayBtn->setAnchorPoint(Point(0.5f, 0.5f));
	replayBtn->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.5 + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.6
							+ origin.y));

	replayBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
					}
					auto *newScene = GameScene::scene();
					auto transition = TransitionFade::create(1.0, newScene);
					Director *pDirector = Director::getInstance();
					pDirector->replaceScene(transition);
				}
			});
	gameoverDialogLayer->addChild(replayBtn);

	//Share facebook button
	Button* facebookBtn = cocos2d::ui::Button::create(s_fb_btn);
	facebookBtn->setTouchEnabled(true);
	facebookBtn->setPressedActionEnabled(true);
	facebookBtn->setAnchorPoint(Point(0.5f, 0.5f));
	facebookBtn->setPosition(
			Vec2(gameoverDialogLayer->getContentSize().width * 0.62f + origin.x,
					gameoverDialogLayer->getContentSize().height * 0.6
							+ origin.y));

	facebookBtn->addTouchEventListener(
			[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
				{
					if (isSound) {
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_click);
					}
					SocialPlugin::shareFacebook();
				}
			});
	gameoverDialogLayer->addChild(facebookBtn);

	gameoverDialogLayer->setScale(0.0f, 0.0f);
	auto scale = ScaleTo::create(0.2f, 0.9f);
	gameoverDialogLayer->runAction(scale);
	layerTrum->addChild(gameoverDialogLayer);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (isGameOver)
			return;

		if (isPauseGame == false) {
			showPauseDialog();
		} else {
			continutePlay();
		}

	}
}

void GameScene::moveEnemy(float dt)
{
	//Scroll enemy folowing background
	float scrollVelocity;
		if (!isLandMoved) {

			if (isPlayer2Steps)
				scrollVelocity = gameVelocity * 2;
			else
				scrollVelocity = gameVelocity;

			float x1 = enemy->getPosition().x;
			float y1 = enemy->getPosition().y;
			x1 += -scrollVelocity;
			y1 += 0;
			enemy->setPosition(x1, y1);


		}



	if(enemy->getPositionX() <= -47)
	{
		isEnemyApearing = true;
		if(isEnemyJumping)
		{
			enemy->stopAllActions();
			isEnemyJumping = false;
			enemy->setVisible(false);
		}
		enemyAppearTimer+=dt;
		float timeToAppearEnemy = 2.0f/enemyVelocity;

		if(!isStartedPlaying)
		{
			//Theo game goc, moi vo thi 3s moi xuat hien enemy, nhung khi choi roi thi 1.5s la enemy da duoi kip
			timeToAppearEnemy = 3;
		}
		if(enemyAppearTimer>=timeToAppearEnemy && isLandMoved)
		{
			enemyLandIndex =-1;
			enemyAppearTimer = 0;
			enemyJumpTimer = 0;
			enemy->setVisible(true);
			enemy->setPosition(-30 - vt_Lands[0]->getContentSize().width * 0.5,
						winSize.height * 0.08 + vt_Lands[0]->getContentSize().height + 60);
			//Nhay vo man hinh - mac dinh la ban dau enemy xuat hien o line duoi
			 float velocityGameBySecond = enemyVelocity / animationInterval; //60 frames per second
			 float scrollingOverLandsDuration = vt_Lands[0]->getContentSize().width / velocityGameBySecond;
			 auto funcAfterJump =
						     CallFunc::create(
						                      [=]() {

													isEnemyApearing = false;






													//Wave animation
													auto landToRunAction = getLandOnScreen(enemyLandIndex);
													if(landToRunAction != NULL && landToRunAction->getTag()!=kTagNoneLand)
													{
														Sprite* wave_land_sprite = Sprite::create(s_wave_land_sheet[0]);
														wave_land_sprite->setPosition(landToRunAction->getContentSize().width/2,landToRunAction->getContentSize().height*0.2);
														wave_land_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
														landToRunAction->addChild(wave_land_sprite);

														int frameAmount_waveLand = 4;
														Vector<SpriteFrame*> animFrames_waveLand(frameAmount_waveLand);
														for (int i = 0; i < frameAmount_waveLand; i++) {
															Rect rect = Rect(0, 0, 209, 55);
															auto frame = SpriteFrame::create(s_wave_land_sheet[i], rect);
															animFrames_waveLand.pushBack(frame);
														}
														auto animation_waveLand = Animation::createWithSpriteFrames(animFrames_waveLand, 0.12f);
														auto animate_waveLand = Animate::create(animation_waveLand);
														auto func = CallFunc::create([=]() {
															landToRunAction->removeChild(wave_land_sprite,false);
																});
														auto seq = Sequence::create(animate_waveLand,func,nullptr);
														wave_land_sprite->runAction(seq);
													}
						                      });
			 auto jumpAction = Sequence::create(MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2,90)),MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2,-90)),funcAfterJump,nullptr);
			 auto jumpAction_Up = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2,120)),MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1,-60)),funcAfterJump,nullptr);
			 isEnemy2Steps = false;
			 if(getLandOnScreen(0)->getTag()==kTagNoneLand)
				 isEnemy2Steps = true;
			 int landToCheck = enemyLandIndex+1;
			 if(isEnemy2Steps)
			 {
				 jumpAction = Sequence::create(MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2*2,90)),MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2*2,-90)),funcAfterJump,nullptr);
				 jumpAction_Up = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2*2,120)),MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1*2,-60)),funcAfterJump,nullptr);
				 landToCheck++;
			 }
			 if(checkIsLandAtAboveLine(getLandOnScreen(landToCheck)))
			 {
				 jumpAction = jumpAction_Up;
			 }
			 enemy->runAction(jumpAction);
			 //Animation
			enemy->runAction(animate_enemy);
			 if(isEnemy2Steps)
				enemyLandIndex++;
			enemyLandIndex++;

		}
	}else
	{
		if(isEnemyApearing)
			return;
		//Jump enemy
		enemyJumpTimer+=dt;
		if(enemyJumpTimer>=0.2f && isLandMoved && enemyLandIndex>-1 && !isEnemyJumping)
		{
			enemyJumpTimer=0;
			isEnemyJumping = true;

			//Nhay vo man hinh - mac dinh la enemy xuat hien o line duoi
			float velocityGameBySecond = enemyVelocity / animationInterval; //60 frames per second
			 float scrollingOverLandsDuration = vt_Lands[0]->getContentSize().width / velocityGameBySecond;
			 auto funcAfterJump =
			     CallFunc::create(
			                      [=]() {
			                         if(enemyLandIndex == playerLandIndex && !isGameOver)
			                         {
			                        	showGameoverDialog();
			                         }
			                         isEnemyJumping = false;



			                     	auto landNe = getLandOnScreen(enemyLandIndex);


			                     	//Wave animation
									auto landToRunAction = getLandOnScreen(enemyLandIndex);
									if(landToRunAction != NULL && landToRunAction->getTag()!=kTagNoneLand)
									{
										Sprite* wave_land_sprite = Sprite::create(s_wave_land_sheet[0]);
										wave_land_sprite->setPosition(landToRunAction->getContentSize().width/2,landToRunAction->getContentSize().height*0.2);
										wave_land_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
										landToRunAction->addChild(wave_land_sprite);

										int frameAmount_waveLand = 4;
										Vector<SpriteFrame*> animFrames_waveLand(frameAmount_waveLand);
										for (int i = 0; i < frameAmount_waveLand; i++) {
											Rect rect = Rect(0, 0, 209, 55);
											auto frame = SpriteFrame::create(s_wave_land_sheet[i], rect);
											animFrames_waveLand.pushBack(frame);
										}
										auto animation_waveLand = Animation::createWithSpriteFrames(animFrames_waveLand, 0.12f);
										auto animate_waveLand = Animate::create(animation_waveLand);
										auto func = CallFunc::create([=]() {
											landToRunAction->removeChild(wave_land_sprite,false);
												});
										auto seq = Sequence::create(animate_waveLand,func,nullptr);
										wave_land_sprite->runAction(seq);
									}
			                      });
			 auto jumpAction = Sequence::create(MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2,90)),MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2,-90)),funcAfterJump,nullptr);
			 auto jumpAction_Up = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2,120)),MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1,-60)),funcAfterJump,nullptr);
			 auto jumpAction_Down = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1,60)),MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2,-120)),funcAfterJump,nullptr);
			 int nextEnemyLandIndex = enemyLandIndex+1;
			 isEnemy2Steps = false;
			 if(getLandOnScreen(nextEnemyLandIndex)->getTag()==kTagNoneLand || nextEnemyLandIndex > playerLandIndex)
				 isEnemy2Steps = true;
			 if(isEnemy2Steps)
			 {
				 nextEnemyLandIndex++;
				 jumpAction = Sequence::create(MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2*2,90)),MoveBy::create(scrollingOverLandsDuration/2, Vec2(vt_Lands[0]->getContentSize().width/2*2,-90)),funcAfterJump,nullptr);
				 jumpAction_Up = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2*2,120)),MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1*2,-60)),funcAfterJump,nullptr);
				 jumpAction_Down = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(vt_Lands[0]->getContentSize().width/3*1*2,60)),MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(vt_Lands[0]->getContentSize().width/3*2*2,-120)),funcAfterJump,nullptr);

			 }
			 if(checkIsLandAtAboveLine(getLandOnScreen(nextEnemyLandIndex)) && getLandOnScreen(nextEnemyLandIndex)->getTag() != kTagNoneLand) //Neu land tiep theo nam o tren
			 {
				 if(!checkIsLandAtAboveLine(getLandOnScreen(enemyLandIndex))) //Neu land hien tai o duoi
					jumpAction = jumpAction_Up; //Nhay len
			 }
			 else //Neu land tiep theo nam o duoi
			 {
				 if(checkIsLandAtAboveLine(getLandOnScreen(enemyLandIndex)) && getLandOnScreen(nextEnemyLandIndex)->getTag() != kTagNoneLand) //Neu land hien tai o tren
					jumpAction = jumpAction_Down; //Nhay xuong
			 }
			 enemy->runAction(jumpAction);
			 //Animation
			enemy->runAction(animate_enemy);
			 if(isEnemy2Steps)
				 enemyLandIndex++;
			 enemyLandIndex++;
		}

	}








}
void GameScene::update(float dt)
{
	if (isGameOver || isPauseGame || isTuturialBeShowed)
		return;

	scrollBackground();
	moveEnemy(dt);
	scrollLand();
	countDownDangerous(dt);

}

void GameScene::countDownDangerous(float dt)
{
	if(!isDangerous)
		return;

	timerCountDown -= dt;
	if(timerCountDown <=0)
	{
		this->unschedule(schedule_selector(GameScene::countDownDangerous));
		isGameOver = true;

		//Player animation
		int frameAmount_player = 6;
		Vector<SpriteFrame*> animFrames_player(frameAmount_player);
		for (int i = 0; i < frameAmount_player; i++) {
			Rect rect = Rect(0, 0, 207, 152);
			auto frame = SpriteFrame::create(s_player_fall_sheet[i], rect);
			animFrames_player.pushBack(frame);
		}

		auto animation_player = Animation::createWithSpriteFrames(animFrames_player,0.03f);
		auto animate_player_fall = Animate::create(animation_player);
		auto func = CallFunc::create([=]() {
			player->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_player_fall_sheet[frameAmount_player-1]));
			showGameoverDialog();
							});
		player->runAction(Sequence::create(animate_player_fall,func,nullptr));
	}
}



void GameScene::addLand()
{

	//Real lands for Game Play
	for (int i = 0; i < 11; i++) {

		Sprite* land;
		if(i==playerLandIndex+2 || i==playerLandIndex+1 || i==0|| i==1)
		{
			land = Sprite::create(s_realLand);
			land->setTag(kTagRealLand);
		}
		else
		{
			int rand = CppUtils::randomBetween(1,4);
			if((rand == 1 || rand == 2) && vt_Lands[i-1]->getTag() != kTagNoneLand)
			{
				land = Sprite::create(s_realLand_none);
				land->setTag(kTagNoneLand);
			}
			else if(rand == 3)
			{
				land = Sprite::create(s_realLand_dangerous);
				land->setTag(kTagDangerousLand);
			}
			else
			{
				land = Sprite::create(s_realLand);
				land->setTag(kTagRealLand);
			}
		}
		land->setAnchorPoint(Vec2::ZERO);
		float posY = winSize.height*0.08+60;
		if(CppUtils::randomBetween(1,2) == 1)
			posY = winSize.height*0.08;
		land->setPosition(Vec2(land->getContentSize().width * i,posY));
		this->addChild(land);
		vt_Lands.push_back(land);
	}


}
void GameScene::scrollLand()
{
	if(isLandMoved)
		return;

	float scrollVelocity;
	 if(isPlayer2Steps)
	    	scrollVelocity = gameVelocity*2;
	 else
		 scrollVelocity = gameVelocity;
		for(int i=0; i < vt_Lands.size() ; i++)//for(Sprite* land:vt_Lands)
		{
			auto land = vt_Lands[i];
			float x1 = land->getPosition().x;
			float y1 = land->getPosition().y;
			x1 += -scrollVelocity;
			y1 += 0;
			land->setPosition(x1, y1);
			if(land->getPositionX() <= -land->getContentSize().width) //Neu land di het man hinh
			{
				if (landToRemoveChild != NULL) {
					landToRemoveChild->removeChildByTag(kTagBonus); //Remove bonus
					landToRemoveChild = NULL;
				}
				land->setPositionX(land->getContentSize().width * (vt_Lands.size()-1)); //Set pos ra cuoi'

				//Set gameLogic
				if(!isPlayer2Steps)
				{

					if(getLandOnScreen(playerLandIndex)->getTag() == kTagDangerousLand)
					{
						timerCountDown = 1.0f *3/enemyVelocity;
						if(timerCountDown <= 0.35)
							timerCountDown = 0.35;
						isDangerous = true;
					}
					else if(getLandOnScreen(playerLandIndex)->getTag() == kTagNoneLand)
					{
						timerCountDown = 0;
						isDangerous = true;
					}
					isLandMoved=true;
					enemyAppearTimer=0;
					bonusLandIndex--;
				}
				else
				{
					bonusLandIndex--;
					numberOfPassedLand++;
					if(numberOfPassedLand >=2)
					{

						if(getLandOnScreen(playerLandIndex)->getTag() == kTagDangerousLand)
						{
							if(timerCountDown <= 0.35)
								timerCountDown = 0.35;
							isDangerous = true;
						}
						else if(getLandOnScreen(playerLandIndex)->getTag() == kTagNoneLand)
						{
							timerCountDown = 0;
							isDangerous = true;
						}
						isLandMoved=true;
						enemyAppearTimer=0;
						numberOfPassedLand=0;
					}
				}

				//Scroll land
				if (bonusLandIndex == playerLandIndex && numberOfPassedLand == 0) {
								score += 9;

								if (isSound) {
									CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
											s_click);
								}
								bonusLandIndex = -1;
								getLandOnScreen(playerLandIndex)->removeChildByTag(kTagBonus,true);
							}
				if (bonusLandIndex == 0) {
					landToRemoveChild = getLandOnScreen(bonusLandIndex);
				}
				int rand = CppUtils::randomBetween(1,4); //random lai thanh land khac'
				if((rand == 1 || rand == 2) && getLandOnScreen(9)->getTag() != kTagNoneLand)
				{
					land->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_realLand_none));
					land->setTag(kTagNoneLand);
				}
				else if(rand == 3)
				{
					land->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_realLand_dangerous));
					land->setTag(kTagDangerousLand);
					if (score > 10 && bonusLandIndex < 0
										&& CppUtils::randomBetween(1, 20) == 11) {
									auto bonus = Sprite::create(s_bonus);
									bonus->setPosition(land->getContentSize().width / 2,
											land->getContentSize().height);
									bonus->setTag(kTagBonus);
									land->addChild(bonus);
									bonusLandIndex = 10;
								}
				}
				else
				{
					land->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_realLand));
					land->setTag(kTagRealLand);
					if (score > 10 && bonusLandIndex < 0
										&& CppUtils::randomBetween(1, 20) == 11) {
									auto bonus = Sprite::create(s_bonus);
									bonus->setPosition(land->getContentSize().width / 2,
											land->getContentSize().height);
									bonus->setTag(kTagBonus);
									land->addChild(bonus);
									bonusLandIndex = 10;
								}
				}





			}

		}
}
Sprite* GameScene::getLandOnScreen(int index)
{
	for(Sprite* land:vt_Lands)
		{
			cocos2d::Rect rectLand = land->getBoundingBox();
			rectLand.setRect(rectLand.origin.x, origin.y,rectLand.size.width, winSize.height); //Chi can truc X thoa dieu kien
			if (rectLand.containsPoint(Point(land->getContentSize().width*(index+0.5f),land->getPositionY()))) {
				return land;
			}
		}
		return NULL;
}
bool GameScene::checkIsLandAtAboveLine(Sprite* land)
{
	if(land->getPositionY() >= winSize.height*0.08+60)
		return true;
	return false;
}
bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
 
	//Remove tutorial if showed
	if(isTuturialBeShowed)
	{
		  auto func = CallFunc::create([=]() {
									tutorialLayer->removeChildWithEffect();
									numberOfTutorialContent--;
					  });

		  auto func2 = CallFunc::create([=]() {

					if(numberOfTutorialContent <= 0)
					{
						tutorialLayer->removeChildWithEffect();
						UserDefault::getInstance()->setBoolForKey("isFirstUse",false);
						isTuturialBeShowed = false;
					}
					else if(numberOfTutorialContent == 1)
					{
						auto tutorialString = "Be careful! Not stand in\nthe leaf so long or you will die.";
						int fontSize = 65;
						#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
							fontSize = 35;
						#endif
						tutorialLayer->addDialog(tutorialString, winSize.width/2,winSize.height/2, fontSize,Color3B(185, 129, 72));
					}


		  });
		this->runAction(Sequence::create(func,DelayTime::create(1),func2,nullptr));
		return true;
	}

    if(isPlayerRunningAction || isGameOver || !isLandMoved)
        return true;

    if(touch->getLocation().x <= origin.x + winSize.width/2)
    	isPlayer2Steps = false;
    else
    	isPlayer2Steps = true;






    auto funcAfterJump =
     CallFunc::create(
                      [=]() {
                          isPlayerRunningAction = false;
                          isStartedPlaying = true;

                          //Update score
						score++;
						labelScore->setString(CppUtils::doubleToString(score));
						if(enemyVelocity<=16 && score%10 == 9)
						enemyVelocity++;




                          //Wave animation
                          auto landToRunAction = getLandOnScreen(playerLandIndex);
                          if(landToRunAction != NULL && landToRunAction->getTag()!=kTagNoneLand)
                          {
                              Sprite* wave_land_sprite = Sprite::create(s_wave_land_sheet[0]);
                              wave_land_sprite->setPosition(landToRunAction->getContentSize().width/2,landToRunAction->getContentSize().height*0.2);
                              wave_land_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                              landToRunAction->addChild(wave_land_sprite);
                              
                              int frameAmount_waveLand = sizeof s_wave_land_sheet / sizeof s_wave_land_sheet[0];
                              Vector<SpriteFrame*> animFrames_waveLand(frameAmount_waveLand);
                              for (int i = 0; i < frameAmount_waveLand; i++) {
                                  Rect rect = Rect(0, 0, 209, 55);
                                  auto frame = SpriteFrame::create(s_wave_land_sheet[i], rect);
                                  animFrames_waveLand.pushBack(frame);
                              }
                              auto animation_waveLand = Animation::createWithSpriteFrames(animFrames_waveLand, 0.12f);
                              auto animate_waveLand = Animate::create(animation_waveLand);
                              auto func = CallFunc::create([=]() {
                                  landToRunAction->removeChild(wave_land_sprite,false);
                              });
                              auto seq = Sequence::create(animate_waveLand,func,nullptr);
                              wave_land_sprite->runAction(seq);
                          }
                      });
     float velocityGameBySecond = gameVelocity / animationInterval; //60 frames per second
     float scrollingOverLandsDuration = vt_Lands[0]->getContentSize().width / velocityGameBySecond;
     auto jumpAction = Sequence::create(MoveBy::create(scrollingOverLandsDuration/2, Vec2(0,90)),MoveBy::create(scrollingOverLandsDuration/2, Vec2(0,-90)),funcAfterJump,nullptr);
     auto jumpAction_Up = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(0,120)),MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(0,-60)),funcAfterJump,nullptr);
     auto jumpAction_Down = Sequence::create(MoveBy::create(scrollingOverLandsDuration/3*1, Vec2(0,60)),MoveBy::create(scrollingOverLandsDuration/3*2, Vec2(0,-120)),funcAfterJump,nullptr);
     int nextPlayerLandIndex = playerLandIndex+1;
     if(isPlayer2Steps)
    	 nextPlayerLandIndex++;
	 if(checkIsLandAtAboveLine(getLandOnScreen(nextPlayerLandIndex)) && getLandOnScreen(nextPlayerLandIndex)->getTag() != kTagNoneLand) //Neu land tiep theo nam o tren
	 {
		 if(!checkIsLandAtAboveLine(getLandOnScreen(playerLandIndex))) //Neu land hien tai o duoi
			jumpAction = jumpAction_Up; //Nhay len
	 }
	 else //Neu land tiep theo nam o duoi
	 {
		 if(checkIsLandAtAboveLine(getLandOnScreen(playerLandIndex)) && getLandOnScreen(nextPlayerLandIndex)->getTag() != kTagNoneLand) //Neu land hien tai o tren
			jumpAction = jumpAction_Down; //Nhay xuong
	 }
     player->runAction(jumpAction);

 	//Animation
     isLandMoved=false;
     isDangerous=false;

     player->stopAllActionsByTag(kTagPlayerAnimate);
     player->runAction(animate_player);
     isPlayerRunningAction=true;

     if(enemyLandIndex >= 0)
		 enemyLandIndex--; //player nhay 1 buoc -> enemy lui 1 buoc
	 if(isPlayer2Steps && enemyLandIndex >= 0)
		 enemyLandIndex--;

	 return true;
}
void GameScene::addEyesToBackground(Sprite* background)
{
	    int posX=0;
		int posY=0;
		for(int i=0; i < 8 ; i++)
		{
			Sprite* eyes1;
			if(CppUtils::randomBetween(1,2) == 1)
				eyes1= Sprite::create(s_eyes1);
			else
				eyes1= Sprite::create(s_eyes2);

			int distanceX = CppUtils::randomBetween(background->getContentSize().width*0.1, background->getContentSize().width*0.15);
			posX+=distanceX;
			posY = CppUtils::randomBetween(background->getContentSize().height*0.35, background->getContentSize().height*0.95);
			if(posX >= background->getContentSize().width - eyes1->getContentSize().width)
				posX = background->getContentSize().width - eyes1->getContentSize().width;
			eyes1->setPosition(posX,posY);
			background->addChild(eyes1);

			//Blink;
			auto hide = Hide::create();
			auto show = Show::create();
			auto delayHide = DelayTime::create(CCRANDOM_0_1()*2+0.5f);
			auto blinkSequence = Sequence::create(DelayTime::create(0.19f),hide,delayHide,show,nullptr);
			eyes1->runAction(RepeatForever::create(blinkSequence));
		}


}
void GameScene::scrollBackground()
{
	if(isLandMoved)
			return;

	float scrollVelocity;
	 if(isPlayer2Steps)
	    	scrollVelocity = gameVelocity*2;
	 else
		 scrollVelocity = gameVelocity;
		float x1 = background->getPosition().x;
		float y1 = background->getPosition().y;
		x1 += -scrollVelocity;
		y1 += 0;
		background->setPosition(x1, y1);
		if(background->getPositionX() <= -background->getContentSize().width)
		{
				background->setPositionX(background->getContentSize().width);
				Vector<Node*> childs = background->getChildren();
									for (Node* child : childs) {
										if (child != NULL && child->getTag() != kTagDecorating) {
											Sprite *eyes = (Sprite *) child;
											float posY = CppUtils::randomBetween(background->getContentSize().height*0.5, background->getContentSize().height*0.9);
											eyes->setPositionY(posY);

											if(CppUtils::randomBetween(1,2) == 1)
												eyes->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_eyes1));
											else
												eyes->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_eyes2));

											//Blink;
											auto hide = Hide::create();
											auto show = Show::create();
											auto delayHide = DelayTime::create(CCRANDOM_0_1()*2+0.5f);
											auto blinkSequence = Sequence::create(DelayTime::create(0.19f),hide,delayHide,show,nullptr);
											eyes->stopAllActions();
											eyes->runAction(RepeatForever::create(blinkSequence));
										}
									}

		}

		float x2 = background2->getPosition().x;
		float y2 = background2->getPosition().y;
		x2 += -scrollVelocity;
		y2 += 0;
		background2->setPosition(x2, y2);
		if(background2->getPositionX() <= -background2->getContentSize().width)
		{
				background2->setPositionX(background2->getContentSize().width);
				Vector<Node*> childs = background2->getChildren();
					for (Node* child : childs) {
						if (child != NULL && child->getTag() != kTagDecorating) {
							Sprite *eyes = (Sprite *) child;
							float posY = CppUtils::randomBetween(background->getContentSize().height*0.5, background->getContentSize().height*0.9);
							eyes->setPositionY(posY);

							if(CppUtils::randomBetween(1,2) == 1)
								eyes->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_eyes1));
							else
								eyes->setTexture(CCTextureCache::sharedTextureCache()->addImage(s_eyes2));

							//Blink;
							auto hide = Hide::create();
							auto show = Show::create();
							auto delayHide = DelayTime::create(CCRANDOM_0_1()*2+0.5f);
							auto blinkSequence = Sequence::create(DelayTime::create(0.19f),hide,delayHide,show,nullptr);
							eyes->stopAllActions();
							eyes->runAction(RepeatForever::create(blinkSequence));
						}
					}
		}



}





void GameScene::postScoreToServer(int score)
{
	if (FacebookAgent::getInstance()->getAccessToken() == "")
	{
		isRankingTouchable = true;
		return;
	}

		vector < string > h;
		h.push_back(
				"X-Parse-Application-Id: hYf8HYrrrvZTdUligYAdy5lKtqrzHg7i8XMR8Jig");
		h.push_back(
				"X-Parse-REST-API-Key: bVFb1yUe4n6NX4oSzW9UCy1FqwR1HsMjnA5EfGqs");
		h.push_back("Content-Type: application/json");

		auto r2 = new HttpRequest();

		std::string myObjectID= UserDefault::getInstance()->getStringForKey("MY_OBJECT_ID");
		char url[256];
		sprintf(url, "https://api.parse.com/1/classes/RedHood/%s",myObjectID.c_str());
		const char* urlOK = url;

		r2->setUrl(urlOK);

		r2->setHeaders(h);

		r2->setRequestType(HttpRequest::Type::PUT);


		char data[5500];
		sprintf(data, "{\"Score\":%d}",score);
		const char* buffer = data;
		r2->setRequestData(buffer, strlen(buffer));

		r2->setResponseCallback(this, httpresponse_selector(GameScene::postScoreToServerCallBack));

		HttpClient::getInstance()->send(r2);

		//Close connection
		r2->release();
}
void GameScene::postScoreToServerCallBack(HttpClient* client,
		HttpResponse* response) {
	if (response->isSucceed()) {
			log("Success");

		} else {
			log("Failed");
		}
	isRankingTouchable = true;
}
