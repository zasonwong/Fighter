//
//  PomeloNetwork.cpp
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#include "PomeloNetwork.hpp"
#include "Constants.h"
#include "json.h"
#include "GameScene.hpp"
#include "PreStartScene.hpp"
#include "User.hpp"

static int handler_id;

static pc_client_t* client;

static void connectCallback(pc_client_t* client, int ev_type, void* ex_data, const char* arg1, const char* arg2);
static void requestCallback(const pc_request_t* req, int rc, const char* resp);

PomeloNetwork::PomeloNetwork()
{
    
}

PomeloNetwork::~PomeloNetwork()
{
    disconnect();
}

PomeloNetwork * PomeloNetwork::getInstance()
{
    
    static PomeloNetwork * instance = nullptr;
    
    if (!instance)
    {
        instance = new (std::nothrow) PomeloNetwork();
        instance->init();
    }
    
    return instance;
    
}

void PomeloNetwork::init()
{

}

void PomeloNetwork::connect(const char* host, int port, const char* extraData)
{
    
    if (!client){
        client = (pc_client_t*)malloc(pc_client_size());
    }
    
    pc_client_config_t config = PC_CLIENT_CONFIG_DEFAULT;
    config.transport_name = PC_TR_NAME_UV_TCP;
    pc_client_init(client,  NULL, &config);
    
    handler_id = pc_client_add_ev_handler(client, connectCallback, (void *)extraData, NULL);
    
    pc_client_connect(client, host, port, NULL);

}

void PomeloNetwork::request(const char* route, const char* msg, const char* extraData)
{
    pc_request_with_timeout(client, route, msg, (void *)extraData, TIMEOUT, requestCallback);
}

void PomeloNetwork::disconnect()
{
    if (!client) {
        return;
    }
    pc_client_rm_ev_handler(client, handler_id);
    pc_client_disconnect(client);
    pc_client_cleanup(client);
}

//callback methods
void connectCallback(pc_client_t* client, int ev_type, void* ex_data, const char* arg1, const char* arg2)
{
    
    CCLOG("connectCallback: %s\n", pc_client_ev_str(ev_type));
    
    if (PC_EV_CONNECTED == ev_type) {
        
        //std::string str = "{\"username\":\"zaza\",\"rid\":\"foo\"}";
        
        Json::Value value;
        value["username"] = User::getInstance()->nickName;
        value["rid"] = "foo";
        
        PomeloNetwork::getInstance()->request(RouteConnectorEnter, value.toStyledString().c_str(), ExtraDataConnect);
        
    }else if(PC_EV_USER_DEFINED_PUSH == ev_type){//推送
    
        Json::Reader reader;
        Json::Value value;
        
        if (strcmp(arg1, RespOnAdd) == 0) { //加入
            
            reader.parse(arg2, value);
            std::string str = value["user"].asString();
            if (str.length() > 0) {
                PreStartScene::onUpdateCompeteName(str);
            }
            
        }else if(strcmp(arg1, RespOnStart) == 0){
        
            reader.parse(arg2, value);
            Json::Value arr = value["users"];
            
            if (arr.size() > 0) {
                if (arr[0] == User::getInstance()->nickName) {
                    
                    User::getInstance()->isMain = true;
                    
                }else{

                    User::getInstance()->isMain = false;
                    
                }
                
                PreStartScene::onStartGame();
            }
        
        }else if(strcmp(arg1, RespOnMove) == 0){
        
            reader.parse(arg2, value);
            float x = value["x"].asFloat();
            float y = value["y"].asFloat();
            CCLOG("####x: %f, y: %f", x, y);
            GameScene::onUpdatePosition( x, y );
            
        }
            
    }else if(PC_EV_KICKED_BY_SERVER == ev_type){//被踢出
        
        
    
    }else{//连接错误
        
    }
    
}

void requestCallback(const pc_request_t* req, int rc, const char* resp)
{

    CCLOG("requestCallback response: %s\n", resp);


    if ( PC_RC_TIMEOUT == rc ) {//超时重连
        
    }else if( PC_RC_ERROR == rc ){//请求错误
    
    }else if( PC_RC_OK == rc ){//成功返回
    
        Json::Reader reader;
        Json::Value value;
        if (reader.parse(resp, value)) {
            
        }
    
    }
}
