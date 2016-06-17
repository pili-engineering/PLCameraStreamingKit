//
//  PLQosEnv.h
//  PLStreamReport
//
//  Created by bailong on 16/4/28.
//  Copyright © 2016年 pili. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface PLStreamingEnv : NSObject

/**
 @brief 初始化 StreamingSession 的运行环境，需要在 -application:didFinishLaunchingWithOptions: 方法下调用该方法，
 
 @warning 不调用该方法将导致 PLStreamingSession 对象无法初始化
 */
+(void)initEnv;

/**
 @brief 判断当前环境是否已经初始化
 
 @return 已初始化返回 YES，否则为 NO
 */
+(BOOL)isInited;

/**
 @brief 是否开启 QoS，默认开启
 
 @param flag 开启为 YES，否则为NO
 */
+(void)enableQos:(BOOL)flag;

/**
 @brief 是否打开测速功能，默认关闭
 
 @param flag 开启为 YES，否则为 NO
 */
+(void)enableSpeedMeasure:(BOOL)flag;

@end
