//
//  PLAudioStreamingSession.h
//  PLAudioStreamingKit
//
//  Created on 15/4/28.
//  Copyright (c) 2015年 Pili Engineering, Qiniu Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#import "PLAudioStreamingConfiguration.h"
#import "PLMacroDefines.h"
#import "PLTypeDefines.h"
#import "PLStream.h"
#import "PLBuffer.h"

// post with userinfo @{@"state": @(state)}. always posted via MainQueue.
extern NSString *PLStreamStateDidChangeNotification;
extern NSString *PLMicrophoneAuthorizationStatusDidGetNotificaiton;

@class PLAudioStreamingSession;

/*!
 * @protocol PLAudioStreamingSessionDelegate
 *
 * @discussion delegate 对象可以实现对应的方法来获取流的状态及设备授权情况。
 */
@protocol PLAudioStreamingSessionDelegate <NSObject>

@optional
- (void)audioStreamingSession:(PLAudioStreamingSession *)session streamStateDidChange:(PLStreamState)state;
- (void)audioStreamingSession:(PLAudioStreamingSession *)session didDisconnectWithError:(NSError *)error;
- (void)audioStreamingSession:(PLAudioStreamingSession *)session didGetMicrophoneAuthorizationStatus:(PLAuthorizationStatus)status;

- (void)audioStreamingSessionWillBeginBackgroundTask:(PLAudioStreamingSession *)session;
- (void)audioStreamingSession:(PLAudioStreamingSession *)session willEndBackgroundTask:(BOOL)isExpirationOccurred;

@end

/*!
 * @class PLAudioStreamingSession
 *
 * @abstract 推流中的核心类。
 *
 * @discussion 一个 PLAudioStreamingSession 实例会包含了对音频源的控制，并且对流的操作及流状态的返回都是通过它来完成的。
 */
@interface PLAudioStreamingSession : NSObject

/*!
 * @property configuration
 *
 * @abstract 音频编码信息均包含其中。
 */
@property (nonatomic, PL_STRONG) PLAudioStreamingConfiguration *configuration;  // reset will not work until startWithPushURL: invoked.

/// 流对象
@property (nonatomic, PL_STRONG) PLStream   *stream;

/// 推流 host 地址
@property (nonatomic, PL_STRONG) NSString   *rtmpPublishHost;

/*!
 * @property delegate
 *
 * @abstract 代理对象
 */
@property (nonatomic, PL_WEAK) id<PLAudioStreamingSessionDelegate> delegate;

/*!
 * @property streamState
 *
 * @abstract 流的状态，只读属性
 */
@property (nonatomic, assign, readonly) PLStreamState               streamState;

/*!
 * @property isRunning
 *
 * @abstract 是否开始推流，只读属性
 * 
 * @discussion 该状态表达的是 streamingSession 有没有开始推流。当 streamState 为 PLStreamStateConnecting 或者 PLStreamStateConnected 时, isRunning 都会为 YES，所以它为 YES 时并不表示流一定已经建立连接，其从广义上表达 streamingSession 作为客户端对象的状态。
 */
@property (nonatomic, assign, readonly) BOOL                        isRunning;

/*!
 * @property pushURL
 *
 * @abstract 推流 URL，只读属性
 */
@property (nonatomic, PL_STRONG, readonly) NSURL *pushURL;   // rtmp only now.

/*!
 * @property backgroundMode
 *
 * @abstract 音频推流的后台模式，默认为 PLAudioStreamingBackgroundModeAutoStop
 */
@property (nonatomic, assign) PLStreamingBackgroundMode    backgroundMode;

/*!
 * 初始化方法
 *
 * @param configuration 用于音频编码的配置信息
 *
 * @return PLAudioStreamingSession 实例
 */
- (instancetype)initWithConfiguration:(PLAudioStreamingConfiguration *)configuration
                               stream:(PLStream *)stream;

/*!
 * 销毁 session 的方法
 *
 * @discussion 销毁 StreamingSession 的方法，销毁前不需要调用 stop 方法。
 */
- (void)destroy;

// RTMP Operations
/*!
 * 开始推流
 *
 * @param pushURL 推流地址
 *
 * @param handler 流连接的结果会通过该回调方法返回
 *
 * @discussion 当调用过一次并且开始推流时，如果再调用该方法会直接返回不会做任何操作，尽管如此，也不要在没有断开时重复调用该方法。
 */
- (void)startWithCompleted:(void (^)(BOOL success))handler;

/*!
 * 结束推流
 */
- (void)stop;

@end

#pragma mark - Category (SendingBuffer)

@interface PLAudioStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// 最低阈值, [0..1], 不可超出这个范围, 也不可大于 highThreshold - 0.1, 默认为 0.2
@property (nonatomic, assign) CGFloat    lowThreshold;

/// 最高阈值, [0..1], 不可超出这个范围, 也不可小于 lowThreshold + 0.1, 默认为 0.8
@property (nonatomic, assign) CGFloat    highThreshold;

/// Buffer 的最大长度, 默认为 3s, 可设置范围为 [1..5]
@property (nonatomic, assign) NSTimeInterval    maxDuration;

@property (nonatomic, assign, readonly) NSTimeInterval    currentDuration;

@end

#pragma mark - Category (MicrophoneSource)

/*!
 * @category PLAudioStreamingSession(MicrophoneSource)
 *
 * @discussion 与麦克风相关的接口
 */
@interface PLAudioStreamingSession (MicrophoneSource)

@property (nonatomic, assign, getter=isMuted)   BOOL    muted;                   // default as NO.

@end

#pragma mark - Categroy (Application)

/*!
 * @category PLAudioStreamingSession(Application)
 *
 * @discussion 与系统相关的接口
 */
@interface PLAudioStreamingSession (Application)

@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end

#pragma mark - Category (Authorization)

/*!
 * @category PLAudioStreamingSession(Authorization)
 *
 * @discussion 与设备授权相关的接口
 */
@interface PLAudioStreamingSession (Authorization)

// Microphone
+ (PLAuthorizationStatus)microphoneAuthorizationStatus;
+ (void)requestMicrophoneAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

@end

#pragma mark - Category (Deprecated)

@interface PLAudioStreamingSession (Deprecated)

/*!
 * @deprecated
 * 初始化方法
 *
 * @param configuration 用于音频编码的配置信息
 *
 * @return PLAudioStreamingSession 实例
 *
 * @see - (instancetype)initWithConfiguration:(PLAudioStreamingConfiguration *)configuration stream:(PLStream *)stream;
 */
- (instancetype)initWithConfiguration:(PLAudioStreamingConfiguration *)configuration
                               stream:(PLStream *)stream
                      rtmpPublishHost:(NSString *)rtmpPublishHost DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 * 开始推流
 *
 * @param pushURL 推流地址
 *
 * @param handler 流连接的结果会通过该回调方法返回
 *
 * @discussion 当调用过一次并且开始推流时，如果再调用该方法会直接返回不会做任何操作，尽管如此，也不要在没有断开时重复调用该方法。
 *
 * @see - (void)startWithCompleted:
 */
- (void)startWithPushURL:(NSURL *)pushURL completed:(void (^)(BOOL success))handler DEPRECATED_ATTRIBUTE;

@end
