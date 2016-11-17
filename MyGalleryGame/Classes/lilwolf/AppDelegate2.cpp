#include "AppDelegate.h"
#include "SplashScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "Resources.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
	Size designSize = Size(1280, 800);
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(designSize.width,designSize.height,ResolutionPolicy::EXACT_FIT);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // create a scene. it's an autorelease object
    Scene* scene;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    scene = MenuScene::scene();
#endif
#if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    scene = SplashScene::scene();
#endif

    // run
    director->runWithScene(scene);

    FileUtils *fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), "res");
    fileUtils->setSearchPaths(searchPaths);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    GameScene* scene = dynamic_cast <GameScene*>(Director::getInstance()->getRunningScene()->getChildByTag(kTagLayerHavingPauseDialog));
       if (scene != NULL)
       	scene->showPauseDialog();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
