//
//  PLAudioCaptureConfiguration.h
//  PLCaptureKit
//
//  Created by WangSiyu on 5/5/16.
//  Copyright © 2016 Pili Engineering, Qiniu Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PLAudioCaptureConfiguration : NSObject

/**
 @brief 采集音频数据的声道数，默认为 1
 
 @warning 并非所有采集设备都支持多声道数据的采集，可以通过检查 [AVAudioSession sharedInstance].maximumInputNumberOfChannels 得到当前采集设备支持的最大声道数
 */
@property (nonatomic, assign) NSUInteger channelsPerFrame;

/**
 @brief 创建一个默认配置的 PLAudioCaptureConfiguration 实例.
  
 @return 创建的默认 PLAudioCaptureConfiguration 对象
 */
+ (instancetype)defaultConfiguration;

@end
