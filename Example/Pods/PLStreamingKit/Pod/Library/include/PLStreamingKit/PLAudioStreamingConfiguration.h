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
 @brief 音频编码的采样率, 默认为设备采样率
 */
@property (nonatomic, assign) PLStreamingAudioSampleRate encodedAudioSampleRate;

/**
 @brief 音频编码比特率，默认为 96kbps
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

/**
 @brief 提供一个能支持的编码采样率
 @param sampleRate 想要的编码采样率
 @return PLStreamingAudioSampleRate 如果能支持 sampleRate，则原样返回 sampleRate。如果不能支持 sampleRate，返回一个比 sampleRate 大但最接近 sampleRate 的可以被支持的编码采样率。
 */
+ (PLStreamingAudioSampleRate)mostSimilarSupportedValueWithEncodedAudioSampleRate:(NSUInteger)sampleRate;

- (instancetype)initWithAudioQuality:(NSString *)quality;

- (instancetype)initWithEncodedAudioSampleRate:(PLStreamingAudioSampleRate)sampleRate
                       encodedNumberOfChannels:(UInt32)numberOfChannels
                                  audioBitRate:(PLStreamingAudioBitRate)audioBitRate;

@end
