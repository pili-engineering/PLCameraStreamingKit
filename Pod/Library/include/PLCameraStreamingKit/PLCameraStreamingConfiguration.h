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
@property (nonatomic, PL_STRONG, readonly) NSString *quality;

@property (nonatomic, assign, readonly) CGSize  videoSize;
@property (nonatomic, assign, readonly) NSUInteger videoFrameRate;
@property (nonatomic, assign, readonly) NSUInteger videoMaxKeyframeInterval;
@property (nonatomic, PL_STRONG, readonly) NSString *videoProfileLevel;
@property (nonatomic, assign, readonly) NSUInteger videoBitRate;
@property (nonatomic, assign, readonly) NSUInteger audioBitRate;    // always 128kbps right now.
@property (nonatomic, assign, readonly) NSUInteger audioSampleRate; // always 44100 right now.

+ (instancetype)defaultConfiguration;   // PLStreamingDimension_Default, PLStreamingQualityMedium1

+ (instancetype)configurationWithDimension:(PLStreamingDimension)dimension
                                   quality:(NSString *)quality;

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