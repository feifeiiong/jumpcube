#pragma once
#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "OptionLayer.h"
using namespace cocos2d;
const float GRAVITY = -2600.0f;
class GameScene :public Scene {
public:
	GameScene();
	~GameScene();
	bool init();
	void restart();
	CREATE_FUNC(GameScene);
};
