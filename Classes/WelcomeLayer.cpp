




#include "WelcomeLayer.h"
#include "UserRecord.h"
#include "Number.h"
#include "StatusLayer.h"
const char* KEY_MUSIC_STATE = "MUSIC_STATE";
WelcomeLayer::WelcomeLayer(){}
WelcomeLayer::~WelcomeLayer(){}
bool WelcomeLayer::isMusicOpen = true;
void WelcomeLayer::initLand() {
	this->createRecs();
}
void WelcomeLayer::createRecs() 
{
	Color3B tempColor = GameLayer::getRandomColor();
	for (int i = 0;i<this->recCount;i++){
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point orign = Director::getInstance()->getVisibleOrigin();
		auto rec = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("white_rec"));
		rec->setColor(tempColor);
		rec->setPositionX(orign.x+i*(rec->getContentSize().width));
		rec->setPositionY(REC_HEIGHT-45);
		this->addChild(rec);
		this->recs.push_back(rec);
	}
}
void WelcomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode==EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		this->menuQuitCallback(nullptr);
	}
}
bool WelcomeLayer::init(){
	if (!Layer::init()){
		return false;
	}
	this->setKeyboardEnabled(true);
	Number::getInstance()->loadNumber(NUMBER_SCORE.c_str(), "num_%02d");
	Number::getInstance()->loadNumber(NUMBER_FONT.c_str(), "num_%02d");
	isMusicOpen = UserRecord::getInstance()->readMusicStateFromUserDefault(KEY_MUSIC_STATE);
	// get the orign point of the X-Y axis, and the visible size of the screen.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	// get the current time, the background image will be selected by current time day or night
	time_t t = time(NULL);
	tm* lt = localtime(&t);
	int hour = lt->tm_hour;
	//create the background image according to the current time;
	Sprite *background;
	background = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_bg"));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background);
	// add the world game-title to the current scene
	createTitle();

	// add the rank menu to the current scene
	this->createRankButton();
	// add the audio menu to the current scene
	this->createAudioButton();
	// add the start menu to the current scene
	this->createPlayButton();
	// Add the land
	auto rec = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("white_rec"));
	auto boundRec = rec->getBoundingBox();
	int boundwidth = boundRec.size.width;
	this->recCount = (visibleSize.width/boundRec.size.width)+2;
	initLand();

	this->schedule(schedule_selector(WelcomeLayer::scrollLand), 0.01f);
	return true;
}

void WelcomeLayer::scrollLand(float dt){
	for (auto rec : this->recs){
		int movDistance = 2.5;
		rec->setPositionX(rec->getPositionX()-movDistance);
		if (rec->getPositionX()<(-rec->getContentSize().width)){
			rec->setTag(REC_NEW);
			Point point = Director::getInstance()->getVisibleOrigin();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			float deta = (-rec->getPositionX()-rec->getContentSize().width);
			rec->setPositionX(point.x+(this->recCount-1)*rec->getContentSize().width-deta);

		}
	}
}
void WelcomeLayer::menuAudioCallback(Ref* sender) {
	if (isMusicOpen)
	{
		isMusicOpen = false;

	}else
	{
		isMusicOpen = true;
	}
	this->createAudioButton();
	UserRecord::getInstance()->saveMusicStateToUserDefault(KEY_MUSIC_STATE,isMusicOpen);
}
void WelcomeLayer::menuRankCallback(Ref * sender) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto titleAction1 = MoveTo::create(0.8f,Point(origin.x-title->getContentSize().width,this->title->getPositionY()));
	title->runAction(titleAction1);

	auto audioMenuAction = MoveTo::create(0.8f,Point(origin.x-audioMenu->getContentSize().width,this->audioMenu->getPositionY()));
	audioMenu->runAction(audioMenuAction);

	auto playMenuAction = MoveTo::create(0.8f,Point(origin.x+playMenu->getContentSize().width+visibleSize.width,this->playMenu->getPositionY()));
	playMenu->runAction(playMenuAction);

	auto rankMenuAction = MoveTo::create(0.8f,Point(origin.x+rankMenu->getPositionX(),-this->rankMenu->getPositionY()-this->rankMenu->getContentSize().height));
	auto actionDone = CallFunc::create(bind(&WelcomeLayer::moveFunc,this));
	auto rankSequence = Sequence::createWithTwoActions(rankMenuAction,actionDone);
	rankMenu->runAction(rankSequence);

}
void WelcomeLayer::returnFunc() {
	this->createTitle();
	this->createPlayButton();
	this->createAudioButton();
	this->createRankButton();

}
void WelcomeLayer::menuQuitCallback(Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
	JniMethodInfo minfo;//定义Jni函数信息结构体 
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
	//bool isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showAD", "()V");
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showQuitDialog", "()V");  

	if (!isHave) { 
		CCLog("jni:no this method"); 
	}else{ 
		CCLog("jni:have method"); 
		//调用此函数 

		//minfo.env->CallVoidMethod(jActivity, minfo.methodID);
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID); 
	} 
#endif 
}
void WelcomeLayer::menuReturnCallback(Ref* sender) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto scoreAction = MoveTo::create(0.8f,Point(-500,this->scoreSprite->getPositionY()));
	this->scoreSprite->runAction(scoreAction);
	if (this->shareMenu!=nullptr)
	{
		auto shareAction = MoveTo::create(0.8f,Point(origin.x+visibleSize.width+this->shareMenu->getContentSize().width,this->shareMenu->getPositionY()));

		this->shareMenu->runAction(shareAction);
	}


	auto returnAction = MoveTo::create(0.8f,Point(origin.x-returnMenu->getContentSize().width,this->returnMenu->getPositionY()));
	auto returnFunc = CallFunc::create(bind(&WelcomeLayer::returnFunc,this));
	auto sequence = Sequence::createWithTwoActions(returnAction,returnFunc);
	this->returnMenu->runAction(sequence);
}
void WelcomeLayer::moveFunc() {

	this->showNewScene();
}

void WelcomeLayer::createPlayButton() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	this->removeChild(playMenu);
	Sprite * startButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_start"));
	Sprite * activeStartButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_start"));
	activeStartButton->setPositionY(5.0f);
	auto menuItem = MenuItemSprite::create(startButton,activeStartButton,NULL,CC_CALLBACK_1(WelcomeLayer::menuStartCallback,this));
	menuItem->setPosition(origin.x + visibleSize.width/2 ,origin.y + visibleSize.height*3/6);
	this->playMenu = Menu::create(menuItem,NULL);
	playMenu->setPosition(origin.x,origin.y);
	this->addChild(playMenu,1);
}
void WelcomeLayer::createTitle(){
	this->removeChild(this->title);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	this->title = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_title"));
	title->setPosition(origin.x+visibleSize.width/2,(visibleSize.height*7)/9);
	this->addChild(title);
}

void WelcomeLayer::createReturnButton() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	// create return button
	this->removeChild(returnMenu);
	auto returnButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_return_little"));
	auto returnButtonActive = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_return_little"));
	returnButtonActive->setPositionY(-5.0f);
	auto menuItemReturn = MenuItemSprite::create(returnButton,returnButtonActive,NULL,CC_CALLBACK_1(WelcomeLayer::menuReturnCallback,this));
	menuItemReturn->setPosition(origin.x+menuItemReturn->getContentSize().width,origin.y+menuItemReturn->getContentSize().height);
	returnMenu = Menu::create(menuItemReturn,NULL);
	returnMenu->setPosition(origin.x,origin.y);
	this->addChild(returnMenu,1);


}
void shareCallback(int platform, int stCode,
				   const string& errorMsg) {

}
void WelcomeLayer::createShareButton(){
	this->removeChild(shareMenu);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	// 创建分享按钮, 参数1为按钮正常情况下的图片, 参数2为按钮选中时的图片,参数3为友盟appkey, 参数4为分享回调
	UMShareButton *shareButton = UMShareButton::create("share.png","share.png", "53a3f01456240b689600678e", share_selector(shareCallback)) ;
	// ********************** 设置平台信息 ***************************
	


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
	CCUMSocialSDK *sdk = shareButton->getSocialSDK();
	sdk->setQQAppIdAndAppKey("1101679567", "bMUGAx0G0qbKPiZ6");
	sdk->setWeiXinAppInfo("wxc5cfffb07cd6fbac","52fed70eb9a7c2ede1cc4f8f2a57f31e");
	// 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
	sdk->setTargetUrl("http://jumpcube.sinaapp.com/wx_share/");
	//     // 打开或者关闭log
	sdk->setLogEnable(true) ;
	// **********************   END ***************************
	// 设置友盟分享面板上显示的平台
	// 设置按钮的位置
	shareButton->setPosition(origin.x+visibleSize.width-shareButton->getContentSize().width,origin.y+shareButton->getContentSize().height);
	// 然后开发者需要将该按钮添加到游戏场景中
	this->shareMenu = CCMenu::create(shareButton, NULL);
	this->shareMenu->setPosition(CCPointZero);
	this->addChild(this->shareMenu, 1);


}
void WelcomeLayer::showNewScene(){
	createReturnButton();
	createShareButton();
	createScoreSprite();
}
void WelcomeLayer::createScoreSprite(){
	this->removeChild(scoreSprite);
	this->scoreSprite = Sprite::create();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point orign = Director::getInstance()->getVisibleOrigin();
	auto bestScore = LabelTTF::create("Best Score:  ", "Arial", 24);
	bestScore->setColor(Color3B::BLUE);
	bestScore->setPosition(0,0);

	int best = UserRecord::getInstance()->readIntegerFromUserDefault("best_score");

	auto bestSprite = (Sprite *)Number::getInstance()->convert(NUMBER_SCORE.c_str(), best, Gravity::GRAVITY_CENTER);
	bestSprite->setPosition(bestScore->getContentSize().width-20,-10);
	this->scoreSprite->addChild(bestScore);
	this->scoreSprite->addChild(bestSprite);
	this->scoreSprite->setPosition(visibleSize.width/2,2*visibleSize.height/3);
	this->addChild(scoreSprite);
}
void WelcomeLayer::createRankButton() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto rankButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("rank"));
	auto rankButtonActive = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("rank"));
	rankButtonActive->setPositionY(-5.0f);
	auto menuItemRank = MenuItemSprite::create(rankButton,rankButtonActive,NULL,CC_CALLBACK_1(WelcomeLayer::menuRankCallback,this));
	menuItemRank->setPosition(origin.x+visibleSize.width-menuItemRank->getContentSize().width,origin.y+menuItemRank->getContentSize().height);
	rankMenu = Menu::create(menuItemRank,NULL);
	rankMenu->setPosition(origin.x,origin.y);
	this->addChild(rankMenu,1);
}
void WelcomeLayer::createAudioButton() {
	this->removeChild(this->audioMenu);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	Sprite* audioMenuButton;
	Sprite *activeAudioMenuButton;
	if (isMusicOpen)
	{
		audioMenuButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_audio_open"));
		activeAudioMenuButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_audio_open"));
	}
	else{
		audioMenuButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_audio_close"));
		activeAudioMenuButton = Sprite::createWithSpriteFrame(PictureLoader::getInstance()->getSpriteFrameByName("flappyrec_welcome_audio_close"));
	}
	activeAudioMenuButton->setPositionY(5.0f);
	auto menuItemAudio = MenuItemSprite::create(audioMenuButton,activeAudioMenuButton,NULL,CC_CALLBACK_1(WelcomeLayer::menuAudioCallback,this));
	menuItemAudio->setPosition(origin.x+menuItemAudio->getContentSize().width,origin.y+menuItemAudio->getContentSize().height);
	this->audioMenu = Menu::create(menuItemAudio,NULL);
	audioMenu->setPosition(origin.x,origin.y);
	this->addChild(audioMenu,1);
}
void WelcomeLayer::menuStartCallback(Ref *sender){
	if (isMusicOpen)
	{
		SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.wav");
	}

	this->removeChildByTag(BIRD_SPRITE_TAG);
	auto scene = GameScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}






