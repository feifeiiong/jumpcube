#include "GameScene.h"
#include "StatusLayer.h"


GameScene::GameScene(){}
GameScene::~GameScene(){}
bool GameScene::init(){
	// why here initWithPhysics()
	if (Scene::initWithPhysics()){
		//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		this->getPhysicsWorld()->setGravity(Vect(0,GRAVITY));
		// Add the background
		auto backgroundLayer = BackgroundLayer::create();
		if (backgroundLayer){
			this->addChild(backgroundLayer);
		}
		auto statusLayer = StatusLayer::create();
		auto gameLayer = GameLayer::create();
		if (gameLayer){
			gameLayer->setPhyWorld(this->getPhysicsWorld());
			gameLayer->setDelegator(statusLayer);
			this->addChild(gameLayer);
		}
		//add the game status layer to show the score and the game status
		if (statusLayer){
			this->addChild(statusLayer);
		}
		//	add operationLayer to control the game
		auto optionLayer = OptionLayer::create();
		if (optionLayer){
			optionLayer->setDelegator(gameLayer);
			this->addChild(optionLayer);
		}
		return true;
	}else {
		return false;
	}
}
void GameScene::restart() {
	this->removeAllChildrenWithCleanup(true);
	this->init();
}