#include "HelloWorldScene.h"
#include "PreStartScene.hpp"
#include "Constants.h"
#include "GameScene.hpp"
#include "GatewayNetwork.hpp"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{

    if ( !Layer::init() )
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto titleLabel = Label::createWithTTF("FIGHTER", "fonts/Marker Felt.ttf", Title_Label_Font_Size);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height * 0.7));
    this->addChild(titleLabel);
    
    
    auto singleLabel = Label::createWithTTF("Single", "fonts/Marker Felt.ttf", Sub_Title_Label_Font_Size);
    auto singleItem = MenuItemLabel::create(singleLabel, CC_CALLBACK_1(HelloWorld::singleGameCallback, this));
    singleItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2 + 20.0f));
    
    
    
    auto onlineItemLabel = Label::createWithTTF("Online", "fonts/Marker Felt.ttf", Sub_Title_Label_Font_Size);
    auto onlineItem = MenuItemLabel::create(onlineItemLabel, CC_CALLBACK_1(HelloWorld::onlineGameCallback, this));
    onlineItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height / 2 - 10.0f));
    
    
    
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(singleItem,onlineItem,closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    return true;
}

void HelloWorld::onlineGameCallback(Ref* pSender)
{
    GatewayNetwork::getInstance()->connect(GW_HOST.c_str(), GW_PORT, nullptr);
    Scene *pScene = PreStartScene::createScene();
    Director::getInstance()->pushScene(pScene);
}

void HelloWorld::singleGameCallback(Ref* pSender)
{
    Scene *pScene = GameScene::createScene();
    GameScene::setGameType(E_SINGLE);
    
    Director::getInstance()->pushScene(pScene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}
