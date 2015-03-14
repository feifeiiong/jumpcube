#include "StatusLayer.h"
#include "WelcomeScene.h"


StatusLayer::StatusLayer(){};

StatusLayer::~StatusLayer(){};


bool StatusLayer::init(){
	if(!Layer::init()){
		return false;
	}
	// init numbers
	this->bestScore = 0;
	this->currentScore = 0;
	this->isNewRecord = false;
	//Number::getInstance()->loadNumber(NUMBER_FONT.c_str(), "font_0%02d", 48);
	Number::getInstance()->loadNumber(NUMBER_FONT.c_str(), "big_num_%02d");
	Number::getInstance()->loadNumber(NUMBER_SCORE.c_str(), "num_%02d");
	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->originPoint = Director::getInstance()->getVisibleOrigin();
	this->showReadyStatus();
	this->loadWhiteSprite();
	return true;
}

void StatusLayer::showReadyStatus() {
	scoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_FONT.c_str(), 0);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto originPoint = Director::getInstance()->getVisibleOrigin();
	scoreSprite->setPosition(Point(this->originPoint.x + this->scoreSprite->getContentSize().width/2 + 20.0f, this->originPoint.y + this->visibleSize.height-scoreSprite->getContentSize().height-20.0f));
	this->addChild(scoreSprite);

	/*getreadySprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("text_ready"));
	getreadySprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height *2/3));
	this->addChild(getreadySprite);

	tutorialSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("tutorial"));
	tutorialSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height * 1/2));
	this->addChild(tutorialSprite);*/
}

void StatusLayer::showStartStatus() {
	//this->getreadySprite->runAction(FadeOut::create(0.4f));
	//this->tutorialSprite->runAction(FadeOut::create(0.4f));
}

void StatusLayer::showOverStatus(int curScore, int bestScore) {
	this->currentScore = curScore;
	this->bestScore = bestScore;
	if(curScore > bestScore){
		this->bestScore = curScore;
		this->isNewRecord = true;

	}else{
		this->isNewRecord = false;
	}
	//this->removeChild(scoreSprite);
	this->blinkFullScreen();
}

void StatusLayer::onGameStart(){
	this->showStartStatus();
}

void StatusLayer::onGamePlaying(int score){
	this->removeChild(scoreSprite);
	scoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_FONT.c_str(), score);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto originPoint = Director::getInstance()->getVisibleOrigin();
	scoreSprite->setPosition(Point(this->originPoint.x + this->scoreSprite->getContentSize().width/2 + 20.0f, this->originPoint.y + this->visibleSize.height-scoreSprite->getContentSize().height-20.0f));
	this->addChild(scoreSprite);
}

void StatusLayer::onGameEnd(int curScore, int bestScore){
	this->showOverStatus(curScore,bestScore);
}

void StatusLayer::loadWhiteSprite(){
	//this white sprite is used for blinking the screen for a short while
	whiteSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("whitebird"));
	whiteSprite->setScale(100);
	whiteSprite->setOpacity(0);
	this->addChild(whiteSprite,10000);
}

void StatusLayer::blinkFullScreen(){
	//display a flash blink
	auto fadeOut = FadeOut::create(0.1f);
	auto fadeIn = FadeIn::create(0.1f);
	auto blinkAction = Sequence::create(fadeIn,fadeOut,NULL);
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInGameOver, this));
	auto sequence = Sequence::createWithTwoActions(blinkAction, actionDone);
	whiteSprite->stopAllActions();
	whiteSprite->runAction(sequence);
}

void StatusLayer::fadeInGameOver(){    
	// create the game over panel
	//Sprite* gameoverSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("text_game_over"));
	//gameoverSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height *2/3));
	//this->addChild(gameoverSprite);
	auto gameoverFadeIn = FadeIn::create(0.5f);

	// Start next action
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::jumpToScorePanel, this));
	auto sequence = Sequence::createWithTwoActions(gameoverFadeIn, actionDone);
	this->runAction(sequence);
	/*gameoverSprite->stopAllActions();
	gameoverSprite->runAction(sequence);*/
}
void StatusLayer::showAD(){
}
void StatusLayer::jumpToScorePanel(){    
	// create the score panel
	showAD();
	this->removeChild(this->scoreFinalPanelSprite);
	this->scoreFinalPanelSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_rec"));
	scoreFinalPanelSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - scoreFinalPanelSprite->getContentSize().height));
	this->addChild(scoreFinalPanelSprite);

	//display the  best score on the score panel
	// create best score on the panel

	auto currentScoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_FONT.c_str(), this->currentScore, Gravity::GRAVITY_RIGHT);
	auto bigScoreSize = currentScoreSprite->getContentSize();
	bigScoreSize.setSize(bigScoreSize.width*1.5,bigScoreSize.height*1.5);
	currentScoreSprite->setContentSize(bigScoreSize);
	currentScoreSprite->setPosition(scoreFinalPanelSprite->getContentSize().width/2,scoreFinalPanelSprite->getContentSize().height/2);
	currentScoreSprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
	scoreFinalPanelSprite->addChild(currentScoreSprite);
	auto bestShowPanel = Sprite::create();
	bestShowPanel->setPosition(currentScoreSprite->getPositionX(),scoreFinalPanelSprite->getContentSize().height/6.0);
	auto bestShowSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("best_score"));
	auto tempSprite  = (Sprite *)Number::getInstance()->convert(NUMBER_SCORE.c_str(), bestScore);
	bestShowSprite->setPosition(bestShowSprite->getContentSize().width/2.0,bestShowSprite->getContentSize().height/2.0);
	tempSprite->setPosition(bestShowSprite->getPositionX()+bestShowSprite->getContentSize().width/2.0+tempSprite->getContentSize().width/2+5,bestShowSprite->getPositionY());
	bestShowPanel->addChild(bestShowSprite);
	bestShowPanel->addChild(bestShowSprite);
	/*auto bestScoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_SCORE.c_str(), this->bestScore, Gravity::GRAVITY_RIGHT);
	bestScoreSprite->setPosition(bestLabel->getPositionX()+bestLabel->getContentSize().width,bestLabel->getPositionY()+5);
	bestScoreSprite->setAnchorPoint(Point::ZERO);*/
	/*bestSprite->addChild(bestLabel);*/
	//string medalsName = this->getMedalsName(currentScore);
	//if(medalsName != "") {
	//	Sprite* medalsSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName(medalsName));
	//	medalsSprite->addChild(this->blink);
	//	medalsSprite->setPosition(54, 58);
	//	scorepanelSprite->addChild(medalsSprite);
	//}

	////if the current score is higher than the best score.
	////the panel will appear a "new" tag.
	//if(this->isNewRecord){
	//	Sprite* newTagSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("new"));
	//	newTagSprite->setPosition(-16, 12);
	//	bestScoreSprite->addChild(newTagSprite);
	//}

	// Start next action
	auto scorePanelMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + 1.6*this->visibleSize.height/3 - 10.0f));
	// add variable motion for the action
	EaseExponentialOut* sineIn = EaseExponentialOut::create(scorePanelMoveTo);
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInRestartBtn, this));
	auto sequence = Sequence::createWithTwoActions(sineIn, actionDone);
	scoreFinalPanelSprite->stopAllActions();
	//SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	scoreFinalPanelSprite->runAction(sequence);
}
void shareUMCallback(int platform, int stCode,
					 const string& errorMsg) {

}
void StatusLayer::fadeInRestartBtn(){
	auto finalPanelSize = this->scoreFinalPanelSprite->getContentSize();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Node * tmpNode = Node::create();
	//create the restart menu;
	Sprite* restartBtn = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_restart"));
	Sprite* restartBtnActive = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_restart"));
	restartBtnActive->setPositionY(-4);
	auto  menuItem = MenuItemSprite::create(restartBtn,restartBtnActive,NULL,CC_CALLBACK_1(StatusLayer::menuRestartCallback,this));
	auto menu = Menu::create(menuItem,NULL);
	menu->setPosition(scoreFinalPanelSprite->getPositionX()+scoreFinalPanelSprite->getContentSize().width/2-restartBtn->getContentSize().width/2,origin.y+visibleSize.height/2-finalPanelSize.height/2-restartBtn->getContentSize().height/2);
	menu->setAnchorPoint(Point::ZERO);
	tmpNode->addChild(menu);
	// share
	UMShareButton *shareButton = UMShareButton::create("share.png","share.png", "53a3f01456240b689600678e", share_selector(shareUMCallback)) ;
	// ********************** 设置平台信息 ***************************
	CCUMSocialSDK *sdk = shareButton->getSocialSDK();
	sdk->setQQAppIdAndAppKey("1101679567", "bMUGAx0G0qbKPiZ6");

	sdk->setWeiXinAppInfo("wxc5cfffb07cd6fbac","52fed70eb9a7c2ede1cc4f8f2a57f31e");

	// 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
	sdk->setTargetUrl("http://jumpcube.sinaapp.com/wx_share/");
	//     // 打开或者关闭log
	sdk->setLogEnable(true) ;
	// **********************   END ***************************
	// 设置友盟分享面板上显示的平台


	// 显示在友盟分享面板上的平台
	vector<int>* platforms = new vector<int>();
	platforms->push_back(SINA);
	platforms->push_back(WEIXIN_CIRCLE);
	platforms->push_back(WEIXIN);
	platforms->push_back(QZONE) ;
	platforms->push_back(QQ) ;
	platforms->push_back(RENREN);
	platforms->push_back(DOUBAN);
	shareButton->setPlatforms(platforms);
	// 设置文本分享内容
	shareButton->setShareContent("我发现了一个超贱的小清新游戏！快来挑战我的记录吧！") ;
	// 设置要分享的图片, 图片支持本地图片和url图片, 但是url图片必须以http://或者https://开头
	shareButton->setShareImage("http://jumpcube.sinaapp.com/static/images/weixinshare.png");

	// 设置按钮的位置 scoreFinalPanelSprite->getPositionX()
	shareButton->setPosition(scoreFinalPanelSprite->getPositionX(),menu->getPositionY());
	// 然后开发者需要将该按钮添加到游戏场景中
	this->shareMenu = CCMenu::create(shareButton, NULL);
	this->shareMenu->setAnchorPoint(Point::ZERO);
	this->shareMenu->setPosition(CCPointZero);
	tmpNode->addChild(this->shareMenu);
	// add the return button
	
	Sprite* returnBtn = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_return_little"));
	Sprite* returnBtnActive = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_return_little"));
	returnBtnActive->setPosition(0,-4);
	auto returnItem = MenuItemSprite::create(returnBtn,returnBtnActive,NULL,CC_CALLBACK_1(StatusLayer::menuReturnCallback,this));
	auto nextmenu = Menu::create(returnItem,NULL);
	nextmenu->setPosition(origin.x+visibleSize.width/2-finalPanelSize.width/2+returnBtn->getContentSize().width/2+10,menu->getPositionY());
	nextmenu->setAnchorPoint(Point::ZERO);
	tmpNode->addChild(nextmenu);
	this->addChild(tmpNode);
	//add the share Button


	



	this->addChild(tmpNode);
	//fade in the two buttons
	auto fadeIn = FadeIn::create(0.1f);
	//tmpNode->stopAllActions();
	//tmpNode->runAction(fadeIn);

	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::refreshScoreCallback,this));
	auto sequence = Sequence::createWithTwoActions(fadeIn,actionDone);
	tmpNode->stopAllActions();
	tmpNode->runAction(sequence);
	//wechat share
	if (this->isNewRecord)
	{
		showWechatPanel();
	}

}
void StatusLayer::showWechatPanel(){
	this->removeChild(this->wechatSharePanelSprite);
	this->wechatSharePanelSprite = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("wechat_share"));
	wechatSharePanelSprite->setPosition(this->scoreFinalPanelSprite->getPosition());
	this->addChild(wechatSharePanelSprite);
	auto wechatSharePanelSize = this->wechatSharePanelSprite->getContentSize();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	// add sharecontent
	stringstream ss;
	string s;
	ss << this->bestScore;
	ss >> s;
	auto bestLabel = LabelTTF::create("太任性了，你获得了"+s+"分，竟然击败了\n全国"+"的人，赶快分享到朋友圈炫耀一下!", "Arial", 24);
	bestLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	bestLabel->setColor(Color3B::BLUE);
	bestLabel->setPosition(wechatSharePanelSprite->getPositionX(),wechatSharePanelSize.height/2);
	wechatSharePanelSprite->addChild(bestLabel);
	//create the restart menu;
	auto leftLabel = LabelTTF::create("不，老子还可以\n更任性，再来一次！", "Arial", 24);
	leftLabel->setColor(Color3B::BLACK);
	auto  menuItem = MenuItemSprite::create(leftLabel,NULL,NULL,CC_CALLBACK_1(StatusLayer::menuRestartCallback,this));
	auto menu = Menu::create(menuItem,NULL);
	menu->setPosition(leftLabel->getContentSize().width/2+10,leftLabel->getContentSize().height/2+25);
	wechatSharePanelSprite->addChild(menu);
	// create wechatBtn
	auto rightLabel =  LabelTTF::create("好滴，任性一下", "Arial", 24);
	rightLabel->setColor(Color3B::RED);
	auto  shareMenuItem = MenuItemSprite::create(rightLabel,NULL,NULL,CC_CALLBACK_1(StatusLayer::wechatShareCallback,this));
	auto shareMenu = Menu::create(shareMenuItem,NULL);
	shareMenu->setPosition(wechatSharePanelSize.width-rightLabel->getContentSize().width/2.0-10,menu->getPositionY());
	wechatSharePanelSprite->addChild(shareMenu);

}
void StatusLayer::refreshScoreCallback(){
	this->tmpScore = 0;
	//schedule(schedule_selector(StatusLayer::refreshScoreExecutor),0.1f);
}

void StatusLayer::refreshScoreExecutor(float dt){
	if(this->getChildByTag(CURRENT_SCORE_SPRITE_TAG)){
		this->removeChildByTag(CURRENT_SCORE_SPRITE_TAG);
	}
	scoreSprite = (Sprite *)Number::getInstance()->convert(NUMBER_FONT.c_str(), this->tmpScore, Gravity::GRAVITY_RIGHT);
	scoreSprite->setPosition(Point(this->originPoint.x + this->scoreSprite->getContentSize().width/2 + 20.0f, this->originPoint.y + this->visibleSize.height-scoreSprite->getContentSize().height/2-20.0f));
	scoreSprite->setTag(CURRENT_SCORE_SPRITE_TAG);
	this->addChild(scoreSprite,1000);
	this->tmpScore++;
	if(this->tmpScore > this->currentScore){
		unschedule(schedule_selector(StatusLayer::refreshScoreExecutor));
	}
}

void StatusLayer::setBlinkSprite() {
	this->blink = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("blink_00"));
	Animation *animation = Animation::create();
	animation->setDelayPerUnit(0.1f);
	for (int i = 0; i < 3; i++){
		const char *filename = String::createWithFormat("blink_%02d", i)->getCString();
		SpriteFrame *frame = PictureLoader::getInstance()->getSpriteFrameByName(filename);
		animation->addSpriteFrame(frame);
	}
	for (int i = 2; i >= 0; i--){
		const char *filename = String::createWithFormat("blink_%02d", i)->getCString();
		SpriteFrame *frame = PictureLoader::getInstance()->getSpriteFrameByName(filename);
		animation->addSpriteFrame(frame);
	}
	auto animate = Animate::create(animation);
	auto actionDone = CallFunc::create(bind(&StatusLayer::blinkAction,this));
	auto sequence = Sequence::createWithTwoActions(animate, actionDone);
	blink->runAction(RepeatForever::create(sequence));
}

void StatusLayer::blinkAction() {
	if(this->blink && this->blink->getParent()) {
		Size activeSize = this->blink->getParent()->getContentSize();
		this->blink->setPosition(rand()%((int)(activeSize.width)), rand()%((int)(activeSize.height)));
	}
}

string StatusLayer::getMedalsName(int score){
	this->setBlinkSprite();

	//display the golden silver or bronze iron
	string medalsName = "";
	if(this->currentScore >=10 && this->currentScore < 20){//iron medals
		medalsName = "medals_0";
	}else if(this->currentScore >= 20 && currentScore < 30){//bronze medals
		medalsName = "medals_1";
	}else if(currentScore >=30 && currentScore < 50){//silver medals
		medalsName = "medals_2";
	}else if(currentScore >=50){//golden medals
		medalsName = "medals_3";
	}
	return medalsName;
}
void StatusLayer::wechatShareCallback(Ref* pSender) {
	stringstream ss;
	string scorestr;
	ss<<this->currentScore;
	ss>>scorestr;
	string s = "我在方块向前冲里创造了"+scorestr+"分的新纪录!快来挑战我吧!";
	auto sdk = CCUMSocialSDK::create("53a3f01456240b689600678e");
	sdk->setTargetUrl("http://jumpcube.sinaapp.com/wx_share/");
	sdk->setWeiXinAppInfo("wxc5cfffb07cd6fbac","52fed70eb9a7c2ede1cc4f8f2a57f31e");
	sdk->setPlatformShareContent(WEIXIN_CIRCLE,s.c_str(),"http://jumpcube.sinaapp.com/static/images/weixinshare.png",s.c_str(),"http://jumpcube.sinaapp.com/wx_share/");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	sdk->directShare(WEIXIN_CIRCLE, s.c_str(),"http://jumpcube.sinaapp.com/static/images/weixinshare.png", share_selector(shareUMCallback));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	sdk->directShare(WEIXIN_CIRCLE, s.c_str(),"http://jumpcube.sinaapp.com/static/images/weixinshare.png", share_selector(shareUMCallback));
#endif
}
void StatusLayer::menuRestartCallback(Ref* pSender){
	if (WelcomeLayer::isMusicOpen)
	{
		SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.wav");
	}


	auto scene = GameScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}
void StatusLayer::menuReturnCallback(Ref * pSender) {
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}


