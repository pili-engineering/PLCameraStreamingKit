//
//  PLCameraStreamingConfiguration.h
//  PLCameraStreamingKit
//
//  Created on 15/4/24.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

#import "PLTypeDefines.h"
#import "PLMacroDefines.h"

/*!
 * @deprecated
 *
 * @abstract 推流配置。
 *
 * @discussion 推流配置选用不同的参数主要影响音视频的编码参数，相应的会从效果上产生不同质量的画质和不同的流量消耗。
 */
__deprecated
@interface PLCameraStreamingConfiguration : NSObject

/// 分辨率，如果自定义了 videoSize, 该值为 PLStreamingDimension_UserDefine
@property (nonatomic, assign, readonly) PLStreamingDimension dimension;

/// 推流质量
@property (nonatomic, PL_STRONG, readonly) NSString *quality;

/// 视频的分辨率
@property (nonatomic, assign, readonly) CGSize  videoSize;

/// 视频的帧率，即 fps
@property (nonatomic, assign, readonly) NSUInteger videoFrameRate;

/// 最大关键帧间隔
@property (nonatomic, assign, readonly) NSUInteger videoMaxKeyframeInterval;

/// 编码的 profile level
@property (nonatomic, PL_STRONG, readonly) NSString *videoProfileLevel;

/// 视频的码率
@property (nonatomic, assign, readonly) NSUInteger videoBitRate;
@property (nonatomic, assign, readonly) NSUInteger audioBitRate;    // always 128kbps right now.
@property (nonatomic, assign, readonly) NSUInteger audioSampleRate; // always 44100 right now.

/*!
 * 生成一个默认的配置
 * @discussion 该方法每次都会生成一个新的配置，不是单例方法。默认情况下，对应的参数为 PLStreamingDimension_Default, PLStreamingQualityMedium1
 */
+ (instancetype)defaultConfiguration;

/*!
 * 指定可选分辨率和推流质量生成一个配置
 */
+ (instancetype)configurationWithDimension:(PLStreamingDimension)dimension
                                   quality:(NSString *)quality;

/*!
 * 指定自定义分辨率和推流质量生成一个配置
 */
+ (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize
                                             quality:(NSString *)quality;

@end


@interface PLCameraStreamingConfiguration (Deprecated)

@property (nonatomic, assign, readonly) PLStreamingNetworkType networkType DEPRECATED_ATTRIBUTE;

+ (instancetype)configurationWithDimension:(PLStreamingDimension)dimension
                                   network:(PLStreamingNetworkType)network DEPRECATED_ATTRIBUTE;

+ (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize
                                             network:(PLStreamingNetworkType)network DEPRECATED_ATTRIBUTE;

@end