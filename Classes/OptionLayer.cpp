#include "OptionLayer.h"
OptionLayer::OptionLayer() {

}
OptionLayer::~OptionLayer() {

}
bool OptionLayer::init() {
	if (Layer::init()){
		// 创建过程 1 .得到dispatcher 2. 创建对应的listener 3.设置listener的回掉函数 4. 调度函数添加listener
		auto dispather = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(OptionLayer::onTouchesBegan,this);
		dispather->addEventListenerWithSceneGraphPriority(listener,this);
		return true; 
	}else {
		return false;
	}
}
void OptionLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *event) {
	// 通过委托类处理ontouch事件
	// 问题 何处初始化了 纯虚函数
	this->getDelegator()->onTouch();
}