#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "TutorialLayer.h"

using namespace cocos2d::network;
using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

class GameScene: public cocos2d::Layer {
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    static cocos2d::Scene* scene();
    
    CREATE_FUNC(GameScene)
    
    void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

    bool onTouchBegan(Touch* touch, Event* event);
    
    
    bool isSound;
    bool isPlayerRunningAction;
    bool isLandMoved;
    bool isPlayer2Steps;
    bool isGameOver;
    bool isDangerous;
    bool isPauseGame;
	int numberOfPassedLand;
    float gameVelocity;

    Size winSize;
    Point origin;
    int score;
    
    //Game widget
    void pauseGame(Ref* pSender, ui::Widget::TouchEventType eEventType);
    Label* labelScore;

    Sprite* player;
    int playerLandIndex;
    Sprite* getLandOnScreen(int index);
    bool checkIsLandAtAboveLine(Sprite* land);
    Animate* animate_player;
    
    //Bonus
    int bonusLandIndex;

    void scrollBackground();
    Sprite* background2;
    Sprite* background;
    void addEyesToBackground(Sprite* background);
    void addWavesToBackground(Sprite* background);

    void addLand();
    vector<Sprite*> vt_Lands;
    void scrollLand();
    Sprite* landToRemoveChild;

    void update(float dt);
    void countDownDangerous(float dt);
    float timerCountDown;

    Sprite* enemy;
    void moveEnemy(float dt);
    float enemyAppearTimer;
    float enemyJumpTimer;
    bool isStartedPlaying;
    bool isEnemy2Steps;
    int enemyLandIndex;
    Animate* animate_enemy;
    float enemyVelocity;
    bool isEnemyJumping;
    bool isEnemyApearing;
    float animationInterval;

    //Gameover & pause dialog
	LayerColor* layerTrum;

	void showGameoverDialog();
	Layer* gameoverDialogLayer;
	void showPauseDialog();
	void continutePlay();
	Layer* pauseDialogLayer;




	//Post score onto server
    bool isRankingTouchable;
	void postScoreToServer(int score);
	void postScoreToServerCallBack(HttpClient* client, HttpResponse* response);

	//Tutorial
		void showTutorial();
		TutorialLayer* tutorialLayer;
		int numberOfTutorialContent;
		bool isTuturialBeShowed;
};

#endif // __GAME_SCENE_H__
