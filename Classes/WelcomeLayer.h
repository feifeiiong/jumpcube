#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CCMenuItem.h"
#include "GameScene.h"
#include "PictureLoader.h"
#include "time.h"
#include "BirdSprite.h"
#include "CCPlatformMacros.h"
//#include "Cocos2dx/Common/CCUMSocialSDK.h"
//#include "Cocos2dx/ShareButton/UMShareButton.h"
////  π”√”—√À√¸¡Óø’º‰
//USING_NS_UM_SOCIAL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif

class WelcomeLayer:public Layer {
public:
	static bool isMusicOpen;
	WelcomeLayer(void);
	~WelcomeLayer(void);
	bool init();

	

	

	CREATE_FUNC(WelcomeLayer);
private:
	/**
	* The start button has been pressed will call this function
	*/
	void menuStartCallback(Ref *sender);
	/**
	* this method is to make the land have a scroll animation
	*/
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void scrollLand(float dt);
	void menuAudioCallback(Ref* sender);
	void createAudioButton();
	void createRecs();
	void initLand();
	void createRankButton();
	void menuRankCallback(Ref * sender);
	void moveFunc();
	void showNewScene();
	void createTitle();
	void createShareButton();
	void createPlayButton();
	void createReturnButton(); 
	void createScoreSprite();
	void menuReturnCallback(Ref* sender);
	void returnFunc();
	void menuQuitCallback(Ref* sender);
	vector<Node *> recs;
	Menu* audioMenu;
	int recCount;
	Menu* rankMenu;
	Sprite* title;
	Sprite* scoreSprite;
	Menu* playMenu;
	Menu* returnMenu;
	Menu* shareMenu;
	Menu* quitMenu;
};