#pragma once
#include "cocos2d.h"
#include "time.h"
#include "PictureLoader.h"
using namespace cocos2d;
using namespace std;
class BackgroundLayer :public Layer {
public:
	BackgroundLayer();
	~BackgroundLayer();
	bool init();
	CREATE_FUNC(BackgroundLayer);
	static float getLandHeight();
};