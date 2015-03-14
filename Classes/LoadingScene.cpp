#include "LoadingScene.h"
extern const char* GAME_ATLAS_PIC;
extern const char* GAME_ATLAS_TXT;
//LoadingScene::LoadingScene() {}
//LoadingScene::~LoadingScene() {}
bool LoadingScene::init(){
	if (Scene::init()){
		return true;
	}else{
		return false;
	}
}
void LoadingScene::onEnter() {
	// add background to current scene
	Sprite *background = Sprite::create(GAME_BACKGROUND);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point orign = Director::getInstance()->getVisibleOrigin();
	background->setPosition(orign.x+visibleSize.width/2,orign.y+visibleSize.height/2);
	this->addChild(background);
	// start async method load the atlas.png
	Director::getInstance()->getTextureCache()->addImageAsync(GAME_ATLAS_PIC,CC_CALLBACK_1(LoadingScene::loadingCallBack,this));

}
void LoadingScene::loadingCallBack(Texture2D *texture) {
	PictureLoader::getInstance()->loadAtlas(GAME_ATLAS_TXT,texture);
	// After loading the texture,preload all the sound
	SimpleAudioEngine::getInstance()->preloadEffect("die.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_point.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_swooshing.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("jump.wav");
	// after load all the things, change the scene to a new one
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1,scene);
	Director::getInstance()->replaceScene(transition);

}