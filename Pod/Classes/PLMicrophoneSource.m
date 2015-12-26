//
//  PLMicrophoneSource.m
//  PLCameraStreamingKit
//
//  Created by 0day on 15/3/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#import <sys/utsname.h>

#import "PLMicrophoneSource.h"
//#import "PLTimeFunctions.h"

NSString *PLAudioComponentFailedToCreateNotification = @"PLAudioComponentFailedToCreateNotification";

@interface PLMicrophoneSource ()

@property (nonatomic, assign) AudioComponentInstance    componetInstance;
@property (nonatomic, assign) AudioComponent            component;
@property (nonatomic, assign) uint64_t tsBase;
@property (nonatomic, PL_STRONG) dispatch_queue_t       taskQueue;

@property (nonatomic, assign) PLAACSampleRate sampleRate;   // rewrite
@property (nonatomic, assign) NSInteger channelsPerFrame;   // rewrite
@property (nonatomic, assign) BOOL isRunning;   // rewrite

@end

static NSString* deviceName(){
    struct utsname systemInfo;
    uname(&systemInfo);
    
    return [NSString stringWithCString:systemInfo.machine
                              encoding:NSUTF8StringEncoding];
}

//@"iPad4,1" on 5th Generation iPad (iPad Air) - Wifi
//@"iPad4,2" on 5th Generation iPad (iPad Air) - Cellular
//@"iPad4,4" on 2nd Generation iPad Mini - Wifi
//@"iPad4,5" on 2nd Generation iPad Mini - Cellular
//@"iPad4,7" on 3rd Generation iPad Mini - Wifi (model A1599)
//@"iPhone7,1" on iPhone 6 Plus
//@"iPhone7,2" on iPhone 6
//@"iPhone8,1" on iPhone 6S
//@"iPhone8,2" on iPhone 6S Plus

static BOOL isNewThaniPhone6(){
    NSString *device = deviceName();
    NSLog(@"device %@", device);
    if (device == nil) {
        return NO;
    }
    NSArray *array = [device componentsSeparatedByString:@","];
    if (array.count <2) {
        return NO;
    }
    NSString *model = [array objectAtIndex:0];
    NSLog(@"model %@", model);
    if ([model hasPrefix:@"iPhone"]) {
        NSString *str1 = [model substringFromIndex:[@"iPhone" length]];
        NSUInteger num = [str1 integerValue];
        NSLog(@"num %lu", (unsigned long)num);
        if (num > 7) {
            return YES;
        }
    }
    
    if ([model hasPrefix:@"iPad"]) {
        NSString *str1 = [model substringFromIndex:[@"iPad" length]];
        NSUInteger num = [str1 integerValue];
        if (num > 4) {
            return YES;
        }
    }
    
    return NO;
}

static OSStatus handleInputBuffer(void *inRefCon,
                                  AudioUnitRenderActionFlags *ioActionFlags,
                                  const AudioTimeStamp *inTimeStamp,
                                  UInt32 inBusNumber,
                                  UInt32 inNumberFrames,
                                  AudioBufferList *ioData) {
    @autoreleasepool {
        PLMicrophoneSource *source = (__bridge PLMicrophoneSource *)inRefCon;
        
        AudioBuffer buffer;
        buffer.mData = NULL;
        buffer.mDataByteSize = 0;
        buffer.mNumberChannels = 1;
        
        AudioBufferList buffers;
        buffers.mNumberBuffers = 1;
        buffers.mBuffers[0] = buffer;
        
        OSStatus status = AudioUnitRender(source.componetInstance,
                                          ioActionFlags,
                                          inTimeStamp,
                                          inBusNumber,
                                          inNumberFrames,
                                          &buffers);
        
        if (!source.isRunning) {
            dispatch_sync(source.taskQueue, ^{
                NSLog(@"MicrophoneSource: stopRunning");
                AudioOutputUnitStop(source.componetInstance);
            });
            
            return status;
        }
        if (source.isMuted) {
            for (int i = 0; i < buffers.mNumberBuffers; i++) {
                AudioBuffer ab = buffers.mBuffers[i];
                memset(ab.mData, 0, ab.mDataByteSize);
            }
        }
        
        if(!status) {
            AudioBuffer audioBuffer = buffers.mBuffers[0];
            if ([source.delegate respondsToSelector:@selector(microphoneSource:didGetAudioBuffer:)]) {
                [source.delegate microphoneSource:source didGetAudioBuffer:&audioBuffer];
            }
        }
        return status;
    }
}

@implementation PLMicrophoneSource

- (instancetype)initWithSampleRate:(PLAACSampleRate)sampleRate channelsPerFrame:(NSInteger)cpf {
    self = [super init];
    if (self) {
        self.sampleRate = sampleRate;
        self.channelsPerFrame = cpf;
        self.isRunning = NO;
        self.muted = NO;
        self.taskQueue = dispatch_queue_create("microphone_queue", NULL);
        self.tsBase = 0;
        
        AVAudioSession *session = [AVAudioSession sharedInstance];
        [session setActive:YES withOptions:kAudioSessionSetActiveFlag_NotifyOthersOnDeactivation error:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector(handleRouteChange:)
                                                     name: AVAudioSessionRouteChangeNotification
                                                   object: session];
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector(handleInterruption:)
                                                     name: AVAudioSessionInterruptionNotification
                                                   object: session];
        [[NSNotificationCenter defaultCenter] addObserver: self
                                                 selector: @selector(handleMediaServicesWereReset:)
                                                     name: AVAudioSessionMediaServicesWereResetNotification
                                                   object: session];
        
        NSError *error = nil;
        
        [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker | AVAudioSessionCategoryOptionMixWithOthers error:nil];
        
        [session setMode:AVAudioSessionModeVideoRecording error:&error];
        
        if (![session setActive:YES error:&error]) {
            [self handleAudioComponentCreationFailure];
        }
        
        
        AudioComponentDescription acd;
        acd.componentType = kAudioUnitType_Output;
        acd.componentSubType = kAudioUnitSubType_RemoteIO;
        acd.componentManufacturer = kAudioUnitManufacturer_Apple;
        acd.componentFlags = 0;
        acd.componentFlagsMask = 0;
        
        self.component = AudioComponentFindNext(NULL, &acd);
        
        OSStatus status = noErr;
        status = AudioComponentInstanceNew(self.component, &_componetInstance);
        
        if (noErr != status) {
            [self handleAudioComponentCreationFailure];
        }
        
        UInt32 flagOne = 1;
        
        AudioUnitSetProperty(self.componetInstance, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &flagOne, sizeof(flagOne));
        
        AudioStreamBasicDescription desc = {0};
        desc.mSampleRate = self.sampleRate;
        desc.mFormatID = kAudioFormatLinearPCM;
        desc.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked;
        desc.mChannelsPerFrame = (UInt32)self.channelsPerFrame;
        desc.mFramesPerPacket = 1;
        desc.mBitsPerChannel = 16;
        desc.mBytesPerFrame = desc.mBitsPerChannel / 8 * desc.mChannelsPerFrame;
        desc.mBytesPerPacket = desc.mBytesPerFrame * desc.mFramesPerPacket;
        
        AURenderCallbackStruct cb;
        cb.inputProcRefCon = (__bridge void *)(self);
        cb.inputProc = handleInputBuffer;
        AudioUnitSetProperty(self.componetInstance, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &desc, sizeof(desc));
        AudioUnitSetProperty(self.componetInstance, kAudioOutputUnitProperty_SetInputCallback, kAudioUnitScope_Global, 1, &cb, sizeof(cb));
        
        status = AudioUnitInitialize(self.componetInstance);
        
        if (noErr != status) {
            [self handleAudioComponentCreationFailure];
        }
        
        [session setPreferredSampleRate:session.sampleRate error:nil];
        
        [session setActive:YES error:nil];
    }
    
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionRouteChangeNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionInterruptionNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVAudioSessionMediaServicesWereResetNotification object:nil];
    
    if (self.isRunning) {
        self.isRunning = NO;
        dispatch_sync(self.taskQueue, ^{
            NSLog(@"MicrophoneSource: stopRunning");
            AudioOutputUnitStop(self.componetInstance);
        });
    }
    
    dispatch_sync(self.taskQueue, ^{
        AudioComponentInstanceDispose(self.componetInstance);
        self.componetInstance = nil;
        self.component = nil;
    });
    
//    self.taskQueue = nil;
}

- (void)startRunning {
    if (self.isRunning) {
        return;
    }
    dispatch_async(self.taskQueue, ^{
        self.isRunning = YES;
        NSLog(@"MicrophoneSource: startRunning");
        AudioOutputUnitStart(self.componetInstance);
    });
}

- (void)stopRunning {
    self.isRunning = NO;
}

#pragma mark -

- (void)handleAudioComponentCreationFailure {
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:PLAudioComponentFailedToCreateNotification object:nil];
    });
}

- (void)handleMediaServicesWereReset:(NSNotification *)notification {
    //  If the media server resets for any reason, handle this notification to reconfigure audio or do any housekeeping, if necessary
    //    • No userInfo dictionary for this notification
    //      • Audio streaming objects are invalidated (zombies)
    //      • Handle this notification by fully reconfiguring audio
    NSLog(@"handleMediaServicesWereReset: %@ ",[notification name]);
}


- (void)handleInterruption:(NSNotification *)notification {
    NSInteger reason = 0;
    NSString* reasonStr=@"";
    if ([notification.name isEqualToString:AVAudioSessionInterruptionNotification]) {
        //Posted when an audio interruption occurs.
        reason = [[[notification userInfo] objectForKey:AVAudioSessionInterruptionTypeKey] integerValue];
        if (reason == AVAudioSessionInterruptionTypeBegan) {
            //       Audio has stopped, already inactive
            //       Change state of UI, etc., to reflect non-playing state
//            if(soundSessionIO_.isProcessingSound)[soundSessionIO_ stopSoundProcessing:nil];
            if (self.isRunning) {
                dispatch_sync(self.taskQueue, ^{
                    NSLog(@"MicrophoneSource: stopRunning");
                    AudioOutputUnitStop(self.componetInstance);
                });
            }
        }
        
        if (reason == AVAudioSessionInterruptionTypeEnded) {
            //       Make session active
            //       Update user interface
            //       AVAudioSessionInterruptionOptionShouldResume option
            reasonStr = @"AVAudioSessionInterruptionTypeEnded";
            NSNumber* seccondReason = [[notification userInfo] objectForKey:AVAudioSessionInterruptionOptionKey] ;
            switch ([seccondReason integerValue]) {
                case AVAudioSessionInterruptionOptionShouldResume:
                    if (self.isRunning) {
                        dispatch_async(self.taskQueue, ^{
                            NSLog(@"MicrophoneSource: stopRunning");
                            AudioOutputUnitStart(self.componetInstance);
                        });
                    }
                    //          Indicates that the audio session is active and immediately ready to be used. Your app can resume the audio operation that was interrupted.
                    break;
                default:
                    break;
            }
        }
        
        
        if ([notification.name isEqualToString:@"AVAudioSessionDidBeginInterruptionNotification"]) {
//            if (soundSessionIO_.isProcessingSound) {
//                
//            }
            //      Posted after an interruption in your audio session occurs.
            //      This notification is posted on the main thread of your app. There is no userInfo dictionary.
        }
        if ([notification.name isEqualToString:@"AVAudioSessionDidEndInterruptionNotification"]) {
            //      Posted after an interruption in your audio session ends.
            //      This notification is posted on the main thread of your app. There is no userInfo dictionary.
        }
        if ([notification.name isEqualToString:@"AVAudioSessionInputDidBecomeAvailableNotification"]) {
            //      Posted when an input to the audio session becomes available.
            //      This notification is posted on the main thread of your app. There is no userInfo dictionary.
        }
        if ([notification.name isEqualToString:@"AVAudioSessionInputDidBecomeUnavailableNotification"]) {
            //      Posted when an input to the audio session becomes unavailable.
            //      This notification is posted on the main thread of your app. There is no userInfo dictionary.
        }
        
    };
    NSLog(@"handleInterruption: %@ reason %@",[notification name], reasonStr);
}

- (void)handleRouteChange:(NSNotification *)notification {
    AVAudioSession *session = [ AVAudioSession sharedInstance ];
    NSString* seccReason = @"";
    NSInteger  reason = [[[notification userInfo] objectForKey:AVAudioSessionRouteChangeReasonKey] integerValue];
    //  AVAudioSessionRouteDescription* prevRoute = [[notification userInfo] objectForKey:AVAudioSessionRouteChangePreviousRouteKey];
    switch (reason) {
        case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
            seccReason = @"The route changed because no suitable route is now available for the specified category.";
            break;
        case AVAudioSessionRouteChangeReasonWakeFromSleep:
            seccReason = @"The route changed when the device woke up from sleep.";
            break;
        case AVAudioSessionRouteChangeReasonOverride:
            seccReason = @"The output route was overridden by the app.";
            break;
        case AVAudioSessionRouteChangeReasonCategoryChange:
            seccReason = @"The category of the session object changed.";
            break;
        case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
            seccReason = @"The previous audio output path is no longer available.";
            break;
        case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
            seccReason = @"A preferred new audio output path is now available.";
            break;
        case AVAudioSessionRouteChangeReasonUnknown:
        default:
            seccReason = @"The reason for the change is unknown.";
            break;
    }
    AVAudioSessionPortDescription *input = [[session.currentRoute.inputs count]?session.currentRoute.inputs:nil objectAtIndex:0];
    if (input.portType == AVAudioSessionPortHeadsetMic) {
        
    }
}

#pragma mark - <PLSourceAccessProtocol>

+ (PLAuthorizationStatus)deviceAuthorizationStatus {
    return (PLAuthorizationStatus)[AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
}

+ (void)requestDeviceAccessWithCompletionHandler:(void (^)(BOOL granted))handler {
    [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
        if (handler) {
            handler(granted);
        }
    }];
}

@end
