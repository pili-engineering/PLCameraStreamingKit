//
//  PLSourceAccessProtocol.h
//  PLCameraStreamingKit
//
//  Created by 0day on 15/3/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#import <Foundation/Foundation.h>

/// 设备授权状态
typedef NS_ENUM(NSUInteger, PLAuthorizationStatus) {
    /// 还没有确定是否授权
    PLAuthorizationStatusNotDetermined = 0,
    /// 设备受限，一般在家长模式下设备会受限
    PLAuthorizationStatusRestricted,
    /// 拒绝授权
    PLAuthorizationStatusDenied,
    /// 已授权
    PLAuthorizationStatusAuthorized
};

@protocol PLSourceAccessProtocol <NSObject>

@required
+ (PLAuthorizationStatus)deviceAuthorizationStatus;
+ (void)requestDeviceAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

@end
