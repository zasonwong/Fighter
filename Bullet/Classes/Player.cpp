//
//  Player.cpp
//  Bullet
//
//
//  Created by zason on 7/5/16.
//
//

#include "Player.hpp"

USING_NS_CC;

Player * Player::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    
    Player* player = new (std::nothrow)Player();
    
    if (player && player ->initWithSpriteFrameName(spriteFrameName)) {
        player->autorelease();
        return player;
    }
    
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::initWithSpriteFrameName(const std::string& spriteFrameName)
{

    if (Sprite::initWithSpriteFrameName(spriteFrameName)) {
        return true;
    }
    return false;
}


void Player::onEnter()
{
    
    Sprite::onEnter();

    Vector<SpriteFrame*> vPlayerAction;
    
    for (int i = 1 ; i<=2; i++) {
        
        char str[20];
        sprintf(str, "hero_fly_%d.png",i);
        //从内存池中取出Frame
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        //添加到序列中
        vPlayerAction.pushBack(frame);
        
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    Animation* animPlayer = Animation::createWithSpriteFrames(vPlayerAction, 0.1f);
    //生成动画播放的行为对象
    Animate* actPlayer = Animate::create(animPlayer);
    //清空缓存数组
    vPlayerAction.clear();
    
    this->runAction(RepeatForever::create(actPlayer));

}
