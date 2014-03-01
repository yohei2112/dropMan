#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#define PI 3.141592
#include "cocos2d.h"

USING_NS_CC;

const float ONE_SCREEN_DROP_TIME = 1;//1画面落下するのにかかる秒数
const float DROP_EASE_PARAM = 1;//落下の加速度、1で等速、増やすと初速が遅くなって終速が早くなる
const float SCROLL_TIME = 1.5;//スクロールにかかる時間
const int NUM_BACKGROUND = 3;//背景素材の数
const int CHARACTER_MOVE_SPEED_BASE = 10;//横移動の早さの基準、1フレームごとに、この値にタップ位置とキャラクターの位置関係から割り出した倍率をかけたピクセル数移動する
const int NUM_MAX_ENEMY = 5;
const int NUM_MIN_ENEMY = 3;
const int SAFETY_AREA_WIDTH_PARAM = 3;

class GameScene : public cocos2d::CCLayer
{
protected:
    enum kTag
    {
        kTag_Character = 1,
        kTag_Background,
        kTag_Action_DropCharacter,
        kTag_Action_ScrollCharacter,
        kTag_Action_MoveCharacterLeft,
        kTag_Action_MoveCharacterRight,
    };

    enum kZOrder
    {
        kZOrder_Background,
        kZOrder_Enemy,
        kZOrder_Character,
        kZOrder_Score,
        kZOrder_PlayButton,
        kZOrder_GameOver,
        kZOrder_Title,
    };

    CCLabelTTF* scoreLabel;
    CCLabelTTF* highScoreLabel;
    CCLabelTTF* winSizeLabel;
    CCLabelTTF* flagLabel;
    CCLabelTTF* gameOverLabel;
    CCLabelTTF* titleLabel;
    CCSprite* character;
    CCSprite* backgroundCurrent;
    CCSprite* backgroundNext;
    CCSprite* title;
    CCSpriteBatchNode* backgroundNode;
    CCSpriteBatchNode* togeNode;
    CCSpriteBatchNode* kabeNode;
    CCArray* deadSprites;
    CCDictionary* soundIds;
    CCSize winSize;
    CCPoint location;

    void makeBackground();
    void makeCharacter();
    void makeLabel();
    void jumpCharacter();
    void dropCharacter();
    void gameOver();
    void startGame();
    void scrollCharacter();
    void scrollAll();
    void scrollBackground();
    void scrollKabe();
    void scrollEnemy();
    void swapBackground();
    void moveCharacter();
    void deleteSprite(CCNode* sender);
    void setEnemy();
    void setKabe();
    void checkCollision();
    void gameOverAnimation();
    void setTcouchEnable();
    void playSound(CCString* name);
    void stopSound(CCString* name);
    void setTitle();
    void deleteTitle();

    void update(float dt);

    int scoreCount;
    int highScore;
    int scrollCount;
    int playCount;
    int deadSoundId;
    int dropSoundId;
    int scrollSoundId;
    
    bool isGame;
    bool isScroll;
    bool isTouch;

    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);



public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

    GameScene();

};

#endif // __GAME_SCENE_H__
