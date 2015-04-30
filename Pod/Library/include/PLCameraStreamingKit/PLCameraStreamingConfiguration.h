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

@interface PLCameraStreamingConfiguration : NSObject

@property (nonatomic, assign, readonly) PLStreamingDimension dimension;
@property (nonatomic, assign, readonly) PLStreamingNetworkType networkType;

@property (nonatomic, assign, readonly) CGSize  videoSize;
@property (nonatomic, assign, readonly) NSUInteger videoFrameRate;
@property (nonatomic, assign, readonly) NSUInteger videoMaxKeyframeInterval;
@property (nonatomic, PL_STRONG, readonly) NSString *videoProfileLevel;
@property (nonatomic, assign, readonly) NSUInteger videoBitRate;
@property (nonatomic, assign, readonly) NSUInteger audioBitRate;    // always 128kbps right now.
@property (nonatomic, assign, readonly) NSUInteger audioSampleRate; // always 44100 right now.

+ (instancetype)defaultConfiguration;   // PLStreamingDimension_Default, PLStreamingNetworkTypeEither

+ (instancetype)configurationWithDimension:(PLStreamingDimension)dimension
                                   network:(PLStreamingNetworkType)network;

+ (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize
                                             network:(PLStreamingNetworkType)network;

@end
