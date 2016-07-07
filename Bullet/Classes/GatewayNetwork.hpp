//
//  GatewayNetwork.hpp
//  Bullet
//
//  Created by zason on 7/6/16.
//
//

#ifndef GatewayNetwork_hpp
#define GatewayNetwork_hpp

#include <stdio.h>
#include "pomelo.h"
#include "cocos2d.h"

class GatewayNetwork
{
public:
    
    GatewayNetwork();
    ~GatewayNetwork();
    static GatewayNetwork* getInstance();
    
    void connect(const char* host, int port, const char* extraData);
    void disconnect();
    
private:
    
    void init();
    
};

#endif /* GatewayNetwork_hpp */
