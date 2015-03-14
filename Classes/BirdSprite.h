#pragma once
#include "cocos2d.h"
#include "PictureLoader.h"
using namespace cocos2d;
typedef enum{
	ACTION_STATE_IDLE,
	ACTION_STATE_FLY,
	ACTION_STATE_DIE
} ActionState;
const int BIRD_SPRITE_TAG = 10003;
class BirdSprite :public Sprite {

public:
	/**
	* Default construct
	*/
	BirdSprite();
	/**
	* Default distruct
	*/
	~BirdSprite();
	/************************************************************************/
	/* getInstance                                                                     */
	/************************************************************************/
	static BirdSprite* getInstance();
	/**
	* Cocos2d construct
	*/
	bool virtual init();
	/**
	* create && init the bird
	*/
	bool createBird();
	//CREATE_FUNC(BirdSprite);

	/**
	* The bird fly with swing, but do not effected by gravity.
	*/
	void idle();

	/**
	* The bird fly drived by player, effected by gravity. need physical support.
	*/
	void fly();

	/**
	* The bird die
	*/
	void die();
protected:
	/**
	* This method can create a frame animation with the likey name texture.
	*/
	static cocos2d::Animation * createAnimation(const char *fmt, int count, float fps);
	/**
	* Since this game has three different types of bird
	* this method is just used for choosing which type of bird by random
	*/
	void createBirdByRandom();
private:
	// this bird needs to be static 
	static BirdSprite * shareBirdSprite;
	Action* idleAction;

	//Action* flyAction;

	Action* swingAction;

	ActionState currentState;

	//the bird name will be created by random
	string birdName;

	//the bird name format depends on the bird name we have random created before
	string birdNameFormat;

	//record the first time into the game.
	unsigned int isFirstTime;
	/**
	* the method is used to change the bird's state,called by idle() or fly() etc.
	* I do not know when should I use the pointer and reference or either.
	*/
	bool changeState(ActionState state);


};