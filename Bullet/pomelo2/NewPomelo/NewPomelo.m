//
//  aaa.m
//  pomelotest
//
//  Created by iMac on 15/3/23.
//  Copyright (c) 2015年 iMac. All rights reserved.
//

#include "opensslv.h"

#include "NewPomelo.h"
#include "pomelo.h"


#define REQ_ROUTE "connector.entryHandler.entry"
#define REQ_MSG "{\"name\": \"test\"}"
#define REQ_EX ((void*)0x22)
#define REQ_TIMEOUT 10

#define NOTI_ROUTE "test.testHandler.notify"
#define NOTI_MSG "{\"content\": \"test content\"}"
#define NOTI_EX ((void*)0x33)
#define NOTI_TIMEOUT 30

#define EV_HANDLER_EX ((void*)0x44)



@interface NewPomelo ()


@end




@implementation NewPomelo

//连接客户端
static pc_client_t* client;
/**
 * 超时事件限制（当前设置为10，可以根据需求更改）
 */
static int TIMEOUT=10;



//监听时间ID
static int handler_id;

//    //内网--网关
//    public static String GATE_WAY_HOST="192.168.16.200";
//    //内网--账号服务器
//    public static String AUTH_HOST="192.168.16.200";


//    外网--网关
//    public static String GATE_WAY_HOST="218.244.151.40";
//外网--账号服务器
//    public static String AUTH_HOST="114.215.171.69";


static int LAST_PORT=0;

static NSString *LAST_HOST=@"";

static BOOL Request_TIMEOUT=false;

static BOOL PC_CONNECT=true;



static NSTimer *timer;

//RESPONSE保存
 static NSData *respData;
//路由保存
static NSString *ROUTE=@"";
//发送的Json保存
static NSString *JSON=@"";

//初始化Pomelo连接
+ (void)PomeloInit
{
    pc_lib_init(NULL, NULL, NULL, NULL);
};



/**
* POMELO连接
* 网关 8002 （GAT）
* 账号 8001 （AUTH）
* 192.168.16.200:8080
*/
+ (void)Connection:(NSString*)host :(int) port blockcompletion:(void (^)(BOOL isConn))Connected
{
    
    LAST_PORT = port;
    LAST_HOST = host;
    
    client = (pc_client_t*)malloc(pc_client_size());
    
    pc_client_config_t config = PC_CLIENT_CONFIG_DEFAULT;
    config.transport_name = PC_TR_NAME_UV_TLS;
    config.local_storage_cb = local_storage_cb;
    pc_client_init(client,  NULL, &config);
    
    handler_id = pc_client_add_ev_handler(client, event_cb, NULL, NULL);
    
    const char *h = [host UTF8String];
    
    pc_client_connect(client, h, port, NULL);
   
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.5 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        Connected(PC_CONNECT);
    });

};


//断开连接
+ (void)DisConnection
{
    //移除监听
    pc_client_rm_ev_handler(client, handler_id);
    //断开连接
    pc_client_disconnect(client);
}
//Connect连接完后监听事件监听连接状态并做操作
static void event_cb(pc_client_t* client, int ev_type, void* ex_data, const char* arg1, const char* arg2)
{
    
    printf("Get Event: %s\n", pc_client_ev_str(ev_type));
   
    //连接中
    if (ev_type == 0){
        //连接中,返回代码ing
        
        PC_CONNECT=YES;
    }
    
    NSString* evstr = [NSString stringWithUTF8String:pc_client_ev_str(ev_type)];
    //CONNECTED 被连接上一连上
    if(ev_type == 1&&[evstr isEqual: @"PC_EV_CONNECTED"]){
        if (!PC_CONNECT) {
            PC_CONNECT=YES;
            NSLog(@"POMELO长连接重连重连重连重连重连重连重连");
            pc_request_with_timeout(client, [ROUTE UTF8String], [JSON UTF8String], NULL, TIMEOUT, request_cb);
//            NSLog(@"重连重连重连重连重连重连重连");
        }else{
//            pc_request_with_timeout(client, [ROUTE UTF8String], [JSON UTF8String], NULL, TIMEOUT, request_cb);
//            NSLog(@"重连重连重连重连重连重连重连");
            NSLog(@"POMELO长连接正常连接");
        }
        
    }
    //异常断开
    if (ev_type == 6&&[evstr  isEqual: @"PC_EV_UNEXPECTED_DISCONNECT"]){
        PC_CONNECT = false;
        
    };
    //收到推送
    if (ev_type == 0&&[evstr  isEqual: @"PC_EV_USER_DEFINED_PUSH"]){
        PC_CONNECT = false;
        NSLog(@"pomelo收到推送");
        //发送广播》传值
        NSString *routeArg = [NSString stringWithCString:arg1 encoding:NSUTF8StringEncoding];
        NSString *msgArg = [NSString stringWithCString:arg2 encoding:NSUTF8StringEncoding];
        if ([routeArg  isEqual: @"onKicked"]) {
            [[NSNotificationCenter defaultCenter]postNotificationName:@"pomelokick" object:msgArg];
            
        }else{
            [[NSNotificationCenter defaultCenter]postNotificationName:@"pomelopush" object:msgArg];
        }
        
        
//        if (strcmp(arg1, "onNewMsg") == 0) {
//            
//        }
        
        
    };
    
    
}



// Pomelo 请求方法
// @param route 路由 "connector.entryHandler.longconn"
// @param JSONStr 参数
+ (void)Request:(NSString*)route :(NSString*)jsonStr blockcompletion:(void (^)(NSData *response))Requested;
{
    ROUTE = route;
    JSON = jsonStr;
    if (client==NULL ||[route isEqual:@""]||[jsonStr isEqual:@""]) {
        pc_lib_cleanup();
        pc_lib_init(NULL, NULL, NULL, NULL);
       
    }else{
        pc_request_with_timeout(client, [route UTF8String], [jsonStr UTF8String], NULL, TIMEOUT, request_cb);
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.3 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
            Requested(respData);
        });
    }
    
   
   
    
};



// 请求完后 对Request返回的rc状态码以及response做操作
static void request_cb(const pc_request_t* req, int rc, const char* resp)
{
   
    printf("Get Response: %s\n", resp);
    
    NSDictionary *defulOB ;

    
    
    //re 为pomelo返回的code
    switch (rc) {
        case PC_RC_TIMEOUT:
            if (!Request_TIMEOUT) {
                pc_request_with_timeout(client, [ROUTE UTF8String], [JSON UTF8String], NULL, TIMEOUT, request_cb);
                Request_TIMEOUT=true;
            }else{
                Request_TIMEOUT=false;
                [defulOB setValue:[NSNumber numberWithInt:rc] forKey:@"code"];
                respData = [NSKeyedArchiver archivedDataWithRootObject:defulOB];
            }
            break;
        case PC_RC_ERROR:
            Request_TIMEOUT=false;
            [defulOB setValue:[NSNumber numberWithInt:rc] forKey:@"code"];
            respData = [NSKeyedArchiver archivedDataWithRootObject:defulOB];
            break;
        case PC_RC_OK:
            Request_TIMEOUT=false;
            //把成功的值返回给前端
            respData = [NSData dataWithBytes:resp length:strlen(resp)];
            break;
        default:
            break;
    }
    fflush(stdout);
    
    
}

static int local_storage_cb(pc_local_storage_op_t op, char* data, size_t* len, void* ex_data)
{
    // 0 - success, -1 - fail
    char buf[1024];
    size_t length;
    size_t offset;
    FILE* f;
    
    if (op == PC_LOCAL_STORAGE_OP_WRITE) {
        f = fopen("pomelo.dat", "w");
        printf("wwwwwww");
        if (!f) {
            return -1;
        }
        fwrite(data, 1, *len, f);
        fclose(f);
        return 0;
        
    } else {
        f = fopen("pomelo.dat", "r");
        printf("rrrrrrr");
        if (!f) {
            *len = 0;
            return -1;
        }
        *len = 0;
        offset = 0;
        
        while((length = fread(buf, 1, 1024, f))) {
            *len += length;
            if (data) {
                memcpy(data + offset, buf, length);
            }
            offset += length;
        }
        
        fclose(f);
        
        return 0;
    }
}



//+ (void)Connection:(NSString*)parameters
//{
//    
//    
//    pc_client_config_t config = PC_CLIENT_CONFIG_DEFAULT;
//    
//    pc_lib_init(NULL, NULL, NULL, NULL);
//    
//    config.transport_name = PC_TR_NAME_UV_TLS;
//    config.local_storage_cb = local_storage_cb;
//    
//    
//    client = (pc_client_t*)malloc(pc_client_size());
//    
//    pc_client_init(client,  NULL, &config);
//    
//    
//    
//    
//    handler_id = pc_client_add_ev_handler(client, event_cb, NULL, NULL);
//    
//    pc_client_connect(client, "192.168.16.200", 8080, NULL);
//    
//    
//    const char * a =[parameters UTF8String];
//    pc_request_with_timeout(client, "connector.entryHandler.longconn", a, NULL, 5, request_cb);
//    
//}


@end






