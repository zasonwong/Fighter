//
//  User.cpp
//  Bullet
//
//  Created by zason on 7/6/16.
//
//

#include "User.hpp"

User::User():
isMain(false)
{

}

User::~User()
{

}

User * User::getInstance()
{
    static User * instance = nullptr;
    if (!instance) {
        instance = new User();
        instance->init();
    }
    return instance;
}

void User::init()
{
    nickName = "zaza";
}