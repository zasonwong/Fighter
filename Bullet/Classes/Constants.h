//
//  Constants.h
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#ifndef Constants_h
#define Constants_h

//static const std::string HOST = "192.168.4.91";
//#define PORT   4430

static const std::string GW_HOST = "192.168.90.72";
#define GW_PORT   3014

static const char * ExtraDataGateway = "gate_way";//网关
static const char * ExtraDataConnect = "connect";//连接
static const char * ExtraDataMove = "move";

static const char * RespOnAdd = "onAdd";//加入
static const char * RespOnStart = "onStart";//开始
static const char * RespOnMove = "onMove";//开始

static const char * RouteGateEnter = "gate.gateHandler.queryEntry";
static const char * RouteConnectorEnter = "connector.entryHandler.enter";
static const char * RouteMove = "chat.chatHandler.move";

#define Title_Label_Font_Size   20

#define Sub_Title_Label_Font_Size   18

#define Button_Label_Font_Size   16

#define Normal_Font_Size   14

#define Small_Font_Size   12

#define Tiny_Font_Size   10

#define Font_Padding  5

#define Padding  30

#define COUNT_DOWN  1

#define TIMEOUT  20 

#define VisibleSize (Director::getInstance()->getVisibleSize())
#define VisibleOrigin (Director::getInstance()->getVisibleOrigin())




#endif /* Constants_h */
