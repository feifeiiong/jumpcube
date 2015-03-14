#include "UserRecord.h"

UserRecord::UserRecord()
{
}

UserRecord::~UserRecord()
{
}

bool UserRecord::init(){
	return true;
}

UserRecord* UserRecord::shareUserRecord = nullptr;

UserRecord* UserRecord::getInstance(){
	if(shareUserRecord == NULL){
		shareUserRecord = new UserRecord();
		if(!shareUserRecord->init()){
			delete(shareUserRecord);
			shareUserRecord = NULL;
		}
	}
	return shareUserRecord;
}

void UserRecord::saveIntegerToUserDefault(const char* key, int score){
	UserDefault::getInstance()->setIntegerForKey(key,score);	
}
void UserRecord::saveMusicStateToUserDefault(const char*key,bool isOpen) {
	UserDefault::getInstance()->setBoolForKey(key,isOpen);
}
bool UserRecord::readMusicStateFromUserDefault(const char*key) {
	return UserDefault::getInstance()->getBoolForKey(key,true);
}
int UserRecord::readIntegerFromUserDefault(const char* key){
	int score = UserDefault::getInstance()->getIntegerForKey(key);
	return score;
}