//
//  PLVideoStreamingConfiguration.h
//  PLCameraStreamingKit
//
//  Created on 15/8/4.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

#import "PLTypeDefines.h"
#import "PLMacroDefines.h"

/*!
 * PLVideoStreamingConfiguration 是推流前视频编码时的具体参数
 *
 * @discussion 当你不明白每个具体参数是什么含义前，你可以使用 defaultConfiguration 或者通过指定分辨率和推流质量生成一个 config.
 * 在设定一个参数时，请务必明白其含义，并且知道其可能造成的影响，随意的设定可能引起编码器设定失败。在 configuration 内部检查如果发现了超过
 */
@interface PLVideoStreamingConfiguration : NSObject <NSCopying>

/// 编码的 profile level
@property (nonatomic, PL_STRONG) NSString *videoProfileLevel;

/// 视频的分辨率，宽高务必设定为 2 的倍数，否则解码播放时可能出现绿边
@property (nonatomic, assign) CGSize  videoSize;

/// 视频的帧率，即 fps
@property (nonatomic, assign) NSUInteger videoFrameRate;

/// 最大关键帧间隔，可设定为 fps 的三倍，影响一个 gop 的大小
@property (nonatomic, assign) NSUInteger videoMaxKeyframeInterval;

/// 视频的码率，单位是 bps
@property (nonatomic, assign) NSUInteger videoBitRate;

/*!
 * 生成一个默认的配置
 * @discussion 该方法每次都会生成一个新的配置，不是单例方法。默认情况下，对应的参数为分辨率 (320, 480), video quality PLStreamingQualityMedium1
 */
+ (instancetype)defaultConfiguration;

/*!
 * 指定自定义分辨率和推流质量生成一个配置
 */
+ (instancetype)configurationWithVideoSize:(CGSize)videoSize
                              videoQuality:(NSString *)quality;

/*!
 * 请务必在清楚每个参数的意义及影响后再调用此方法做初始化，该方法内部会调用 validate 接口
 * 如果不能通过 validate 验证，将抛出异常
 */
- (instancetype)initWithVideoSize:(CGSize)videoSize
                   videoFrameRate:(NSUInteger)fps
         videoMaxKeyframeInterval:(NSUInteger)videoMaxKeyframeInterval
                     videoBitrate:(NSUInteger)videoBitRate
                videoProfileLevel:(NSString *)videoProfileLevel;

/*!
 * 当更改了某些属性时，调用 validate 方法验证参数是否正确，如果有错会抛出异常
 * - (instancetype)initWithVideoSize:videoFrameRate:videoMaxKeyframeInterval:videoBitrate:videoProfileLevel: 方法内部会调用该方法
 */
- (BOOL)validate;

@end

