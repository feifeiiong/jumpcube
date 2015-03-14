#include "WelcomeScene.h"
#include "CCPlatformMacros.h"

WelcomeScene::WelcomeScene(){}
WelcomeScene::~WelcomeScene(){}
bool WelcomeScene::init() {
	bool bRec = false;
	do {
		CC_BREAK_IF(!Scene::init());
		auto welcomeLayer = WelcomeLayer::create();
		CC_BREAK_IF(!welcomeLayer);
		this->addChild(welcomeLayer);
		bRec = true;
	} while (0);
	return bRec;

}