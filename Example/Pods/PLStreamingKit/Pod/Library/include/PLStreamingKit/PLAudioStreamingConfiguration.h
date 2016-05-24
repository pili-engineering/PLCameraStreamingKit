//
//  PLAudioStreamingConfiguration.h
//  PLCameraStreamingKit
//
//  Created on 15/8/4.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "PLTypeDefines.h"

/*!
    @class      PLAudioStreamingConfiguration
    @abstract   音频编码推流的相关配置

    @since      v1.0.0
 */
@interface PLAudioStreamingConfiguration : NSObject <NSCopying>

/**
 @brief 音频编码的采样率，为避免iPhone 6s系列的电流音问题，该采样率当前版本默认采用 48000 HZ，并且在初始化 PLCameraStreamingSession 之后硬件采样率会被设置为 48000 HZ，因此需要设置音频采集的采样率为 48000。
 */
@property (nonatomic, assign, readonly) NSUInteger encodedAudioSampleRate;

/**
 @brief 音频编码比特率
 */
@property (nonatomic, assign) PLStreamingAudioBitRate audioBitRate;

/**
 @brief 编码的声道数，默认为1
 */
@property (nonatomic, assign) UInt32    encodedNumberOfChannels;   // default as 1

/// kPLAudioStreamingQualityHigh2 as default
+ (instancetype)defaultConfiguration;

/*!
 * 指定可选分辨率和推流质量生成一个配置
 */
+ (instancetype)configurationWithAudioQuality:(NSString *)quality;

@end
