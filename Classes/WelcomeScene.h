#pragma once
#include "PictureLoader.h"
#include "WelcomeLayer.h"
#include "BackgroundLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class WelcomeScene :public Scene {
public:
	WelcomeScene();
	~WelcomeScene();
	bool init();
	CREATE_FUNC(WelcomeScene);
};