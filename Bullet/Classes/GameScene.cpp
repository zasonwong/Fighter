//
//  GameScene.cpp
//  Bullet
//
//  Created by zason on 7/1/16.
//
//

#include "GameScene.hpp"
#include "Constants.h"
#include "PomeloNetwork.hpp"
#include "json.h"
#include "User.hpp"
#include "PopView.hpp"

USING_NS_CC;


enum {
    kTagStartLabel,
};

GameScene* GameScene::pInstance = nullptr;

GameScene::GameScene():
countDown(COUNT_DOWN),
playerA(NULL),
visibleSize(Size::ZERO),
origin(Vec2::ZERO),
bullet(NULL),
bulletSpeed(0),
gameType(E_SINGLE),
isGameOver(true),
updatePTime(0.0),
vfinalPos(Vec2::ZERO)
{
    
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    pInstance = layer;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameScene::init()
{
    //////////////////////////////
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }

    
    bulletSpeed = 8;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    CCLOG("Origin:(%f, %f), Visible size: (%f, %f)", origin.x, origin.y, visibleSize.width, visibleSize.height);
    
    //sprite batch
    SpriteBatchNode *spriteBatch = SpriteBatchNode::create("gameArts.png");
    this->addChild(spriteBatch);
    

    //start label
    auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
    
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2));

    this->addChild(label, 1, kTagStartLabel);
    
    
    //menu item
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    this->schedule(CC_CALLBACK_1(GameScene::startStep, this), 1.0, countDown+1, 0.5f, "step_key");

    
    return true;
}

void GameScene::initData()
{
    bulletSpeed = 8;
}

///////////private methods

void GameScene::startStep(float dt)
{

    auto label = (Label *)getChildByTag(kTagStartLabel);
    char str[2];
    sprintf(str,"%d", countDown);

    if (countDown == 0) {
        label->setString("GO!");
    }else if(countDown < 0){
        label->removeFromParent();
        startGame();
    }else{
        label->setString(str);
    }
    
    countDown--;
    
}

void GameScene::startGame()
{
    
    initData();
    
    isGameOver = false;

    loadBackground();
    loadPlayer();
    loadBullet();
    
    this->scheduleUpdate();
}

void GameScene::loadBackground()
{
    
    Sprite *bg = Sprite::createWithSpriteFrameName("background_2.png");
    bg->setAnchorPoint(Vec2(0.5, 0.0f));
    bg->setPosition(origin.x + visibleSize.width / 2.0f, origin.y);
    CCLOG("Scale: %f", Director::getInstance()->getContentScaleFactor());
    bg->setScale(Director::getInstance()->getContentScaleFactor());
    this->addChild(bg,-1);
    
}

void GameScene::loadPlayer()
{
    
    if (gameType == E_SINGLE) {
        
        playerA = Player::createWithSpriteFrameName("hero_fly_1.png");
        playerA->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + Padding));
        this->addChild(playerA);

    }else if(gameType == E_ONLINE){
    
        playerA = Player::createWithSpriteFrameName("hero_fly_1.png");
        playerB = Player::createWithSpriteFrameName("hero_fly_1.png");
        
        if (User::getInstance()->isMain) {
            playerA->setPosition(Vec2(origin.x + visibleSize.width / 4.0f, origin.y + Padding));
            playerB->setPosition(Vec2(origin.x + visibleSize.width / 4.0f * 3.0f, origin.y + Padding));
            this->addChild(playerA, 2);
            this->addChild(playerB, 1);
        }else{
            playerA->setPosition(Vec2(origin.x + visibleSize.width / 4.0f * 3.0f, origin.y + Padding));
            playerB->setPosition(Vec2(origin.x + visibleSize.width / 4.0f, origin.y + Padding));
            this->addChild(playerB, 2);
            this->addChild(playerA, 1);
        }
    
    }

    
}

void GameScene::loadBullet()
{
    if (gameType == E_ONLINE) {
        return;
    }
    
    bullet = Sprite::createWithSpriteFrameName("bullet1.png");
    bullet->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bullet);
    
    resetBullet();
}

Vec2 GameScene::limitPosition(Vec2 newPos)
{
    Vec2 retval = newPos;
    retval.x = playerA->getPositionX()+newPos.x;
    retval.y = playerA->getPositionY()+newPos.y;
    
    if (retval.x >= origin.x + visibleSize.width - playerA->getContentSize().width / 2.0f) {
        retval.x = origin.x + visibleSize.width - playerA->getContentSize().width / 2.0f;
    }else if (retval.x <= origin.x + playerA->getContentSize().width / 2.0f) {
        retval.x = origin.x + playerA->getContentSize().width / 2.0f;
    }
    
    if (retval.y >= origin.y + visibleSize.height - Padding) {
        retval.y =  origin.y + visibleSize.height - Padding;
    }else if (retval.y <= Padding) {
        retval.y = Padding;
    }
    
    return retval;
}

void GameScene::gameOver()
{
    
    isGameOver = true;
    this->unscheduleUpdate();
    
}

void GameScene::resetBullet()
{
    bulletSpeed = (visibleSize.height-(playerA->getPositionY()))/18;
    
    if (bulletSpeed <5 )
    {
        bulletSpeed = 5;
    }
    
    CCLOG("## bullet speed: %d", bulletSpeed);
    
    bullet->setPosition(Vec2(playerA->getPositionX(),playerA->getPositionY() + playerA->getContentSize().height / 2.0f + 5.0f));
}

void GameScene::clear()
{
    
    playerA->removeFromParent();

    auto overItem = MenuItemFont::create("Game Over", nullptr);
    overItem->setFontSize(Title_Label_Font_Size);
    
    auto restartItem = MenuItemFont::create("Restart", CC_CALLBACK_1(GameScene::restart, this));
    
    overMenu = Menu::create(overItem, restartItem, nullptr);
    addChild(overMenu);
    overMenu->alignItemsVertically();
    
    auto s = Director::getInstance()->getWinSize();
    overMenu->setPosition(Vec2(s.width/2, s.height/2));
    
}

void GameScene::restart(cocos2d::Ref *pSender)
{
    
    overMenu->removeFromParent();
    startGame();
    
}

void GameScene::updatePosition(float x, float y)
{
    
    vfinalPos = Vec2(x, y);

}


////////// update

void GameScene::update(float delta)
{
 
    if (isGameOver) {
        return;
    }
    
    if (gameType == E_ONLINE) {
        
        updatePTime += delta;
        
        if (updatePTime > 1 / 24.0f) {
            
            updatePTime = 0.0f;
            playerB->stopAllActions();
            auto actionTo = MoveTo::create( 1 / 25.0f, vfinalPos);
            playerB->runAction(actionTo);
            
        }
        
        return;
    }
    
    updateBullet();
    loadEnemyPlane(delta);
    updateEnemyPlane(delta);
    collisionDetect();
    
}

void GameScene::updateBullet()
{
    
    if (!bullet) {
        return;
    }
    
    bullet->setPosition(Vec2(bullet->getPositionX(),bullet->getPositionY() + bulletSpeed));
    
    if (bullet->getPositionY() > origin.y + visibleSize.height) {
        resetBullet();
    }
}

void GameScene::updateEnemyPlane(float delta)
{

    Vector<EnemyPlane *>::iterator iter = enemyVector.begin();
    
    for  ( ; iter != enemyVector.end();) {
        
        EnemyPlane *enemy = *iter;
        enemy->update(delta);
        
        if (enemy->getPositionY() < VisibleOrigin.y - enemy->getContentSize().height) {
            enemy->removeFromParent();
            iter = enemyVector.erase(iter);
        }else{
            iter++;
        }
        
    }
    
}

void GameScene::loadEnemyPlane(float delta)
{
    
    static float timePass = 0.0f;
    timePass += delta;
    
    if (timePass > 2.0f) {
        
        EnemyPlane *enemy = EnemyPlane::createWithSpriteFrameName("enemy1_fly_1.png");
        this->addChild(enemy, 1);
        enemyVector.pushBack(enemy);
        timePass = 0;
        
    }
    
}

void GameScene::collisionDetect()
{

    //了弹和乱机碰撞
    Rect bulletRec = bullet->boundingBox();
    Vector<EnemyPlane *>::iterator iter = enemyVector.begin();
    for  ( ; iter != enemyVector.end();) {
        
        EnemyPlane *enemy = *iter;
        if (bulletRec.intersectsRect(enemy->boundingBox())) {//打到飞机
            this->resetBullet();
            enemy->removeFromParent();
            iter = enemyVector.erase(iter);
        }else{
            iter++;
        }
        
    }
    
    //乱机和飞机
    Rect playerRec = playerA->boundingBox();
    
    Vector<EnemyPlane *>::iterator iiter = enemyVector.begin();
    for  ( ; iiter != enemyVector.end();) {
        
        EnemyPlane *enemy = *iiter;
        if (playerRec.intersectsRect(enemy->boundingBox())) {//打到飞机
            
            this->resetBullet();
            enemy->removeFromParent();
            bullet->removeFromParent();
            
            iiter = enemyVector.erase(iiter);
            
            if (gameType == E_SINGLE) {
                
                playPlaneBlowupAnimation();
                gameOver();
                
            }
            
            break;
            
        }else{
            iiter++;
        }
        
    }
    
}

////////// animate

void GameScene::playPlaneBlowupAnimation()
{
    
    Vector<SpriteFrame*> actionVector;
    for (int i = 1; i<=4 ; i++ ) {
        
        char str[20];
        sprintf(str, "hero_blowup_%i.png",i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        actionVector.pushBack(frame);
        
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    Animation* animPlayer = Animation::createWithSpriteFrames(actionVector, 0.1f);
    //生成动画播放的行为对象
    Animate* actPlayer = Animate::create(animPlayer);
    //清空缓存数组
    actionVector.clear();
    
    auto action_cb = CallFunc::create(this, CC_CALLFUNC_SELECTOR(GameScene::clear));
    playerA->runAction(Sequence::create(actPlayer, action_cb, nullptr));
    
}

////////// callback
void GameScene::menuCloseCallback(Ref* pSender)
{
    if (gameType == E_ONLINE) {
        PomeloNetwork::getInstance()->disconnect();
    }
    Director::getInstance()->popToRootScene();
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
//    CCLOG("GameScene::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    
//    CCLOG("GameScene::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    if (isGameOver) {
        return;
    }
    
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    Vec2 oldTouchLocation = touch->getPreviousLocationInView();
    
    oldTouchLocation = Director::getInstance()->convertToGL(oldTouchLocation);
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    Vec2 translation = touchLocation - oldTouchLocation;
    
    Vec2 limitPosition = this->limitPosition(translation);
    
    if (gameType == E_ONLINE) {
        
        Json::Value value;
        value["x"] = limitPosition.x;
        value["y"] = limitPosition.y;
        std::string msg = value.toStyledString();
        PomeloNetwork::getInstance()->request(RouteMove, msg.c_str(), ExtraDataMove);
        
    }

    playerA->setPosition(limitPosition);//限制在屏幕范围内
    
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{

}

void GameScene::onUpdatePosition(float x, float y)
{
    if (!pInstance) {
        return;
    }
    
    pInstance->updatePosition(x, y);
    
}







