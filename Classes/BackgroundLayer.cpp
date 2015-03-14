#include "BackgroundLayer.h"
#include "PictureLoader.h"
BackgroundLayer::BackgroundLayer(){}
BackgroundLayer::~BackgroundLayer(){}
bool BackgroundLayer::init() {
	if (!Layer::init()){
		return false;
	}
	// get the current time,the background will change according to the current time
	time_t t =  time(nullptr);
	tm* lt = localtime(&t);
	int hour = lt->tm_hour;
	Sprite *background ;
	background = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_bg"));
	//background->setRotation(-90);
	/*if(hour >= 6 && hour <= 17){
	background = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("bg_day"));
	}else{
	background = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("bg_night"));
	}*/
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	addChild(background);
	return true;
}
float BackgroundLayer::getLandHeight(){
	// where u should change
	return
		Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("white_rec"))->getContentSize().height;
}