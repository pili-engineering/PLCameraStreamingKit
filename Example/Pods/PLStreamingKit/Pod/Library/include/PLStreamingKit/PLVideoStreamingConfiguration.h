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
    @class      PLVideoStreamingConfiguration
    @abstract   视频编码和推流相关的配置。

    @discussion 当你不明白每个具体参数是什么含义前，你可以使用 defaultConfiguration 或者通过指定分辨率和推流质量生成一个 config.
                在设定一个参数时，请务必明白其含义，并且知道其可能造成的影响，随意的设定可能引起编码器设定失败。
 
    @since      v1.0.0
 */
@interface PLVideoStreamingConfiguration : NSObject <NSCopying>

/*!
    @property   videoProfileLevel
    @abstract   H.264 编码时使用的 Profile Level。

    @discussion 默认情况下使用 AVVideoProfileLevelH264Baseline31，如果对于视频编码有额外的需求并且知晓该参数带来的影响可以自行更改。

    @warning    当你不清楚该参数更改对分辨率要求，码率等影响时，请不要随意更改。

    @since      v1.0.0
 */
@property (nonatomic, copy) NSString *videoProfileLevel;

/*!
    @property   videoSize
    @abstract   编码时的视频分辨率。

    @discussion 需要注意的是，这个参数影响的是视频编码时的分辨率，而非摄像头采集到数据的预览大小，传递给编码器的图像尺寸与此尺寸不同时，会按照 AVVideoScalingModeResizeAspectFill
                对图像做剪切，从而确保图像不会出现压缩的现象。

    @since      v1.0.0
 */
@property (nonatomic, assign) CGSize  videoSize;

/*!
    @property   expectedSourceVideoFrameRate
    @abstract   预期视频的帧率。

    @discussion 预期视频的编码帧率，这个数值对编码器的来说并不是直接限定了 fps, 而是给编码器一个预期的视频帧率，最终编码的视频帧率，是由实际输入的数据决定的。

    @since      v1.2.0
 */
@property (nonatomic, assign) NSUInteger expectedSourceVideoFrameRate;

/*!
    @property   videoFrameRate
    @abstract   视频的帧率。

    @discussion 视频的编码帧率，从编码器角度，最终决定帧率的是由输入源决定的，所以对于编码器只存在预期的视频帧率，从 v1.2.0 开始该参数弃用，请使用 expectedSourceVideoFrameRate。

    @see        expectedSourceVideoFrameRate
 
    @since      v1.0.0
    @deprecated v1.2.0
 */
@property (nonatomic, assign) NSUInteger videoFrameRate DEPRECATED_ATTRIBUTE;

/*!
    @property   videoMaxKeyframeInterval
    @abstract   视频编码关键帧最大间隔（GOP）。

    @discussion h.264 编码时，两个关键帧之间间隔的最多的帧数，一般为 fps 的两倍或者三倍。

    @since      v1.0.0
 */
@property (nonatomic, assign) NSUInteger videoMaxKeyframeInterval;

/*!
    @property   averageVideoBitRate
    @abstract   平均视频编码码率。

    @discussion 单位为 bps(Bits per Second)。该参数的视频编码实际过程中，并不是恒定的数值，所以只能设定平均视频编码码率。
 
    @since      v1.2.0
 */
@property (nonatomic, assign) NSUInteger averageVideoBitRate;

/*!
    @property   videoBitRate
    @abstract   视频编码码率。

    @warning 已作废，请使用 averageVideoBitRate

    @since      v1.0.0
    @deprecated
 */
@property (nonatomic, assign) NSUInteger videoBitRate DEPRECATED_ATTRIBUTE;

/*!
    @method     defaultConfiguration
    @abstract   生成一个默认的视频编码推流配置对象。
    
    @discussion 该方法每次都会生成一个新的配置，这不是单例方法。默认情况下，对应的参数为分辨率 (320, 480), video quality PLStreamingQualityMedium1
    
    @see        configurationWithVideoSize:videoQuality:
    @see        initWithVideoSize:expectedSourceVideoFrameRate:videoMaxKeyframeInterval:averageVideoBitRate:videoProfileLevel:
 
    @since      v1.0.0
 */
+ (instancetype)defaultConfiguration;

/*!
    @method     configurationWithVideoSize:videoQuality:
    @abstract   指定自定义分辨率和推流质量生成一个配置。

    @param      videoSize 编码分辨率
    @param      quality 编码质量
 
    @discussion 该方法每次都会生成一个新的配置，这不是单例方法。

    @see        configurationWithVideoSize:videoQuality:
    @see        initWithVideoSize:expectedSourceVideoFrameRate:videoMaxKeyframeInterval:averageVideoBitRate:videoProfileLevel:

    @since      v1.0.0
 */
+ (instancetype)configurationWithVideoSize:(CGSize)videoSize
                              videoQuality:(NSString *)quality;

/*!
    @method     initWithVideoSize:expectedSourceVideoFrameRate:videoMaxKeyframeInterval:averageVideoBitRate:videoProfileLevel:
    @abstract   初始化一个 PLVideoStreamingConfiguration 对象。

    @param      videoSize 编码分辨率
    @param      expectedSourceVideoFrameRate 预期采集源视频码率
    @param      videoMaxKeyframeInterval 视频最大关键帧间隔
    @param      averageVideoBitRate 平均视频码率
    @param      videoProfileLevel H.264 编码时的 profile level

    @warning    如果指定的参数不合理，在 -validate 时失败，会抛出异常。
 
    @see        defaultConfiguration
    @see        configurationWithVideoSize:videoQuality:

    @since      v1.2.0
 */
- (instancetype)initWithVideoSize:(CGSize)videoSize
     expectedSourceVideoFrameRate:(NSUInteger)expectedSourceVideoFrameRate
         videoMaxKeyframeInterval:(NSUInteger)videoMaxKeyframeInterval
              averageVideoBitRate:(NSUInteger)averageVideoBitRate
                videoProfileLevel:(NSString *)videoProfileLevel;

/*!
    @method     initWithVideoSize:videoFrameRate:videoMaxKeyframeInterval:videoBitrate:videoProfileLevel:
    @abstract   初始化一个 PLVideoStreamingConfiguration 对象。

    @discussion 该方法已废弃，请使用 -initWithVideoSize:expectedSourceVideoFrameRate:videoMaxKeyframeInterval:averageVideoBitRate:videoProfileLevel:
                方法初始化。

    @see        initWithVideoSize:expectedSourceVideoFrameRate:videoMaxKeyframeInterval:averageVideoBitRate:videoProfileLevel:
    @see        defaultConfiguration
    @see        configurationWithVideoSize:videoQuality:

    @since      v1.0.0
    @deprecated v1.2.0
 */
- (instancetype)initWithVideoSize:(CGSize)videoSize
                   videoFrameRate:(NSUInteger)fps
         videoMaxKeyframeInterval:(NSUInteger)videoMaxKeyframeInterval
                     videoBitrate:(NSUInteger)videoBitRate
                videoProfileLevel:(NSString *)videoProfileLevel DEPRECATED_ATTRIBUTE;

/*!
    @method     validate
    @abstract   校验 PLVideoStreamingConfiguration 是否有设置会导致编码失败，如果有，该方法会抛出异常。

    @since      v1.0.0
 */
- (BOOL)validate;

@end

