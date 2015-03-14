#include "OptionLayer.h"
OptionLayer::OptionLayer() {

}
OptionLayer::~OptionLayer() {

}
bool OptionLayer::init() {
	if (Layer::init()){
		// �������� 1 .�õ�dispatcher 2. ������Ӧ��listener 3.����listener�Ļص����� 4. ���Ⱥ������listener
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
	// ͨ��ί���ദ��ontouch�¼�
	// ���� �δ���ʼ���� ���麯��
	this->getDelegator()->onTouch();
}