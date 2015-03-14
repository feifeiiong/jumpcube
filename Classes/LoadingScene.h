#pragma once
#include "cocos2d.h"
#include "PictureLoader.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "WelcomeScene.h"
#include "BackgroundLayer.h"
#include "CCPlatformMacros.h"


using namespace cocos2d;
using namespace CocosDenshion;
const string GAME_BACKGROUND = "splash.png";

class LoadingScene:public Scene {
public:
	//LoadingScene(){}
	//~LoadingScene(){}
	/**
     *  Initializes the instance of PictureLoader.
     *
     *  @note When you are porting Cocos2d-x to a new platform, you may need to take care of this method.
     *
     *  @return true if successed, otherwise it returns false.
     *
     */
	virtual bool init();
	CREATE_FUNC(LoadingScene);
	/**
     * When this scene appear ,will call this method,
     * not this scene is create, so if you have some thing
     * want do when scene appear not init, please add to here
     */
	void onEnter();
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using PictureLoader
     */
	void loadingCallBack(Texture2D *texture);
};