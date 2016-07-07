//
//  User.hpp
//  Bullet
//
//  Created by zason on 7/6/16.
//
//

#ifndef User_hpp
#define User_hpp

#include <stdio.h>

class User
{
public:
    User();
    ~User();
    
    static User * getInstance();
    void init();
    
    std::string nickName;
    std::string uid;
    std::string icon;
    
    bool isMain;//是否是主机

};

#endif /* User_hpp */
