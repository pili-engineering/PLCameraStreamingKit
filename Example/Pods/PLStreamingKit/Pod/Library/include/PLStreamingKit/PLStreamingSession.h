//
//  PLStreamingSession.h
//  PLStreamingKit
//
//  Created by 0dayZh on 15/10/28.
//  Copyright © 2015年 pili-engineering. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "PLVideoStreamingConfiguration.h"
#import "PLAudioStreamingConfiguration.h"
#import "PLMacroDefines.h"
#import "PLTypeDefines.h"
#import "PLStream.h"
#import "PLBuffer.h"

@class PLStreamingSession;
@class QNDnsManager;

@protocol PLStreamingSessionDelegate <NSObject>

@optional
/// @abstract 流状态已变更的回调
- (void)streamingSession:(PLStreamingSession *)session streamStateDidChange:(PLStreamState)state;

/// @abstract 因产生了某个 error 而断开时的回调
- (void)streamingSession:(PLStreamingSession *)session didDisconnectWithError:(NSError *)error;

@end

@interface PLStreamingSession : NSObject

/// 视频编码及推流配置
@property (nonatomic, PL_STRONG) PLVideoStreamingConfiguration  *videoConfiguration;

/// 音频编码及推流配置
@property (nonatomic, PL_STRONG) PLAudioStreamingConfiguration  *audioConfiguration;

/// 流对象
@property (nonatomic, PL_STRONG) PLStream   *stream;

/// 代理对象
@property (nonatomic, PL_WEAK) id<PLStreamingSessionDelegate> delegate;

/// 流的状态，只读属性
@property (nonatomic, assign, readonly) PLStreamState               streamState;

/*!
 * 是否开始推流，只读属性
 *
 * @discussion 该状态表达的是 streamingSession 有没有开始推流。当 streamState 为 PLStreamStateConnecting 或者 PLStreamStateConnected 时, isRunning 都会为 YES，所以它为 YES 时并不表示流一定已经建立连接，其从广义上表达 streamingSession 作为客户端对象的状态。
 */
@property (nonatomic, assign, readonly) BOOL                        isRunning;

/// 推流 URL，只读属性
@property (nonatomic, PL_STRONG, readonly) NSURL *pushURL;   // rtmp only now.

/*!
 * 初始化方法
 *
 * @param videoConfiguration 视频编码及推流的配置信息
 *
 * @param audioConfiguration 音频编码及推流的配置信息
 *
 * @param stream Stream 对象
 *
 * @return PLCameraStreamingSession 实例
 *
 * @discussion 初始化方法会优先使用后置摄像头，如果发现设备没有后置摄像头，会判断是否有前置摄像头，如果都没有，便会返回 nil。
 */
- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration
                        audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration
                                    stream:(PLStream *)stream;

/*!
 * 初始化方法
 *
 * @param videoConfiguration 视频编码及推流的配置信息
 *
 * @param audioConfiguration 音频编码及推流的配置信息
 *
 * @param stream Stream 对象
 *
 * @param dns dnsmanager, 自定义dns查询，使用happydns
 *
 * @return PLCameraStreamingSession 实例
 *
 * @discussion 初始化方法会优先使用后置摄像头，如果发现设备没有后置摄像头，会判断是否有前置摄像头，如果都没有，便会返回 nil。
 */
- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration
                        audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration
                                    stream:(PLStream *)stream
                                       dns:(QNDnsManager*)dns;


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

- (void)beginUpdateConfiguration;
- (void)endUpdateConfiguration;

// 处理发送数据
/*!
 * 处理视频数据
 */
- (void)pushVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

- (void)pushPixelBuffer:(CVPixelBufferRef)pixelBuffer;

/*!
 * 处理音频数据
 */
- (void)pushAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

- (void)pushAudioBuffer:(AudioBuffer *)audioBuffer;

@end

#pragma mark - Category (SendingBuffer)

@interface PLStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// 最低阈值, [0..1], 不可超出这个范围, 默认为 0.5
@property (nonatomic, assign) CGFloat    lowThreshold;

/// 最高阈值, [0..1], 不可超出这个范围, 默认为 1
@property (nonatomic, assign) CGFloat    highThreshold;

/// 默认为 300
@property (nonatomic, assign) NSUInteger    maxCount;
@property (nonatomic, assign, readonly) NSUInteger    currentCount;

@end

#pragma mark - Categroy (Application)

/*!
 * @category PLCameraStreamingSession(Application)
 *
 * @discussion 与系统相关的接口
 */
@interface PLStreamingSession (Application)

@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end