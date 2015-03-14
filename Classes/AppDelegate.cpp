#include "AppDelegate.h"
const char* GAME_ATLAS_PIC = "flappyrec.png";
const char* GAME_ATLAS_TXT = "flappyrec.plist";
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	
	if(!glview) {
		glview = GLView::create("cocos flappy");
		director->setOpenGLView(glview);
		
	}
	this->setResourceSearchResolution();
	// turn on display FPS
	director->setDisplayStats(false);
	// size of the screen
	auto screenSize = glview->getFrameSize();
	auto designSize = Size(960,640);
	float widthRate = screenSize.width/designSize.width;
	float heightRate = screenSize.height/designSize.height;
	if (widthRate>heightRate){
		glview->setDesignResolutionSize(designSize.width,designSize.height*heightRate/widthRate,ResolutionPolicy::NO_BORDER);
	}else {
		glview->setDesignResolutionSize(designSize.width*widthRate/heightRate,designSize.height,ResolutionPolicy::NO_BORDER);
	}
	
	
	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	auto scene = LoadingScene::create();

	// run
	director->runWithScene(scene);

	return true;
}
void AppDelegate::setResourceSearchResolution()
{
	std::vector<std::string> paths;
	paths.push_back("fonts");
	paths.push_back("image");
	paths.push_back("sounds");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
}
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
