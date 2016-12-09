#include "AppDelegate.h"
#include "SplashScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#include "PluginAdMob/PluginAdMob.h"
#include "PluginChartboost/PluginChartboost.h"
#include "PluginIAP/PluginIAP.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginVungle/PluginVungle.h"
#endif
#endif

USING_NS_CC;
static cocos2d::Size resolutionSize = cocos2d::Size(1200, 1920);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs() {
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
#ifdef SDKBOX_ENABLED
	sdkbox::PluginFacebook::init();
	sdkbox::PluginAdMob::init();
	sdkbox::PluginChartboost::init();
	sdkbox::IAP::init();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sdkbox::PluginVungle::init();
#endif
#endif
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("MyGalleryGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("MyGalleryGame");
#endif
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(resolutionSize.width, resolutionSize.height,
			ResolutionPolicy::EXACT_FIT);

	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// create a scene. it's an autorelease object
	auto scene = SplashScene::scene();

	// run
	director->runWithScene(scene);

	FileUtils *fileUtils = FileUtils::getInstance();
	std::vector < std::string > searchPaths = fileUtils->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), "res");
	fileUtils->setSearchPaths(searchPaths);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be paused
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
