#include "BirdSprite.h"
BirdSprite::BirdSprite() {}
BirdSprite::~BirdSprite() {}
BirdSprite* BirdSprite::shareBirdSprite =nullptr;
BirdSprite* BirdSprite::getInstance() {
	if (shareBirdSprite==nullptr)
	{
		shareBirdSprite = new BirdSprite();
		if (!shareBirdSprite->init())
		{
			delete shareBirdSprite;
			// 删除指针指向对象之后需要把指针赋值为空 很奇怪为什么这个操作没有封装在delete里
			shareBirdSprite=nullptr;
			CCLOG("error init");
		}
	}
	return shareBirdSprite;
}
bool BirdSprite::init() {
	/**
	* 这个数据成员是搞毛
	*/
	this->isFirstTime = 3;
	return true;
}

bool BirdSprite::createBird() {
	this->createBirdByRandom();
	if (Sprite::initWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName(this->birdName)))
	{
		// can not understand
		/*Animation *animation = this->createAnimation(this->birdNameFormat.c_str(), 3, 10);
		Animate * animate = Animate::create(animation);
		this->idleAction = RepeatForever::create(animate);*/
		// create the swing action
		ActionInterval *up = CCMoveBy::create(0.4f,Point(0, 8));
		ActionInterval *upBack= up->reverse();
		this->swingAction = RepeatForever::create(Sequence::create(up, upBack, NULL));
		return true;
	}else {
		return false;
	}
}
bool BirdSprite::changeState(ActionState state) {
	this->currentState = state;
	return true;
}
void BirdSprite::idle(){
	if (changeState(ACTION_STATE_IDLE)) {
		//this->runAction(this->idleAction);
		//this->runAction(this->swingAction);
	}

}
void BirdSprite::fly() {
	if (changeState(ACTION_STATE_FLY)){
		this->stopAction(swingAction);
		this->getPhysicsBody()->setGravityEnable(true);
	}
}
void BirdSprite::die() {
	if (changeState(ACTION_STATE_DIE)){
		this->stopAllActions();
	}
}
void BirdSprite::createBirdByRandom(){
	if (this->isFirstTime&1){
		this->isFirstTime&=2;
	}else if (this->isFirstTime&2){
		this->isFirstTime&=1;
		return;
	}
	srand((unsigned)time((NULL)));
	int type = ((int)rand())%3;
	switch (type)
	{
	case 0:
		this->birdName = "whitebird";
		//this->birdNameFormat = "bird0_%d";
		break;
	case 1:
		this->birdName = "whitebird";
		//this->birdNameFormat = "bird1_%d";
		break;
	case 2:
		this->birdName = "whitebird";
		//this->birdNameFormat = "bird2_%d";
		break;
	default:
		this->birdName = "whitebird";
		//this->birdNameFormat = "bird2_%d";
		break;
	}

}
Animation* BirdSprite::createAnimation(const char *fmt, int count, float fps) {
	Animation *animation = Animation::create();
	animation->setDelayPerUnit(1/fps);
	for (int i = 0;i<count;i++){
		const char * filename = String::createWithFormat(fmt,i)->getCString();
		SpriteFrame *frame = PictureLoader::getInstance()->getSpriteFrameByName(filename);
		animation->addSpriteFrame(frame);
	}
	return animation;
}
