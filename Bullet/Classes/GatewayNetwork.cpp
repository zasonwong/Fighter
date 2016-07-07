//
//  GatewayNetwork.cpp
//  Bullet
//
//  Created by zason on 7/6/16.
//
//

#include "GatewayNetwork.hpp"
#include "Constants.h"
#include "json.h"
#include "PomeloNetwork.hpp"

static int handler_id;

static std::string sConnectHost;
static int sConnectPort;

static pc_client_t* client;

static void connectCallback(pc_client_t* client, int ev_type, void* ex_data, const char* arg1, const char* arg2);
static void requestCallback(const pc_request_t* req, int rc, const char* resp);

GatewayNetwork::GatewayNetwork()
{
    
}

GatewayNetwork::~GatewayNetwork()
{
    disconnect();
    pc_lib_cleanup();
}

GatewayNetwork * GatewayNetwork::getInstance()
{
    
    static GatewayNetwork * instance = nullptr;
    
    if (!instance)
    {
        instance = new (std::nothrow) GatewayNetwork();
        instance->init();
    }
    
    return instance;
    
}

void GatewayNetwork::init()
{
    pc_lib_init(NULL, NULL, NULL, NULL);
}

void GatewayNetwork::connect(const char* host, int port, const char* extraData)
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

void GatewayNetwork::disconnect()
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
        
        pc_request_with_timeout(client, "gate.gateHandler.queryEntry", "{\"uid\":\"zaza\"}", ex_data, TIMEOUT, requestCallback);
        
    }else if(PC_EV_USER_DEFINED_PUSH == ev_type){//推送
        
        
        
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
            
            if (value["code"].asInt() == 200) {

                sConnectHost = value["host"].asString();
                sConnectPort = value["port"].asInt();
                
                if (sConnectHost.length() > 0) {
                    
                    GatewayNetwork::getInstance()->disconnect();
                    PomeloNetwork::getInstance()->connect(sConnectHost.c_str(), sConnectPort, nullptr);
                    
                }
                
            }
            
        }
        
    }
}
