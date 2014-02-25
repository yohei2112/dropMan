#include "GameScene.h"
#include "AppMacros.h"
#include "AdMobUtil.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    highScore = userDefault->getIntegerForKey("highScore", 0);
    scrollCount = userDefault->getIntegerForKey("scrollCount", 0);
    playCount = userDefault->getIntegerForKey("playCount", 0);

/*
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadEffect("dead.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("scroll.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("drop.wav");
*/

    scoreCount = 0;
    this->removeAllChildrenWithCleanup(true);

    winSize = CCDirector::sharedDirector()->getWinSize();

    backgroundNode = CCSpriteBatchNode::create("background.png");
    this->addChild(backgroundNode, kZOrder_Background);

    togeNode = CCSpriteBatchNode::create("toge.png");
    this->addChild(togeNode, kZOrder_Enemy);

    kabeNode = CCSpriteBatchNode::create("kabe.png");
    this->addChild(kabeNode, kZOrder_Enemy);
    soundIds = CCDictionary::create();
    soundIds->setObject(CCInteger::create(1), "dead");
/*
    soundIds->setObject(2, "drop");
    soundIds->setObject(3, "scroll");
*/
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);


    isGame = false;
    isScroll = false;
    isTouch = false;

    setTitle();
    makeBackground();

    return true;
}

GameScene::GameScene()
:scoreCount(0)
{
}

bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    isTouch = true;
    if(!isGame)
    {
        init();
        startGame();
    }

    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

}

void GameScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

}

void GameScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    isTouch = false;

}

void GameScene::setTitle()
{
    title = CCSprite::create("title.png");
    title->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.7));
    this->addChild(title, kZOrder_Title);
    titleLabel = CCLabelTTF::create("TAP TO START", "", NUMBER_FONT_SIZE);
    titleLabel->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(titleLabel, kZOrder_Title);


}

void GameScene::deleteTitle()
{
    deleteSprite(title);
    titleLabel->removeFromParentAndCleanup(true);
}

void GameScene::makeCharacter()
{
    character = CCSprite::create("character.png");
    character->setPosition(ccp(winSize.width * 0.5, winSize.height - character->getContentSize().height * 0.5));
    this->addChild(character, kZOrder_Character);
    character->setTag(kTag_Character);

}

void GameScene::moveCharacter()
{
    if (!isGame || isScroll || fabs(location.x - character->getPositionX()) < CHARACTER_MOVE_SPEED_BASE)
    {
        return;
    }

    int moveScale = 1;

//    moveScale = floor(fabs(location.x - character->getPositionX()) / CHARACTER_MOVE_SPEED_BASE / 10) + 1;
CCLog ("moveScale = %d", moveScale);

    if(character->getPositionX() < location.x)
    {
        character->setPosition(ccp(character->getPositionX() + CHARACTER_MOVE_SPEED_BASE * moveScale, character->getPositionY()));
    }
    else
    {
        character->setPosition(ccp(character->getPositionX() - CHARACTER_MOVE_SPEED_BASE * moveScale, character->getPositionY()));
    }

}

void GameScene::makeBackground()
{
    backgroundCurrent = CCSprite::createWithTexture(backgroundNode->getTexture());
    backgroundCurrent->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    backgroundNode->addChild(backgroundCurrent, kZOrder_Background);

    backgroundNext = CCSprite::createWithTexture(backgroundNode->getTexture());
    backgroundNext->setPosition(ccp(winSize.width * 0.5, - winSize.height * 0.5));
    this->addChild(backgroundNext, kZOrder_Background);

}

void GameScene::makeLabel()
{
/*
    CCString* winSizeString = CCString::createWithFormat("winSize:x=%d y=%d\n\rframeSize:x=%d y=%d\n\rresolutionSize:x=%d y=%d", (int)winSize.width, (int)winSize.height,
    (int)CCEGLView::sharedOpenGLView()->getFrameSize().width, (int)CCEGLView::sharedOpenGLView()->getFrameSize().height,
    (int)CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width, (int)CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height);
    winSizeLabel = CCLabelTTF::create(winSizeString->getCString(), "", NUMBER_FONT_SIZE);
    winSizeLabel->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(winSizeLabel, kZOrder_Score);
/*
    CCString* flagString = CCString::createWithFormat("isScroll=%d", isScroll);
    flagLabel = CCLabelTTF::create(flagString->getCString(), "", NUMBER_FONT_SIZE * 0.5);
    flagLabel->setPosition(ccp(winSize.width - flagLabel->getContentSize().width, flagLabel->getContentSize().height));
    this->addChild(flagLabel, kZOrder_Score);
*/
    CCString* scoreString = CCString::createWithFormat("stage:%02d", scoreCount);
    scoreLabel = CCLabelTTF::create(scoreString->getCString(), "", NUMBER_FONT_SIZE);
    scoreLabel->setPosition(ccp(winSize.width - scoreLabel->getContentSize().width * 0.5, winSize.height - scoreLabel->getContentSize().height * 0.5));
    this->addChild(scoreLabel, kZOrder_Score);
/*
    CCString* highScoreString = CCString::createWithFormat("HIGH:%d", highScore);
    highScoreLabel = CCLabelTTF::create(highScoreString->getCString(), "", NUMBER_FONT_SIZE);
    highScoreLabel->setPosition(ccp(winSize.width - highScoreLabel->getContentSize().width * 0.5, winSize.height - highScoreLabel->getContentSize().height * 0.5));
    this->addChild(highScoreLabel, kZOrder_Score);
*/
}

void GameScene::update(float dt)
{
    if (!isGame)
    {
        return;
    }
    if (isTouch)
    {
        moveCharacter();
    }

    if(!isScroll)
    {
        checkCollision();
    }

}

void GameScene::startGame()
{
    deleteTitle();
    AdMobUtil::hideAdView();
    makeLabel();
    isGame = true;
    scheduleUpdate();
    dropCharacter();
}

void GameScene::dropCharacter()
{
    if (character->getActionByTag(kTag_Action_DropCharacter) != NULL || character->getActionByTag(kTag_Action_ScrollCharacter) != NULL)
    {
        return;
    }
    isScroll = false;
/*
    if(dropSoundId > 0)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(dropSoundId);
    }
    dropSoundId = SimpleAudioEngine::sharedEngine()->playEffect("drop.wav");
*/
    CCMoveTo* dropCharacter = CCMoveTo::create(ONE_SCREEN_DROP_TIME, ccp(character->getPosition().x, 0));
    CCActionInterval* easeAction = CCEaseIn::create(dropCharacter, DROP_EASE_PARAM);

    CCCallFuncN* dropAndScrollCharacter = CCCallFuncN::create(this, callfuncN_selector(GameScene::scrollAll));
    easeAction->setTag(kTag_Action_DropCharacter);
    character->runAction(CCSequence::create(easeAction, dropAndScrollCharacter, NULL));
}

void GameScene::scrollAll()
{
    scoreCount++;
    CCString* scoreString = CCString::createWithFormat("stage:%02d", scoreCount);
    scoreLabel->setString(scoreString->getCString());
/*
    if (scoreCount > highScore)
    {
        CCString* highScoreString = CCString::createWithFormat("HIGH:%d", scoreCount);
        highScoreLabel->setPosition(ccp(winSize.width - highScoreLabel->getContentSize().width * 0.5, winSize.height - highScoreLabel->getContentSize().height * 0.5));
        highScoreLabel->setString(highScoreString->getCString());
    }
*/
    isScroll = true;
    setEnemy();
    setKabe();
    scrollCharacter();
    scrollBackground();
    scrollEnemy();
    scrollKabe();

    scrollCount++;

}

void GameScene::scrollCharacter()
{
    character->stopAllActions();
    CCMoveTo* scrollCharacter = CCMoveTo::create(SCROLL_TIME, ccp(character->getPosition().x, winSize.height - character->getContentSize().height * 0.5));
    scrollCharacter->setTag(kTag_Action_ScrollCharacter);
    CCCallFuncN* scrollAndDropCharacter = CCCallFuncN::create(this, callfuncN_selector(GameScene::dropCharacter));

    character->runAction(CCSequence::create(scrollCharacter, scrollAndDropCharacter, NULL));
}

void GameScene::scrollBackground()
{
    CCMoveBy* scrollBackground = CCMoveBy::create(SCROLL_TIME, ccp(0, winSize.height));
    backgroundCurrent->runAction(scrollBackground);
    scrollBackground = CCMoveBy::create(SCROLL_TIME, ccp(0, winSize.height));
    CCCallFuncN* swap = CCCallFuncN::create(this, callfuncN_selector(GameScene::swapBackground));
    backgroundNext->runAction(CCSequence::create(scrollBackground, swap));
}

void GameScene::scrollEnemy()
{
    CCMoveBy* scrollAction;
    CCSprite* sprite;
    CCObject* child = NULL;
    CCARRAY_FOREACH(togeNode->getChildren(), child)
    {
        scrollAction = CCMoveBy::create(SCROLL_TIME, ccp(0, winSize.height));
        sprite = dynamic_cast<CCSprite*>(child);

        if (sprite->getPositionY() > sprite->getContentSize().height * 0.5)
        {
            CCCallFuncN* deleteEnemy = CCCallFuncN::create(this, callfuncN_selector(GameScene::deleteSprite));
            sprite->runAction(CCSequence::create(scrollAction, deleteEnemy, NULL));
        }
        else
        {
            sprite->runAction(scrollAction);
        }
    }

}

void GameScene::deleteSprite(CCNode* sender)
{
    CCSprite *sprite = (CCSprite *) sender;
    sprite->removeFromParentAndCleanup(true);
}

void GameScene::scrollKabe()
{
    CCMoveBy* scrollAction;
    CCSprite* sprite;
    CCObject* child = NULL;
    CCARRAY_FOREACH(kabeNode->getChildren(), child)
    {
        scrollAction = CCMoveBy::create(SCROLL_TIME, ccp(0, winSize.height));
        sprite = dynamic_cast<CCSprite*>(child);

        if (sprite->getPositionY() > winSize.height)
        {
            CCCallFuncN* deleteKabe = CCCallFuncN::create(this, callfuncN_selector(GameScene::deleteSprite));
            sprite->runAction(CCSequence::create(scrollAction, deleteKabe, NULL));
        }
        else
        {
            sprite->runAction(scrollAction);
        }
    }
}

void GameScene::setKabe()
{
    int kabePosisionX;
    int safetyAreaWidth = character->getContentSize().width * SAFETY_AREA_WIDTH_PARAM;
    kabePosisionX = (arc4random()%(int)winSize.width * 0.5) - safetyAreaWidth - winSize.width * 0.1;
CCLog ("debug:safetyAreaWidth=: %d", safetyAreaWidth);
CCLog ("debug:kabePos=: %d", kabePosisionX);

    CCSprite* kabe1;
    CCSprite* kabe2;

    kabe1 = CCSprite::createWithTexture(kabeNode->getTexture());
    kabe1->setPosition(ccp(kabePosisionX, -winSize.height));
    kabeNode->addChild(kabe1, kZOrder_Enemy);
    kabe2 = CCSprite::createWithTexture(kabeNode->getTexture());
    kabe2->setPosition(ccp(kabePosisionX + kabe2->getContentSize().width + safetyAreaWidth, -winSize.height));
    kabeNode->addChild(kabe2, kZOrder_Enemy);

}

void GameScene::swapBackground()
{
    backgroundCurrent = backgroundNext;

    backgroundNext = CCSprite::createWithTexture(backgroundNode->getTexture());
    backgroundNext->setPosition(ccp(winSize.width * 0.5, - winSize.height * 0.5));
    backgroundNode->addChild(backgroundNext, kZOrder_Background);
    isScroll = false;
}

void GameScene::setEnemy()
{
    int setEnemyCount = MIN(NUM_MAX_ENEMY, arc4random()%NUM_MAX_ENEMY + NUM_MIN_ENEMY);

    int enemyPosisionX;
    int enemyPosisionY;

    CCSprite* enemy;

    for(int i = 0; i < setEnemyCount; i++)
    {
        enemyPosisionX = arc4random()%(int)winSize.width;
        enemyPosisionY = arc4random()%(int)winSize.height * 0.6 + winSize.height * 0.2;
        enemy = CCSprite::createWithTexture(togeNode->getTexture());
        enemy->setPosition(ccp(enemyPosisionX, enemyPosisionY - winSize.height));
        togeNode->addChild(enemy, kZOrder_Enemy);

    }
}

void GameScene::checkCollision()
{
    CCRect characterRect;
    if (isTouch)
    {
        characterRect = CCRectMake(
            character->getPosition().x - character->getContentSize().width * 0.2,
            character->getPosition().y - character->getContentSize().height * 0.2,
            character->getContentSize().width * 0.4,
            character->getContentSize().height * 0.4
            );
    }
    else
    {
        characterRect = CCRectMake(
            character->getPosition().x - character->getContentSize().width * 0.3,
            character->getPosition().y - character->getContentSize().height * 0.3,
            character->getContentSize().width * 0.6,
            character->getContentSize().height * 0.6
            );
    }

    CCSprite* sprite;
    CCObject* child = NULL;
    CCRect togeRect;
    CCARRAY_FOREACH(togeNode->getChildren(), child)
    {
        sprite = dynamic_cast<CCSprite*>(child);
        togeRect = CCRectMake(
                sprite->getPosition().x - sprite->getContentSize().width * 0.25,
                sprite->getPosition().y - sprite->getContentSize().height * 0.25,
                sprite->getContentSize().width * 0.5,
                sprite->getContentSize().height * 0.5
                );
        if (characterRect.intersectsRect(togeRect))
        {
            gameOver();
            return;
        }

    }

    CCRect kabeRect;
    CCARRAY_FOREACH(kabeNode->getChildren(), child)
    {
        sprite = dynamic_cast<CCSprite*>(child);
        if (sprite->getPosition().y == 0)
        {
            kabeRect = CCRectMake(
                    sprite->getPosition().x - sprite->getContentSize().width * 0.5 + character->getContentSize().width * 0.1,
                    sprite->getPosition().y - sprite->getContentSize().height * 0.5,
                    sprite->getContentSize().width - character->getContentSize().width * 0.2,
                    sprite->getContentSize().height
                    );
            if (characterRect.intersectsRect(kabeRect))
            {
                gameOver();
                return;
            }
        }
    }
}

void GameScene::gameOver()
{
    isGame = false;
    AdMobUtil::showAdView();
    gameOverAnimation();

    CCObject* child = NULL;
    CCARRAY_FOREACH(togeNode->getChildren(), child)
    {
        CCSprite* sprite = dynamic_cast<CCSprite*>(child);
        sprite->stopAllActions();
    }
    character->stopAllActions();

    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

    if (highScore < scoreCount)
    {
        CCLog ("update highScore :%d to %d", highScore, scoreCount);
        highScore = scoreCount;
        userDefault->setIntegerForKey("highScore", highScore);
    }

    playCount++;
    userDefault->setIntegerForKey("playCount", playCount);
    userDefault->setIntegerForKey("scrollCount", scrollCount);
    userDefault->flush();

    CCString* gameOverString = CCString::createWithFormat("GAME OVER\n\rSCORE:%d\n\rHIGH SCORE:%d\n\rTAP TO RESTART", scoreCount, highScore);
    gameOverLabel = CCLabelTTF::create(gameOverString->getCString(), "", NUMBER_FONT_SIZE);
    gameOverLabel->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(gameOverLabel, kZOrder_Score);


}

void GameScene::gameOverAnimation()
{
    this->setTouchEnabled(false);

    character->setVisible(false);
    deadSprites = CCArray::create();
    CCSprite* deadSprite;
    for (int i = 0; i < 24; i++)
    {
        deadSprite = CCSprite::create("deadAnime01.png");
        deadSprite->setPosition(ccp(character->getPositionX(), character->getPositionY()));
        deadSprites->addObject(deadSprite);
    }
    CCAnimation* animation;
    CCAnimate* animateAction;

    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::setTcouchEnable));

    CCMoveBy* moveAction;
    int distance;
    int posX;
    int posY;
    int speed;

    CCSprite* sprite;
    CCObject* child = NULL;
    int i = 0;
    CCARRAY_FOREACH(deadSprites, child)
    {
        distance = deadSprite->getContentSize().width * 2 * (int)(i/8+1);
        deadSprite = dynamic_cast<CCSprite*>(child);
        this->addChild(deadSprite);
        posX = (int)distance * cos(i * PI / 4);
        posY = (int)distance * sin(i * PI / 4);
        CCLog ("i:%d posX :%d posY :%d",i, posX, posY);
        
        moveAction = CCMoveBy::create(1.5, ccp(posX, posY));

        animation = CCAnimation::create();
        animation->addSpriteFrameWithFileName("deadAnime01.png");
        animation->addSpriteFrameWithFileName("deadAnime02.png");
        animation->setDelayPerUnit(0.25);
        animation->setLoops(3);

        animateAction = CCAnimate::create(animation);
        if (i == 0)
        {
            deadSprite->runAction(CCSpawn::create(moveAction, CCSequence::create(animateAction, func, NULL), NULL));
        }
        else
        {
            deadSprite->runAction(CCSpawn::create(moveAction, animateAction, NULL));
        }
        i++;

    }

}

void GameScene::setTcouchEnable()
{
    this->setTouchEnabled(true);
}

