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

@property (nonatomic, assign, readonly) PLStreamingAudioBitRate audioBitRate;
@property (nonatomic, assign, readonly) NSUInteger encodedAudioSampleRate; // 48000 for iPhone 6s family devices, 44100 for others
@property (nonatomic, assign) UInt32    encodedNumberOfChannels;   // default as 1

/// kPLAudioStreamingQualityHigh2 as default
+ (instancetype)defaultConfiguration;

/*!
 * 指定可选分辨率和推流质量生成一个配置
 */
+ (instancetype)configurationWithAudioQuality:(NSString *)quality;

@end
