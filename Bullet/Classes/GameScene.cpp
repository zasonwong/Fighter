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

USING_NS_CC;


enum {
    kTagStartLabel,
};

GameScene* GameScene::pInstance = nullptr;

GameScene::GameScene():
countDown(COUNT_DOWN),
playerA(NULL),
visibleSize(Size::ZERO),
origin(Vec2::ZERO)
{
    
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
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
    
    pInstance = this;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    CCLOG("Origin:(%f, %f), Visible size: (%f, %f)", origin.x, origin.y, visibleSize.width, visibleSize.height);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    

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
    loadBackground();
    loadPlayer();
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
    
    playerA = Player::createWithSpriteFrameName("hero_fly_1.png");
    this->addChild(playerA);
    
    playerB = Player::createWithSpriteFrameName("hero_fly_1.png");
    this->addChild(playerB);
    
    if (User::getInstance()->isMain) {
        playerA->setPosition(Vec2(origin.x + visibleSize.width / 4.0f, origin.y + Padding));
        playerB->setPosition(Vec2(origin.x + visibleSize.width / 4.0f * 3.0f, origin.y + Padding));
    }else{
        playerA->setPosition(Vec2(origin.x + visibleSize.width / 4.0f * 3.0f, origin.y + Padding));
        playerB->setPosition(Vec2(origin.x + visibleSize.width / 4.0f, origin.y + Padding));
    }
    
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

////////// callback
void GameScene::menuCloseCallback(Ref* pSender)
{
    PomeloNetwork::getInstance()->disconnect();
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
    
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    Vec2 oldTouchLocation = touch->getPreviousLocationInView();
    
    oldTouchLocation = Director::getInstance()->convertToGL(oldTouchLocation);
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    Vec2 translation = touchLocation - oldTouchLocation;
    
//    if(std::abs(translation.x)> 1.0f || std::abs(translation.y) > 1.0f){
//    
//        Vec2 limitPosition = this->limitPosition(translation);
//        
//        Json::Value value;
//        value["x"] = limitPosition.x;
//        value["y"] = limitPosition.y;
//        std::string msg = value.toStyledString();
//        
//        PomeloNetwork::getInstance()->request(RouteMove, msg, ExtraDataMove);
//        
//        
//        playerA->setPosition(limitPosition);//限制在屏幕范围内
//        
//    }
    
    Vec2 limitPosition = this->limitPosition(translation);
    
    Json::Value value;
    value["x"] = limitPosition.x;
    value["y"] = limitPosition.y;
    std::string msg = value.toStyledString();
    
    PomeloNetwork::getInstance()->request(RouteMove, msg, ExtraDataMove);
    
    
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
    
    pInstance->playerB->setPosition(x, y);
    
}







