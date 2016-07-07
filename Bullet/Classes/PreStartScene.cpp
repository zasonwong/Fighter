//
//  PreStartScene.cpp
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#include "PreStartScene.hpp"
#include "Constants.h"
#include "GameScene.hpp"
#include "PomeloNetwork.hpp"
#include "User.hpp"
#include <sstream>
#include "json.h"

USING_NS_CC;

PreStartScene* PreStartScene::pInstance = nullptr;

PreStartScene::PreStartScene():
playerBSprite(nullptr),
playerBLabel(nullptr),
waitingLabel(nullptr)
{
    
}

Scene* PreStartScene::createScene()
{

    auto scene = Scene::create();
    auto layer = PreStartScene::create();
    
    scene->addChild(layer);
    
    return scene;
    
}

bool PreStartScene::init()
{

    if (!Layer::init()) {
        return false;
    }
    
    pInstance = this;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto label = Label::createWithTTF("Waitting", "fonts/Marker Felt.ttf", Sub_Title_Label_Font_Size);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - 50));
    this->addChild(label);
    
    waitingLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", Sub_Title_Label_Font_Size);
    waitingLabel->setPosition(Vec2(label->getPosition().x + 40.0f,
                            origin.y + visibleSize.height - 50));
    this->addChild(waitingLabel);

    this->schedule(CC_SCHEDULE_SELECTOR(PreStartScene::startStep), 0.5f);
    
    Sprite * playerASprite = Sprite::createWithSpriteFrameName("hero_fly_1.png");
    playerASprite->setPosition(origin.x + visibleSize.width/2 - 50.0f, origin.y + visibleSize.height / 1.5);
    this->addChild(playerASprite);
    
    auto alabel = Label::createWithTTF(User::getInstance()->nickName, "fonts/Marker Felt.ttf", Tiny_Font_Size);
    alabel->setPosition(Vec2(playerASprite->getPosition().x,
                            playerASprite->getPosition().y - playerASprite->getContentSize().height / 2.0f - Font_Padding));
    
    this->addChild(alabel);
    
    playerBSprite = Sprite::createWithSpriteFrameName("hero_fly_1.png");
    playerBSprite->setPosition(origin.x + visibleSize.width/2 + 50.0f, origin.y + visibleSize.height / 1.5);
    this->addChild(playerBSprite);
    playerBSprite->setOpacity(0);//隐藏
    
    playerBLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", Tiny_Font_Size);
    playerBLabel->setPosition(Vec2(playerBSprite->getPosition().x,
                             playerBSprite->getPosition().y - playerBSprite->getContentSize().height / 2.0f - Font_Padding));
    
    this->addChild(playerBLabel);
    playerBLabel->setOpacity(0);//隐藏
    
    
    auto readyItemLabel = Label::createWithTTF("Ready", "fonts/Marker Felt.ttf", Button_Label_Font_Size);
    auto readyItem = MenuItemLabel::create(readyItemLabel, CC_CALLBACK_1(PreStartScene::gameReadyCallback, this));
    readyItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                playerASprite->getPosition().y - 80));
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(PreStartScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(readyItem,closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    return true;
    
}


void PreStartScene::startGame(float dt)
{

    Scene *pScene = GameScene::createScene();
    Director::getInstance()->pushScene(pScene);
    
}

//callback

void PreStartScene::startStep(float dt)
{
    static float time = -1;
    time++;
    if ( time == 0) {
        waitingLabel->setString("");
    }else if(time == 1){
        waitingLabel->setString(".");
    }else if(time == 2){
        waitingLabel->setString("..");
    }else if(time == 3){
        waitingLabel->setString("...");
    }else{
        time = 0;
    }
}

void PreStartScene::gameReadyCallback(Ref* pSender)
{
    return;
    Scene *pScene = GameScene::createScene();
    Director::getInstance()->pushScene(pScene);
}

void PreStartScene::menuCloseCallback(Ref* pSender)
{
    PomeloNetwork::getInstance()->disconnect();
    Director::getInstance()->popScene();
}

void PreStartScene::onUpdateCompeteName(std::string name)
{
    
    pInstance->playerBSprite->setOpacity(1.0f);
    pInstance->playerBLabel->setOpacity(1.0f);
    pInstance->playerBLabel->setString(name);
    
}

void PreStartScene::onStartGame()
{
    pInstance->schedule(CC_SCHEDULE_SELECTOR(PreStartScene::startGame), 2.0f);
}

