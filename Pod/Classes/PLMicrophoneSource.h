//
//  PLMicrophoneSource.h
//  PLCameraStreamingKit
//
//  Created by 0day on 15/3/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreAudio/CoreAudioTypes.h>
#import <AudioToolbox/AudioToolbox.h>
#import <PLStreamingKit/PLStreamingKit.h>

#import "PLSourceAccessProtocol.h"
#import "PLAACTypeDefines.h"

@class PLMicrophoneSource;
@protocol PLMicrophoneSourceDelegate <NSObject>

- (void)microphoneSource:(PLMicrophoneSource *)source didGetAudioBuffer:(AudioBuffer *)buffer;

@end

@interface PLMicrophoneSource : NSObject
<
PLSourceAccessProtocol
>

@property (nonatomic, weak) id<PLMicrophoneSourceDelegate> delegate;
@property (nonatomic, assign, readonly) PLAACSampleRate sampleRate;
@property (nonatomic, assign, readonly) NSInteger channelsPerFrame;

@property (nonatomic, assign, getter=isMuted) BOOL muted;   // default as NO.
@property (nonatomic, readonly, assign) BOOL isRunning;

- (instancetype)initWithSampleRate:(PLAACSampleRate)sampleRate channelsPerFrame:(NSInteger)cpf;

- (void)startRunning;
- (void)stopRunning;

@end
