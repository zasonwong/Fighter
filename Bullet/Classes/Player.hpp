//
//  Player.hpp
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    static Player * createWithSpriteFrameName(const std::string& spriteFrameName);
    bool initWithSpriteFrameName(const std::string& spriteFrameName) override;
    virtual void onEnter() override;
    
};
#endif /* Player_hpp */
