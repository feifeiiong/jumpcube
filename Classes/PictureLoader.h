/**
* the struct is used to describe the real position of the element in the image
*
*/

#pragma once
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
/**
* To see the single line in the file:
* bird0_1 48 48 0.0546875 0.9472656 0.046875 0.046875
*/
typedef struct _altas {
	char *name ;
	int width;
	int height;
	Point start;
	Point end;
}Pictures;
class PictureLoader {
public:
	/**
	*  Get the instance of PictureLoader
	*/
	static PictureLoader* getInstance();
	/**
	*  Destroys the instance of PictureLoader.
	*/
	static void destroyInstance();

	/**
	* Load the atlas of file
	* This function load the image, so it will be delay the main thread
	* exp. PictureLoader::getInstance()->loadAtlas("atlas.txt");
	*/
	void loadAtlas(const char* filename);
	/**
	* Load the atlas of file
	* you can load the texture in asyc method then use this function
	*/
	void loadAtlas(const char* filename,Texture2D* text);
	/**
	* Get the sprite by name.
	* Warm: you should use this function after add atlas.png to getTextureCache,
	* or this function will take you a long time to load texture.
	* exp. SpriteFrame *bg_day = PictureLoader::getInstance()->getSpriteFrameByName("bg_day");
	*/
	SpriteFrame* getSpriteFrameByName(string name);
protected:
	/**
     *  The default constructor.
     */
	PictureLoader();
	/**
     *  Initializes the instance of PictureLoader. 
     *
     *  @note When you are porting Cocos2d-x to a new platform, you may need to take care of this method.
     *        You could assign a default value to _defaultResRootPath in the subclass of FileUtils(e.g. FileUtilsAndroid). Then invoke the FileUtils::init().
     *  @return true if successed, otherwise it returns false.
     *
     */
	virtual bool init();
	/**
     *  The singleton pointer of PictureLoader.
     */
	static PictureLoader* sharedPictureLoader;
	/**
	*  container to store all the sprite frame that already loaded;
	*/
	Map<std::string,SpriteFrame* > _spriteFrames;
};