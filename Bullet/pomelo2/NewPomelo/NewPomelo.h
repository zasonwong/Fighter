//
//  aaa.h
//  pomelotest
//
//  Created by iMac on 15/3/23.
//  Copyright (c) 2015年 iMac. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class NewPomelo;

@interface NewPomelo : NSObject
+ (void)PomeloInit;
+ (void)Connection:(NSString*)host :(int) port blockcompletion:(void (^)(BOOL isConn))Connected;//定义一个block为连接完的操作。isConn为是否连接上
+ (void)DisConnection;
+ (void)Request:(NSString*)route :(NSString*)jsonStr blockcompletion:(void (^)(NSData *response))Requested;//定义一个block为连接完的操作。isConn为是否连接上

@end
