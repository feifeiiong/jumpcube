#include "PictureLoader.h"
#include "CCPlatformMacros.h"
PictureLoader* PictureLoader::sharedPictureLoader = nullptr;

PictureLoader* PictureLoader::getInstance() {
	if (NULL==sharedPictureLoader)
	{
		sharedPictureLoader = new PictureLoader();
		if (!sharedPictureLoader->init())
		{
			delete sharedPictureLoader;
			sharedPictureLoader = nullptr;
			CCLOG("error init");
		}
	}
	return sharedPictureLoader;
}
void PictureLoader::destroyInstance() {
	CC_SAFE_DELETE(sharedPictureLoader);
}
PictureLoader::PictureLoader(){}
bool PictureLoader::init() {
	return true;
}
void PictureLoader::loadAtlas(const char* filename) {
	auto texturePictures = Director::getInstance()->getTextureCache()->addImage("flappyrec.png");
	//auto textureAtlas = Director::getInstance()->getTextureCache()->addImage("atlas.png");
	this->loadAtlas(filename,texturePictures);
}
void PictureLoader::loadAtlas(const char* filename,Texture2D* texture) {
	std::string path = FileUtils::getInstance()->fullPathForFilename(filename);
	ValueMap fileMaps = FileUtils::getInstance()->getValueMapFromFile(path);
	ValueMap framesMaps = fileMaps.at("frames").asValueMap();
	Pictures atlas;
	for (auto e:framesMaps){
		atlas.name = (char *)malloc((e.first.length()+1)*sizeof(char));
		string newFirst = e.first.substr(0,e.first.size()-4);
		strcpy(atlas.name,newFirst.c_str());
		ValueMap vm = e.second.asValueMap();
		string frame =vm.at("frame").asString();
		bool isRotate = vm.at("rotated").asBool();
		frame = frame.substr(1,frame.size()-2);
		auto getPos =  [=](string frame){
			int firstpos = frame.find(",");
			string second = frame.substr(firstpos+1,frame.size()-1);
			return second.find(",")+firstpos+1;
		};
		int pos = getPos(frame);
		string startpoint = frame.substr(0,pos);
		string sizeString = frame.substr(pos+1,frame.size()-1-pos);
		Point  start = PointFromString(startpoint);
		Size size = SizeFromString(sizeString);
		atlas.start = start;
		if (isRotate)
		{
			atlas.height = size.width;
			atlas.width = size.height;
		}else
		{
			atlas.height = size.height;
			atlas.width = size.width;
		}
		
		Rect rect = Rect(atlas.start.x,atlas.start.y,atlas.width,atlas.height);
		auto spriteframe = SpriteFrame::createWithTexture(texture, rect);
		//spriteframe->setRotated(isRotate);
		this->_spriteFrames.insert(string(atlas.name), spriteframe);
	}

}
SpriteFrame* PictureLoader::getSpriteFrameByName(string name) {
	return _spriteFrames.at(name);
}