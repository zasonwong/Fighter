//
//  PomeloNetwork.hpp
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#ifndef PomeloNetwork_hpp
#define PomeloNetwork_hpp

#include <stdio.h>
#include "pomelo.h"
#include "cocos2d.h"

class PomeloNetwork
{
public:
    
    PomeloNetwork();
    ~PomeloNetwork();
    static PomeloNetwork* getInstance();
    
    void connect(const char* host, int port, const char* extraData);
    void request(const char* route, const char* msg, const char* extraData);
    void request(const char* route, std::string msg, const char* extraData);
    void disconnect();
    void init();

};

#endif /* PomeloNetwork_hpp */
