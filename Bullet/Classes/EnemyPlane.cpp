//
//  EnemyPlane.cpp
//  Bullet
//
//  Created by zason on 7/8/16.
//
//

#include "EnemyPlane.hpp"
#include "Constants.h"

USING_NS_CC;

EnemyPlane::EnemyPlane():
speed(0)
{

}

EnemyPlane * EnemyPlane::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    
    EnemyPlane* enemy = new (std::nothrow)EnemyPlane();
    
    if (enemy && enemy ->initWithSpriteFrameName(spriteFrameName)) {
        enemy->autorelease();
        return enemy;
    }
    
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

bool EnemyPlane::initWithSpriteFrameName(const std::string& spriteFrameName)
{
    
    if (Sprite::initWithSpriteFrameName(spriteFrameName)) {
        return true;
    }
    return false;
}

void EnemyPlane::onEnter()
{
    
    Sprite::onEnter();
    this->speed = arc4random() % 4 + 1;
    this->setPosition(Vec2((arc4random()%(int)(VisibleSize.width) - 10) + VisibleOrigin.x + 10, VisibleOrigin.y + VisibleSize.height + 10.0f));
    
}

void EnemyPlane::update(float delta)
{
    this->setPosition(Vec2(this->getPositionX(), this->getPositionY() - this->speed));
}