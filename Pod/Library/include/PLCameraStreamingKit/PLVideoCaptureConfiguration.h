//
//  PLVideoCaptureConfiguration.h
//  PLCaptureKit
//
//  Created by WangSiyu on 5/5/16.
//  Copyright © 2016 Pili Engineering, Qiniu Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface PLVideoCaptureConfiguration : NSObject
<
NSCopying
>

/**
 @brief 采集的视频数据的帧率，默认为 30
 */
@property (nonatomic, assign) NSUInteger videoFrameRate;

/**
 @brief 采集的视频的 sessionPreset，默认为 AVCaptureSessionPreset640x480
 */
@property (nonatomic, copy) NSString *sessionPreset;

/**
 @brief 是否需要对前置摄像头做水平镜像，默认为 YES
 */
@property (nonatomic, assign) BOOL horizontallyMirrorFrontFacingCamera;

/**
 @brief 是否需要对后置摄像头做水平镜像，默认为 NO
 */
@property (nonatomic, assign) BOOL horizontallyMirrorRearFacingCamera;

/**
 @brief 开启 camera 时的采集摄像头位置，默认为 AVCaptureDevicePositionBack
 */
@property (nonatomic, assign) AVCaptureDevicePosition position;

/**
 @brief 创建一个默认配置的 PLVideoCaptureConfiguration 实例.
 
 @return 创建的默认 PLVideoCaptureConfiguration 对象
 */
+ (instancetype)defaultConfiguration;

- (instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset horizontallyMirrorFrontFacingCamera:(BOOL)horizontallyMirrorFrontFacingCamera horizontallyMirrorRearFacingCamera:(BOOL)horizontallyMirrorRearFacingCamera cameraPosition:(AVCaptureDevicePosition)position;

@end
