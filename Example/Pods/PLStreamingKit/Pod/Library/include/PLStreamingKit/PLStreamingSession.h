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
#import "PLStreamStatus.h"

@class PLStreamingSession;
@class QNDnsManager;

/*!
 * @protocol PLStreamingSessionDelegate
 *
 * PLStreamingSession 实例的代理对象必须实现 PLStreamingSessionDelegate 协议。
 *
 * @since v1.0.0
 */
@protocol PLStreamingSessionDelegate <NSObject>

@optional
/*!
 * @method -streamingSession:streamStateDidChange:
 *
 * @abstract 告知代理对象流的状态已经变更。
 *
 * @discussion 该代理方法只会在以下几种状态时触发
 *      <ul>
 *      <li>PLStreamStateConnecting,</li>
 *      <li>PLStreamStateConnected,</li>
 *      <li>PLStreamStateDisconnecting,</li>
 *      <li>PLStreamStateDisconnected</li>
 *      </ul>
 * 该代理方法的几种回调状态都是在正常操作时产生的，也就是主动触发的，对于意外状态，不在该方法中处理，
 * PLStreamStateError 状态便不会触发该回调。
 *
 * @param session 调用该代理方法的 PLStreamingSession 对象
 * @param state 已变更的状态
 *
 * @warning PLStreamStateError 状态不会触发该代理方法，而是触发 -streamingSession:didDisconnectWithError: 代理方法
 *
 * @see -streamingSession:didDisconnectWithError:
 * @see PLStreamState
 *
 * @since v1.0.0
 */
- (void)streamingSession:(PLStreamingSession *)session streamStateDidChange:(PLStreamState)state;

/*!
 * @method -streamingSession:didDisconnectWithError:
 *
 * @abstract 告知代理对象流意外断开。
 *
 * @discussion 该代理方法只会在预期外的 PLStreamStateError 状态时触发回调，其他在预期内主动触发的状态不会触发该代理方法。
 *
 * @param session 调用该代理方法的 PLStreamingSession 对象
 * @param error 引起意外断流的错误信息
 *
 * @warning 只有 PLStreamStateError 状态会触发该代理方法。
 *
 * @see -streamingSession:streamStateDidChange:
 * @see PLStreamError
 *
 * @since v1.0.0
 */
- (void)streamingSession:(PLStreamingSession *)session didDisconnectWithError:(NSError *)error;

/*!
 * @method -streamingSession:streamStatusDidUpdate:
 *
 * @abstract 每隔一段时间告知代理对象流在这段时间内的流状态。
 *
 * @discussion 该代理方法在开始推流后启动触发，停止推流时停止触发。默认是每 3s 调用一次，可以通过更改 statusUpdateInterval 属性来变更触发频率。
 *
 * @param session 调用该代理方法的 PLStreamingSession 对象
 * @param statu 引起意外断流的错误信息
 *
 * @see statusUpdateInterval
 * @see PLStreamStatus
 *
 * @since v1.1.1
 */
- (void)streamingSession:(PLStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status;

@end

/*!
 * @class PLStreamingSession
 *
 * @abstract PLStreamingSession 是 PLStreamingKit 的核心类。
 *
 * @discussion PLStreamingSession 对象负责音视频编码控制、网络连接控制以及各类状态的监控和向代理反馈。它的生命周期从 init 开始，destroy 结束。
 * init 时传递视频的 configuration, 音频的 configuration 以及推流对应的 stream 对象。<br>
 *
 * 同时，为了更好的提供对 dns 域名解析的容错，PLStreamingKit 以 HappyDNS 做为依赖，当然你可以不去关心这部分，如果有定制 dns 解析的需求，也可以
 * 通过 init 方法传递自定义的 HappyDNS QNDnsManager 对象。<br>
 *
 * 在一个 PLStreamingSession 对象的生命周期内，通过调用 -startWithCompleted: 方法开始调用，-stop 方法结束推流。只要一个流没有在服务端做 disable 操作，
 * 都可以在 -stop 后再调用 -startWithCompleted: 方法重新推流。<br>
 *
 * 你只要实现了 PLStreamingSessionDelegate 对应的代理方法，就可以获取到推流的状态变更及异常状态的回调，同时还有定时回调的流状态信息反馈。
 *
 * @see PLStreamingSessionDelegate
 * @see PLVideoStreamingConfiguration
 * @see PLAudioStreamingConfiguration
 * @see PLStream
 *
 * @since v1.0.0
 */
@interface PLStreamingSession : NSObject

/*!
 * @property videoConfiguration
 */
/// 视频编码及推流配置
@property (nonatomic, copy) PLVideoStreamingConfiguration  *videoConfiguration;

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

/// 默认为 3s，可设置范围为 [1..30] 秒
@property (nonatomic, assign) NSTimeInterval    statusUpdateInterval;

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

- (void)reloadVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration;

// 处理发送数据
/*!
 * 处理视频数据
 */
- (void)pushVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;
/// 注意: completion 回调不在主线程
- (void)pushVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer completion:(void (^)(void))handler;

- (void)pushPixelBuffer:(CVPixelBufferRef)pixelBuffer;
/// 注意: completion 回调不在主线程
- (void)pushPixelBuffer:(CVPixelBufferRef)pixelBuffer completion:(void (^)(void))handler;

/*!
 * 处理音频数据
 */
- (void)pushAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;
/// 注意: completion 回调不在主线程
- (void)pushAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer completion:(void (^)(void))handler;

- (void)pushAudioBuffer:(AudioBuffer *)audioBuffer;
/// 注意: completion 回调不在主线程
- (void)pushAudioBuffer:(AudioBuffer *)audioBuffer completion:(void (^)(void))handler;

@end

@interface PLStreamingSession (Network)

/// 接收超时，默认为 15s, 设定最小数值不得低于 3s，否则不变更
@property (nonatomic, assign) int   receiveTimeout;

/// 发送超时，默认为 3s, 设定最小数值不得低于 3s，否则不变更
@property (nonatomic, assign) int   sendTimeout;

@end

#pragma mark - Category (SendingBuffer)

@interface PLStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// [0..1], 不可超出这个范围, 默认为 0.5
@property (nonatomic, assign) CGFloat threshold;

/// 默认为 300
@property (nonatomic, assign) NSUInteger    maxCount;
@property (nonatomic, assign, readonly) NSUInteger    currentCount;

/// 已弃用，请使用 threshold 属性
@property (nonatomic, assign) CGFloat    lowThreshold DEPRECATED_ATTRIBUTE;

/// 已弃用，请使用 threshold 属性
@property (nonatomic, assign) CGFloat    highThreshold DEPRECATED_ATTRIBUTE;

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

#pragma mark - Category (Info)

@interface PLStreamingSession (Info)

+ (NSString *)versionInfo;

@end