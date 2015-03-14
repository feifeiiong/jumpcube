#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "OptionLayer.h"
#include "BirdSprite.h"
#include "BackgroundLayer.h"
#include "PictureLoader.h"
#include "CCArray.h"
#include "GameScene.h"
#include <cstdlib>
using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
#define min(X,Y) ((X) < (Y) ? (X) : (Y)) 
#define max(X,Y) ((X) > (Y) ? (X) : (Y)) 
const Color3B REC_GRAY = Color3B(97,98,128);
const Color3B REC_PINKRED = Color3B(255,102,52);
const Color3B REC_LIGHTYELLOW = Color3B(255,204,51);
const Color3B REC_LIGHTRED = Color3B(242,48,85);
const Color3B REC_LIGHTBLUE = Color3B(51,102,255);
const Color3B REC_LIGHTGRAY = Color3B(255,255,205);
const Color3B REC_GREEN = Color3B(0,204,101);
const int MAX_REC_READY = 12;
const float LAND_V = 0.01f;
const float BIRD_V = 900.0f;
const float FLY_TIME = BIRD_V/2600.0f;
const int REC_RANDOM_HEIGHT = (BIRD_V*BIRD_V)/(2*2600.0f)-20;
const int COLOR_COUNT = 7;
const int UP_PIP = 21;

const int DOWN_PIP = 12;

const int PIP_PASS = 30;

const int PIP_NEW = 31;
/**
* the tag for the rec
*/
const int REC_NEW = 31;
/**
* the tag for the rec
*/
const int REC_PASS = 30;
/**
* max bounds of the rec
*/
const int REC_MAX_BOUNDS = 2;
/**
 * The radius of the bird
 */
const int BIRD_RADIUS = 15;

/**
 * The height of the rec
 */
const int REC_HEIGHT = 0;

/**
 * The height of the pips
 */
const int PIP_HEIGHT = 320;

/**
 * The width of the pips
 */
const int PIP_WIDTH = 52;
/**
 * The width of the rec
 */
const int REC_WIDTH = 52;
/**
 * Pip shift speed
 */
const float PIP_SHIFT_SPEED = 80.0f;

/**
 * The distance between the down pip and up pip
 */
const int PIP_DISTANCE = 100;

/**
 * The distance between the pips vertical
 */
const int PIP_INTERVAL = 180;

/**
 * The number of pip pairs display in the screen in the same time
 */
const int PIP_COUNT = 2;
/**
 * the number of recs display in the screen 
*/
const int REC_COUNT = 10;
/**
 * The distance that the pip will display in the screen, for player to ready
 */
const int WAIT_DISTANCE = 100;
/**
* Define the game status
* GAME_STATUS_READY game is not start, just ready for payer to start.
* GAME_STATUS_START the game is started, and payer is paying this game.
* GAME_STATUS_OVER the player lost this game, the game is over.
*/
typedef enum _game_status{
	GAME_STATUS_READY = 1,
	GAME_STATUS_START,
	GAME_STATUS_OVER
} GameStatus;

class StatusDelegate {
public:
	
	/**
	* the method will be called when the game starts.
	* C++ doesn't have abstract class,it uses the pure virtual method instead.
	*/
	virtual void onGameStart(void) = 0;
	/**
	* the method will be called when the game is playing after the score changed.
	*/
	virtual void onGamePlaying(int score) = 0;
	/*
	*the method will be called when the game ends.
	*
	*/
	virtual void onGameEnd(int curSocre, int bestScore)= 0;
};
class GameLayer :public Layer,public OptionDelegate {
public:
	static Color3B getRandomColor();
	static int boundCount;
	GameLayer();
	~GameLayer();
	bool virtual init();
	CREATE_FUNC(GameLayer);
	/**
	* According to current game status, give the order to delegate.
	*/
	CC_SYNTHESIZE(StatusDelegate*, delegator, Delegator);
	/**
	* @Override 
	* To see @OptionDelegate::onTouch 
	* User have touched the screen
	* This game, user just need only this method to control game
	*/
	void onTouch();
	/**
	* This layer need physical engine work
	*/
	void setPhyWorld(PhysicsWorld* world){this->world = world;};
	void update(float delta);
private: 
	
	mutex recMutex;
	static int readyRecCount ;
	// the count will be added to the screen.
	int recCount;
	PhysicsWorld *world;
	GameStatus gameStatus;
	int score;
	BirdSprite *bird;
	Node * groundNode;
	// store the recs showed on the screen.
	vector<Node *> recs;
	SEL_SCHEDULE shiftLand;
	// the below will show the private methods
	/**
	* set the rotate for the bird,let it looks like the head is very heavy.
	*/
	void rotateBird();
	/**
	* create new pops and make it move from left to right then remove from parent
	*/
	void createPips();
	/**
	*get a random number that can set the pip height
	*/
	int getRandomHeight();
	 
    /**
     * Check the if the bird have hit anything
     */
	void checkHit();
	/**
	 * Since the only global bird can not be addChilded to two layers
	 * we must delete it from one layer,and then add it to another layer
	 * Here we use the fadeout animation to delete it from the gamelayer, so when the player 
	 * press the restart button, the bird can be added successfully
	 */
	void birdSpriteFadeOut();
	/*
	* after the fadeout animation run,delete the bird from current layer.
	*/
	void birdSpriteRemove();
	void scrollLand(float dt);
	bool onContactBegin(const PhysicsContact& contact);

	void onFirstSceneContact( Node* tempRecNode, Node* tempBirdNode );

	void gameOver();
	/**
	* create recs 
	*/
	void initLand();
	void createRecs();
	
	int getRandomRecHeight();
	void setNextColor(Color3B* color,int);
	void onContactSeperate(const PhysicsContact& contact);
};