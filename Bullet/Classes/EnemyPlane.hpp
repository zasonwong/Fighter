//
//  EnemyPlane.hpp
//  Bullet
//
//  Created by zason on 7/8/16.
//
//

#ifndef EnemyPlane_hpp
#define EnemyPlane_hpp

#include <stdio.h>
#include "cocos2d.h"

class EnemyPlane : public cocos2d::Sprite
{

public:
    
    EnemyPlane();
    ~EnemyPlane(){};
    
    static EnemyPlane * createWithSpriteFrameName(const std::string& spriteFrameName);
    bool initWithSpriteFrameName(const std::string& spriteFrameName) override;
    virtual void onEnter() override;
    virtual void update(float delta) override;
    
private:
    int speed;
    
};

#endif /* EnemyPlane_hpp */
