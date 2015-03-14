#include "GameLayer.h"
#include "ccTypes.h"
#include "UserRecord.h"
#include "WelcomeLayer.h"
int GameLayer::boundCount = 0;
int GameLayer::readyRecCount = 0;
void GameLayer::initLand() {
	createRecs();
}

GameLayer::GameLayer(){}
GameLayer::~GameLayer(){};

bool GameLayer::init(){
	if (Layer::init()){
		this->boundCount=0;
		this->readyRecCount= 1;

		//get the origin point of the X-Y axis, and the visible size of the screen
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point orign = Director::getInstance()->getVisibleOrigin();
		auto rec = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("white_rec"));
		auto boundRec = rec->getBoundingBox();
		int boundwidth = boundRec.size.width;
		this->recCount = (visibleSize.width/boundRec.size.width)+2;
		while(recCount<=MAX_REC_READY) {
			recCount++;
		}
		score = 0;
		// ADD THE BIRD
		// when layer init, init bird's body
		bird = BirdSprite::getInstance();
		bird->createBird();
		this->bird->setOpacity(255);
		// set the bird color
		bird->setColor(this->getRandomColor());
		PhysicsBody *body = PhysicsBody::create();
		auto recBox = PhysicsShapeBox::create(bird->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT,Point::ZERO);
		recBox->setFriction(0);
		recBox->setRestitution(0);
		body->addShape(recBox);
		body->setDynamic(true);
		body->setLinearDamping(0.0f);
		body->setGravityEnable(false);
		body->setVelocity(Vect(0,BIRD_V));
		body->setVelocityLimit(BIRD_V);
		body->setMass(1.0f);
		body->setContactTestBitmask(0xFFFFFFFF);
		bird->setPhysicsBody(body);

		bird->setPosition(orign.x+visibleSize.width/3-5,rec->getContentSize().height/2+REC_HEIGHT-5);
		bird->idle();

		addChild(bird,40);
		this->bird->fly();
		// add the ground
		this->groundNode = Node::create();
		float landHeight = BackgroundLayer::getLandHeight();
		auto groundBody = PhysicsBody::create();
		//PhysicsShapeBox and PhysicsShapeCircle
		// where u need to change
		groundBody->addShape(PhysicsShapeBox::create(Size(288,landHeight)));
		groundBody->setDynamic(false);
		groundBody->setLinearDamping(0.0f);
		groundBody->setMass(1.0f);
		groundBody->setContactTestBitmask(0xFFFFFFFF);
		this->groundNode->setPhysicsBody(groundBody);
		this->groundNode->setPosition(144,landHeight/2);
		initLand();
		//addChild(groundNode);
		this->shiftLand = schedule_selector(GameLayer::scrollLand);
		// schedule method is the layer's method
		this->schedule(shiftLand,LAND_V);
		this->scheduleUpdate();
		// 碰撞事件监听
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactSeperate = CC_CALLBACK_1(GameLayer::onContactSeperate,this);
		contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin,this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);
		this->gameStatus =  GAME_STATUS_START;

		return true;
	}else {
		return false;
	}
}
void GameLayer::onContactSeperate(const PhysicsContact& contact) {
	if (this->readyRecCount>=MAX_REC_READY){
		auto a = contact.getShapeA()->getBody()->getNode();
		auto b = contact.getShapeB()->getBody()->getNode();
		Node* tempBirdNode ;
		Node* tempRecNode;
		if (a==this->bird)
		{
			tempBirdNode = a;
			tempRecNode = b;
		}else {
			tempBirdNode = b;
			tempRecNode = a;
		}
		Point birdV = tempBirdNode->getPhysicsBody()->getVelocity();
		tempBirdNode->getPhysicsBody()->setVelocity(Vect(0,birdV.y));
	}
}
bool GameLayer::onContactBegin(const PhysicsContact& contact) {

	this->delegator->onGameStart();

	//if (this->readyRecCount>=MAX_REC_READY){
	auto a = contact.getShapeA()->getBody()->getNode();
	auto b = contact.getShapeB()->getBody()->getNode();
	Node* tempBirdNode ;
	Node* tempRecNode;
	if (a==this->bird)
	{
		tempBirdNode = a;
		tempRecNode = b;
	}else {
		tempBirdNode = b;
		tempRecNode = a;
	}
	auto birdSize = tempBirdNode->getContentSize();
	auto recSize = tempRecNode->getContentSize();
	auto birdPoint = tempBirdNode->getPosition();
	auto recPoint = tempRecNode->getPosition();
	//if (((tempBirdNode->getPositionX()+birdSize.width/2)>=(tempRecNode->getPositionX()-recSize.width/2))
	//	&&(tempBirdNode->getPositionY())<=(tempRecNode->getPositionY()+tempRecNode->getContentSize().height/2)){
	//		tempBirdNode->getPhysicsBody()->setVelocity(Vect(-(tempRecNode->getContentSize().width/(1.25*FLY_TIME)+2),tempBirdNode->getPhysicsBody()->getVelocity().y));

	//		return false;
	//}
	/*tempBirdNode->getPhysicsBody()->setVelocity(Vect(0,tempBirdNode->getPhysicsBody()->getVelocity().y));*/
	auto recNodeBound = tempRecNode->getBoundingBox();
	auto birdNodeBound = tempBirdNode->getBoundingBox();
	// 1 above 2 x in y out 3 x out y in 4 x in  y in
	// above get point change color clear bounds 
	// 1 do nothing 2 give a same v 3 fix y 4 fix x and y
	// here needs a better fix
	// do first
	auto maxRecY = tempRecNode->getPositionY()+recSize.height/2;
	auto minRecY = tempRecNode->getPositionY()-recSize.height/2;
	auto maxRecX = tempRecNode->getPositionX()+recSize.width/2;
	auto minRecX = tempRecNode->getPositionX()-recSize.width/2;
	auto maxBirdY = tempBirdNode->getPositionY()+birdSize.height/2;
	auto minBirdY = tempBirdNode->getPositionY()-birdSize.height/2;
	auto maxBirdX = tempBirdNode->getPositionX()+birdSize.width/2;
	auto minBirdX = tempBirdNode->getPositionX()-birdSize.width/2;
	auto _maxBirdY = birdNodeBound.getMaxY();
	auto _minBirdY = birdNodeBound.getMinY();
	auto _maxBirdX = birdNodeBound.getMaxX();
	auto _minBirdX = birdNodeBound.getMinX();
	if (maxRecY+2<=minBirdY){
		onFirstSceneContact(tempRecNode,tempBirdNode);
		// do 3 after first contact 
		tempBirdNode->setPositionY(maxRecY+tempBirdNode->getContentSize().height/2);
		// reset the bound
		maxBirdY = tempBirdNode->getPositionY()+birdSize.height/2;
		minBirdY = tempBirdNode->getPositionY()-birdSize.height/2;
		/*	tempBirdNode->setPositionY(tempRecNode->getPositionY()+tempRecNode->getContentSize().height/2+REC_HEIGHT+13);*/
		return true;
	}

	// do second
	if ((maxBirdX>=minRecX)&&(maxRecY>=minBirdY+2))
	{

		// 2
		// first give a v then give a fix
		//auto deltaX = maxBirdX-minRecX;
		//tempBirdNode->setPositionX(minBirdX+birdSize.width/2-deltaX);
		maxBirdX = tempBirdNode->getPositionX()+birdSize.width/2;
		minBirdX = tempBirdNode->getPositionX()-birdSize.width/2;
		Point birdV = tempBirdNode->getPhysicsBody()->getVelocity();
		tempBirdNode->getPhysicsBody()->setVelocity(Vect(-(tempRecNode->getContentSize().width/(1.25*FLY_TIME)),birdV.y));


		return true;
	}
	// 4
	if (maxBirdX>=minRecX&&maxBirdY<=maxRecY+2)
	{
		/*auto deltaX = maxBirdX-minRecX;
		tempBirdNode->setPositionX(minBirdX+birdSize.width/2-deltaX);*/
		maxBirdX = tempBirdNode->getPositionX()+birdSize.width/2;
		minBirdX = tempBirdNode->getPositionX()-birdSize.width/2;
		Point birdV = tempBirdNode->getPhysicsBody()->getVelocity();
		tempBirdNode->getPhysicsBody()->setVelocity(Vect(-(tempRecNode->getContentSize().width/(1.25*FLY_TIME)),birdV.y));

		return true;
	}

	//if (birdNodeBound.intersectsRect(recNodeBound))
	//{
	//	if (birdNodeBound.getMinY()+tempBirdNode->getContentSize().height/2+2<recNodeBound.getMaxY())
	//	{
	//		Point birdV = tempBirdNode->getPhysicsBody()->getVelocity();
	//		tempBirdNode->getPhysicsBody()->setVelocity(Vect(-(tempRecNode->getContentSize().width/(1.25*FLY_TIME)),birdV.y));
	//		//tempBirdNode->setPositionX(rx-tempBirdNode->getContentSize().width/2);
	//		return false;
	//	}else {
	//		tempBirdNode->getPhysicsBody()->setVelocity(Vect(0,tempBirdNode->getPhysicsBody()->getVelocity().y));
	//	}


	//}
	//onFirstSceneContact(tempRecNode, tempBirdNode);


	//// fix the contacts
	//if (tempRecNode->getTag()==REC_PASS){
	//	tempBirdNode->setPositionY(tempRecNode->getPositionY()+tempRecNode->getContentSize().height/2+REC_HEIGHT+13);

	//}


	return true;
}
void GameLayer::onFirstSceneContact( Node* tempRecNode, Node* tempBirdNode ) 
{
	if (tempRecNode->getTag()!=REC_PASS){
		if ((tempRecNode->getPositionX()-tempRecNode->getContentSize().width/2)<(tempBirdNode->getPositionX()))
		{

			tempBirdNode->getPhysicsBody()->setVelocity(Vect(0,0));
			tempRecNode->setTag(REC_PASS);
			// add score
			if (this->readyRecCount>MAX_REC_READY){
				if (WelcomeLayer::isMusicOpen){

					SimpleAudioEngine::getInstance()->playEffect("sfx_point.wav");
				}

				this->score++;
				this->delegator->onGamePlaying(score);
				// change the color
				vector<Node*>::iterator iter= this->recs.begin();
				Color3B tempColor = this->getRandomColor();
				/*if(tempBirdNode->getPositionX()<(tempRecNode->getPositionX()-(tempRecNode->getContentSize().width/2))) 
				return true;*/
				//if(!((tempBirdNode->getPositionX()+tempBirdNode->getContentSize().width+2)>(tempRecNode->getPositionX()-tempRecNode->getContentSize().width/2)))
				//	return true;
				tempBirdNode->getPhysicsBody()->setVelocity(Vect(0,0));
				tempBirdNode->setColor(tempColor);
				tempRecNode->setColor(tempColor);

				// use iterator fix the color bug, but the code is not perfect now
				while (iter!=recs.end()){
					int random = rand()%2;
					if (*iter==tempRecNode){
						if ((iter+1)!=recs.end()){
							if (iter+2!=recs.end()){
								if (iter+3!=recs.end()){
									if (random==0){
										(*(iter+2))->setColor(tempColor);
									}
									else{
										(*(iter+3))->setColor(tempColor);
									}

								}
								else{
									if (random==0){
										(*(iter+2))->setColor(tempColor);
									}
									else{
										(*(recs.begin()))->setColor(tempColor);
									}
								}
							}else{
								if (random==0){
									(*(recs.begin()))->setColor(tempColor);
								}
								else{
									(*(recs.begin()+1))->setColor(tempColor);
								}
							}
						}

						else {
							if (random==0){
								(*(recs.begin()+1))->setColor(tempColor);
							}
							else{
								(*(recs.begin()+2))->setColor(tempColor);
							}
						}
						break;
					}	
					iter++;
				}

			}
		}
	}
	for (auto rec :this->recs){
		Color3B recC = rec->getColor();
		if (recC.equals(this->bird->getColor())){
			rec->getPhysicsBody()->setEnable(true);
		}else{
			rec->getPhysicsBody()->setEnable(false);
		}
	}
	boundCount = 0;
}
Color3B GameLayer::getRandomColor() {
	Color3B tempColor;
	int colorType = ((int)rand())%COLOR_COUNT;
	switch (colorType)
	{
	case 0 :
		tempColor = REC_LIGHTGRAY;
		break;
	case 1:
		tempColor = REC_LIGHTBLUE;
		break;
	case 2:
		tempColor = REC_LIGHTRED;
		break;
	case 3:
		tempColor = REC_LIGHTYELLOW;
		break;
	case 4:
		tempColor = REC_GREEN;
		break;
	case 5:
		tempColor = REC_GRAY;
		break;
	case 6:
		tempColor = REC_PINKRED;
		break;
	default:
		break;
	}
	return tempColor;
}
void GameLayer::scrollLand(float dt){

	for (auto rec : this->recs){
		if ((rec->getPositionX()-rec->getContentSize().width/2)<this->bird->getPositionX()&&rec->getTag()!=REC_PASS)
		{
			if (readyRecCount<MAX_REC_READY)
			{
				rec->setTag(REC_PASS);
			}

			readyRecCount++;
		}
		//int movDistance = ((3*rec->getContentSize().width/2)/(2*BIRD_V/-GRAVITY))*dt+0.5;
		//int movDistance = 4;
		float movDistance = (rec->getContentSize().width/(1.25*FLY_TIME))*dt;
		Color3B recC = rec->getColor();
		if (recC.equals(this->bird->getColor())){
			rec->getPhysicsBody()->setEnable(true);
		}else{
			rec->getPhysicsBody()->setEnable(false);
		}
		rec->setPositionX(rec->getPositionX()-movDistance);
		if (rec->getPositionX()<(-rec->getContentSize().width)){
			rec->setTag(REC_NEW);
			Point point = Director::getInstance()->getVisibleOrigin();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			float deta = (-rec->getPositionX()-rec->getContentSize().width);
			rec->setPositionX(point.x+(this->recCount-1)*rec->getContentSize().width-deta);
			rec->setPositionY(point.y+this->getRandomRecHeight());
			rec->setColor(this->getRandomColor());

		}
	}

}
void GameLayer::onTouch() {
	if (this->gameStatus ==  GAME_STATUS_OVER){
		return;
	}
	if (WelcomeLayer::isMusicOpen)
	{
		SimpleAudioEngine::getInstance()->playEffect("jump.wav");
	}

	if (this->gameStatus == GAME_STATUS_READY){


		this->gameStatus = GAME_STATUS_START;
		//this->createPips();
		//this->createRecs();
	}else if(gameStatus == GAME_STATUS_START) {
		this->bird->fly();
		boundCount++;
		if (boundCount<=REC_MAX_BOUNDS){
			this->bird->getPhysicsBody()->setVelocity(Vect(this->bird->getPhysicsBody()->getVelocity().x,BIRD_V));
			this->bird->runAction(RotateBy::create(1,360));
		}

	}
}
// 旋转小鸟 让它蹦跶
void GameLayer::rotateBird() {
	float verticalSpeed = this->bird->getPhysicsBody()->getVelocity().y;

	this->bird->setRotation(0);
	//this->bird->setRotation(min(max(-90, (verticalSpeed*0. + 60)), 30));
}
// 每一帧都做检查
void GameLayer::update(float delta) {
	this->rotateBird();
	this->checkHit();

}
void GameLayer::createRecs() {
	for (int i = 0;i<this->recCount;i++){
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point orign = Director::getInstance()->getVisibleOrigin();
		auto rec = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("white_rec"));

		rec->setPositionX(orign.x+i*(rec->getContentSize().width));
		if (i<MAX_REC_READY){
			rec->setPositionY(REC_HEIGHT);
		}else{
			rec->setPositionY(this->getRandomRecHeight());
		}
		if (i==this->recCount-1){
			rec->setPositionY(REC_HEIGHT+30);
		}
		rec->setAnchorPoint(Point(0,0.5));
		auto body = PhysicsBody::create();
		// offset need to be tested
		auto shapeBox = PhysicsShapeBox::create(rec->getContentSize(),PHYSICSSHAPE_MATERIAL_DEFAULT, Point(4,2));
		shapeBox->setFriction(0);
		shapeBox->setRestitution(0);
		body->addShape(shapeBox);
		body->setDynamic(false);
		body->setMass(1.0f);
		body->setContactTestBitmask(0xFFFFFFFF);
		// set color
		auto birdc = this->bird->getColor();
		if (i==MAX_REC_READY-1)
		{
			auto tempC = getRandomColor();
			while (tempC.equals(birdc))
			{
				tempC = getRandomColor();
			}
			rec->setColor(tempC);
		}else if(i<=MAX_REC_READY)
		{
			rec->setColor(birdc);
		}else
		{
			rec->setColor(getRandomColor());
		}
		
		
		if (birdc.equals(rec->getColor())){
			body->setEnable(true);
		}else
		{
			body->setEnable(false);
		}
		rec->setPhysicsBody(body);
		rec->setTag(REC_NEW);
		this->addChild(rec);
		this->recs.push_back(rec);
	}
}
int GameLayer::getRandomRecHeight() {
	int randomHeight = 0;
	while (randomHeight<5){
		randomHeight = rand()%(4*(REC_HEIGHT+REC_RANDOM_HEIGHT)/3);
	}
	int height = randomHeight+REC_HEIGHT-REC_RANDOM_HEIGHT;
	return height;
}
void GameLayer::checkHit() {
	if (((this->bird->getPositionX()+this->bird->getContentSize().width/2)<0)||(this->bird->getPositionY()+this->bird->getContentSize().height/2)<0)	{
		this->gameOver();
		return;
	}
	for (auto rec:this->recs){
		Color3B c = rec->getColor();
		if (c.equals(this->bird->getColor())&&rec->getTag()!=REC_PASS){
			if (rec->getBoundingBox().intersectsRect(this->bird->getBoundingBox())){
				// do a fix
				Size recSize = rec->getContentSize();
				Size birdSize = this->bird->getContentSize();
				auto maxRecY = rec->getPositionY()+recSize.height/2;
				auto minRecY = rec->getPositionY()-recSize.height/2;
				auto maxRecX = rec->getPositionX()+recSize.width/2;
				auto minRecX = rec->getPositionX()-recSize.width/2;
				auto maxBirdY = bird->getPositionY()+birdSize.height/2;
				auto minBirdY = bird->getPositionY()-birdSize.height/2;
				auto maxBirdX = bird->getPositionX()+birdSize.width/2;
				auto minBirdX = bird->getPositionX()-birdSize.width/2;
				if (maxBirdX>=maxRecX){
					break;
				}
				if ((maxBirdX>=minRecX)&&(maxRecY>=minBirdY+birdSize.height/2))
				{

					// 2
					// first give a v then give a fix
					auto deltaX = maxBirdX-minRecX;
					this->bird->setPositionX(minBirdX+birdSize.width/2-deltaX);
					maxBirdX = this->bird->getPositionX()+birdSize.width/2;
					minBirdX = this->bird->getPositionX()-birdSize.width/2;
					

				}
				// 4
				/*if (maxBirdX>=minRecX&&maxBirdY<=maxRecY+2)
				{
					auto deltaX = maxBirdX-minRecX;
					this->bird->setPositionX(minBirdX+birdSize.width/2-deltaX);
					maxBirdX = this->bird->getPositionX()+birdSize.width/2;
					minBirdX = this->bird->getPositionX()-birdSize.width/2;
					

				}*/
				break;
			}
		}

	}
}
void GameLayer::gameOver(){
	if (this->gameStatus == GAME_STATUS_OVER){
		return ;
	}
	if (WelcomeLayer::isMusicOpen)
	{
		SimpleAudioEngine::getInstance()->playEffect("die.wav");
	}

	//	get the best score
	int bestScore = UserRecord::getInstance()->readIntegerFromUserDefault("best_score");
	this->delegator->onGameEnd(score,bestScore);
	//	update the best score
	if(this->score > bestScore){
		UserRecord::getInstance()->saveIntegerToUserDefault("best_score",this->score);
	}
	
	// 停止移动
	this->unschedule(shiftLand);
	if (WelcomeLayer::isMusicOpen)
	{
		//SimpleAudioEngine::getInstance()->playEffect("sfx_die.ogg");
	}

	this->bird->die();
	this->bird->setRotation(-90);
	this->birdSpriteFadeOut();
	this->gameStatus = GAME_STATUS_OVER;

}
void GameLayer::birdSpriteFadeOut() {
	FadeOut* animationFadeOut = FadeOut::create(1.5);
	CallFunc* animationDoneFuc = CallFunc::create(bind(&GameLayer::birdSpriteRemove,this));
	Sequence* sequence = Sequence::createWithTwoActions(animationFadeOut,animationDoneFuc);

	this->bird->stopAllActions();
	this->bird->runAction(sequence);
}
void GameLayer::birdSpriteRemove() {
	this->bird->setRotation(0);
	this->removeChild(bird);

}