//
//  PLStreamingSession.h
//  PLStreamingKit
//
//  Created by 0dayZh on 15/10/28.
//  Copyright © 2015年 pili-engineering. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "PLMacroDefines.h"
#import "PLTypeDefines.h"

@class PLStreamingSession;
@class PLVideoStreamingConfiguration;
@class PLAudioStreamingConfiguration;
@class PLStream;
@class PLStreamStatus;
@class QNDnsManager;

@protocol PLStreamingSendingBufferDelegate;

/*!
    @protocol    PLStreamingSessionDelegate
    @abstract    PLStreamingSession 实例的代理对象必须实现 PLStreamingSessionDelegate 协议。

    @since       v1.0.0
 */
@protocol PLStreamingSessionDelegate <NSObject>

@optional
/*!
    @method      streamingSession:streamStateDidChange:
    @abstract    告知代理对象流的状态已经变更。

    @param       session 调用该代理方法的 PLStreamingSession 对象
    @param       state 已变更的状态

    @discussion  该代理方法只会在以下几种状态时触发
    <ul>
    <li>         PLStreamStateConnecting
    <li>         PLStreamStateConnected
    <li>         PLStreamStateDisconnecting
    <li>         PLStreamStateDisconnected
    </ul>
                 该代理方法的几种回调状态都是在正常操作时产生的，也就是主动触发的，对于意外状态，不在该方法中处理，
                 PLStreamStateError 状态便不会触发该回调。

    @warning     PLStreamStateError 状态不会触发该代理方法，而是触发 -streamingSession:didDisconnectWithError: 代理方法。

    @see         streamingSession:didDisconnectWithError:
    @see         PLStreamState

    @since       v1.0.0
 */
- (void)streamingSession:(PLStreamingSession *)session streamStateDidChange:(PLStreamState)state;

/*!
    @method     streamingSession:didDisconnectWithError:
    @abstract   告知代理对象流意外断开。

    @param      session 调用该代理方法的 PLStreamingSession 对象
    @param      error 引起意外断流的错误信息

    @discussion 该代理方法只会在预期外的 PLStreamStateError 状态时触发回调，其他在预期内主动触发的状态不会触发该代理方法。

    @warning    只有 PLStreamStateError 状态会触发该代理方法。

    @see        streamingSession:streamStateDidChange:
    @see        PLStreamError

    @since      v1.0.0
 */
- (void)streamingSession:(PLStreamingSession *)session didDisconnectWithError:(NSError *)error;

/*!
    @method     streamingSession:streamStatusDidUpdate:
    @abstract   每隔一段时间告知代理对象流在这段时间内的流状态。

    @param      session 调用该代理方法的 PLStreamingSession 对象
    @param      status 流的状态

    @discussion 该代理方法在开始推流后启动触发，停止推流时停止触发。默认是每 3s 调用一次，可以通过更改 statusUpdateInterval 属性来变更触发频率。

    @see        statusUpdateInterval
    @see        PLStreamStatus

    @since      v1.1.1
 */
- (void)streamingSession:(PLStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status;

@end

/*!
    @class      PLStreamingSession
    @abstract   PLStreamingSession 是 PLStreamingKit 的负责编码控制的核心类。

    @discussion PLStreamingSession 对象负责音视频编码控制、网络连接控制以及各类状态的监控和向代理反馈。它的生命周期从 init 开始，destroy 结束。
                init 时传递视频的 configuration, 音频的 configuration 以及推流对应的 stream 对象。<br>

                同时，为了更好的提供对 dns 域名解析的容错，PLStreamingKit 以 HappyDNS 做为依赖，当然你可以不去关心这部分，如果有定制 dns 解析的需求，也可以
                通过 init 方法传递自定义的 HappyDNS QNDnsManager 对象。<br>

                在一个 PLStreamingSession 对象的生命周期内，通过调用 -startWithCompleted: 方法开始调用，-stop 方法结束推流。只要一个流没有在服务端做 disable 操作，
                都可以在 -stop 后再调用 -startWithCompleted: 方法重新推流。<br>

                你只要实现了 PLStreamingSessionDelegate 对应的代理方法，就可以获取到推流的状态变更及异常状态的回调，同时还有定时回调的流状态信息反馈。

    @see        PLStreamingSessionDelegate
    @see        PLVideoStreamingConfiguration
    @see        PLAudioStreamingConfiguration
    @see        PLStream

    @since      v1.0.0
 */
@interface PLStreamingSession : NSObject

/*!
    @property   videoConfiguration
    @abstract   视频编码和推流的相关参数配置，只读属性。

    @discussion videoConfiguration 的内容会直接影响视频编码的质量，你可以通过 init 方法作为参数传递，也可以在要变更时调用 -reloadVideoConfiguration: 方法。

    @see        PLVideoStreamingConfiguration
    @see        reloadVideoConfiguration:

    @since      v1.0.0
    @deprecated v1.2.0, 该参数已废弃，请使用 `videoStreamingConfiguration`
 */
@property (nonatomic, copy, readonly) PLVideoStreamingConfiguration  *videoConfiguration DEPRECATED_ATTRIBUTE;

/*!
 @property   videoStreamingConfiguration
 @abstract   视频编码和推流的相关参数配置，只读属性。
 
 @discussion videoStreamingConfiguration 的内容会直接影响视频编码的质量，你可以通过 init 方法作为参数传递，也可以在要变更时调用 -reloadVideoStreamingConfiguration: 方法。
 
 @see        PLVideoStreamingConfiguration
 @see        reloadVideoStreamingConfiguration:
 
 @since      v1.1.7
 */
@property (nonatomic, copy, readonly) PLVideoStreamingConfiguration  *videoStreamingConfiguration;

/*!
    @property   audioConfiguration
    @abstract   音频编码和推流的相关参数配置，只读属性。

    @discussion audioConfiguration 的内容会直接影响音频编码的质量，你可以通过 init 方法作为参数传递来设置。

    @see        PLAudioStreamingConfiguration

    @since      v1.0.0
 
    @deprecated v1.2.0, 该参数已废弃，请使用 `audioStreamingConfiguration`
 */
@property (nonatomic, copy, readonly) PLAudioStreamingConfiguration  *audioConfiguration DEPRECATED_ATTRIBUTE;

/*!
 @property   audioStreamingConfiguration
 @abstract   音频编码和推流的相关参数配置，只读属性。
 
 @discussion audioStreamingConfiguration 的内容会直接影响音频编码的质量，你可以通过 init 方法作为参数传递来设置。
 
 @see        PLAudioStreamingConfiguration
 
 @since      v1.1.7
  */
@property (nonatomic, copy, readonly) PLAudioStreamingConfiguration  *audioStreamingConfiguration;

/*!
    @property   stream
    @abstract   推流时对应的流对象。

    @discussion stream 对象包含了生成推流、播放地址及签名鉴权等信息，该信息是通过七牛直播云服务的服务端获取的。
                通过 init 方法传递给 stream session，或者从服务端获取到信息时，再设置给 stream session。

    @warning    务必确保在调用 -startWithCompleted: 方法前成功设置了正确的 stream，否则会引起推流失败。

    @see        PLStream

    @since      v1.0.0
 */
@property (nonatomic, PL_STRONG) PLStream   *stream;

/*!
    @property   delegate
    @abstract   PLStreamingSession 的代理对象。

    @discussion 要获取 stream session 的流状态、错误异常、流实时统计信息等需要实现 PLStreamingSessionDelegate 中的代理方法，
                如果你设定了 delegateQueue, 那么所有回调将通过对应的 delegateQueue 执行异步回调，如果未指定，将使用 main queue 来执行回调。

    @see        PLStreamingSessionDelegate
    @see        delegateQueue

    @since      v1.0.0
 */
@property (nonatomic, PL_WEAK) id<PLStreamingSessionDelegate> delegate;

/*!
    @property   delegateQueue
    @abstract   触发代理对象回调时所在的任务队列。

    @discussion 默认情况下该值为 nil，此时代理方法都会通过 main queue 异步执行回调。如果你期望可以所有的回调在自己创建或者其他非主线程调用，
                可以设置改 delegateQueue 属性。

    @see        PLStreamingSessionDelegate
    @see        delegate

    @since      v1.2.0
 */
@property (nonatomic, PL_STRONG) dispatch_queue_t   delegateQueue;

/*!
    @property   streamState
    @abstract   流的状态，只读属性。

    @discussion 你可以在 delegate 的回调方法 -streamingSession:streamStateDidChange: 中及时的获取到该值的变更，当然也可以通过 kvo 的方式监听。

    @see        PLStreamingSessionDelegate
    @see        PLStreamState

    @since      v1.0.0
 */
@property (nonatomic, assign, readonly) PLStreamState   streamState;

/*!
    @property   isRunning
    @abstract   是否开始尝试或正在推流，只读属性。

    @discussion 该状态表达的是 streamingSession 有没有开始推流。当 streamState 为 PLStreamStateConnecting 或者 PLStreamStateConnected 时,
                isRunning 都会为 YES，所以它为 YES 时并不表示流一定已经建立连接，其从广义上表达 streamingSession 作为客户端对象的状态，表示是否开始尝试或已经在推流。

    @see        streamState

    @since      v1.0.0
 */
@property (nonatomic, assign, readonly) BOOL    isRunning;

/*!
    @property   statusUpdateInterval
    @abstract   Stream Status 回调方法的调用间隔

    @discussion 该属性的单位为妙，默认为 3 妙，可设置范围为 [1..30] 秒。它影响 PLStreamSessionDelegate 中 -streamingSession:streamStatusDidUpdate:
                方法每次调用的间隔，默认在主线程回调，如果你设置过 delegateQueue，会在 delegateQueue 触发回调。

    @see        PLStreamSessionDelegate
    @see        delegateQueue

    @since      v1.1.1
 */
@property (nonatomic, assign) NSTimeInterval    statusUpdateInterval;

///----------------------
/// @name 对象生命周期
///----------------------

/*!
 @method     initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:
 @abstract   初始化方法
 
 @param      videoStreamingConfiguration 视频编码及推流的配置信息
 @param      audioStreamingConfiguration 音频编码及推流的配置信息
 @param      stream Stream 对象
 
 @return     PLStreamingSession 对象
 
 @discussion 初始化时根据推流的需求传递参数，比如推流同时包含音视频，那么就需要同时传递 videoStreamingConfiguraiton 和 audioStreamingConfiguraiton，类比的，如果只需要
 推送音频，那么 videoStreamingConfiguraiton 就可以为 nil, 只传递 audioStreamingConfiguraiton 便可。<br>
 对于 stream 参数，当没有从服务端获取到 stream 对象时，传递 nil，获取到后再设置，但务必在调用 startWithCompleted: 方法前设置正确的 stream。<br>
 PLStreamingSession 对象默认会使用 HappyDNS 做 dns 解析，如果你期望自己配置 dns 解析的规则，可以调用 -initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:dns:
 方法来初始化 PLStreamingSession 对象。
 
 @see        PLVideoStreamingConfiguration
 @see        PLAudioStreamingConfiguration
 @see        PLStream
 @see        initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:dns:
 
 @since      v1.1.7
 
 */
- (instancetype)initWithVideoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration
                        audioStreamingConfiguration:(PLAudioStreamingConfiguration *)audioStreamingConfiguration
                                             stream:(PLStream *)stream;

/*!
    @method     initWithVideoConfiguration:audioConfiguration:stream:
    @abstract   初始化方法

    @param      videoConfiguration 视频编码及推流的配置信息
    @param      audioConfiguration 音频编码及推流的配置信息
    @param      stream Stream 对象

    @return     PLStreamingSession 对象

    @discussion 初始化时根据推流的需求传递参数，比如推流同时包含音视频，那么就需要同时传递 videoConfiguraiton 和 audioConfiguraiton，类比的，如果只需要
                推送音频，那么 videoConfiguraiton 就可以为 nil, 只传递 audioConfiguraiton 便可。<br>
                对于 stream 参数，当没有从服务端获取到 stream 对象时，传递 nil，获取到后再设置，但务必在调用 startWithCompleted: 方法前设置正确的 stream。<br>
                PLStreamingSession 对象默认会使用 HappyDNS 做 dns 解析，如果你期望自己配置 dns 解析的规则，可以调用 -initWithVideoConfiguration:audioConfiguration:stream:dns:
                方法来初始化 PLStreamingSession 对象。

    @see        PLVideoStreamingConfiguration
    @see        PLAudioStreamingConfiguration
    @see        PLStream
    @see        initWithVideoConfiguration:audioConfiguration:stream:dns:

    @since      v1.0.0
    @deprecated v1.2.0，该参数已经废弃，请使用`initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:`

 */
- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration
                        audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration
                                    stream:(PLStream *)stream DEPRECATED_ATTRIBUTE;

/*!
 @method     initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:dns:
 @abstract   初始化方法
 
 @param      videoStreamingConfiguration 视频编码及推流的配置信息
 @param      audioStreamingConfiguration 音频编码及推流的配置信息
 @param      stream Stream 对象
 @param      dns dnsmanager，自定义 dns 查询，使用 HappyDNS
 
 @return     PLStreamingSession 对象
 
 @discussion 初始化时根据推流的需求传递参数，比如推流同时包含音视频，那么就需要同时传递 videoStreamingConfiguraiton 和 audioStreamingConfiguraiton，类比的，如果只需要
 推送音频，那么 videoStreamingConfiguraiton 就可以为 nil, 只传递 audioStreamingConfiguraiton 便可。<br>
 对于 stream 参数，当没有从服务端获取到 stream 对象时，传递 nil，获取到后再设置，但务必在调用 startWithCompleted: 方法前设置正确的 stream。<br>
 PLStreamingSession 对象默认会使用 HappyDNS 做 dns 解析，如果你期望自己配置 dns 解析的规则，可以通过传递自己定义的 dns manager 来做 dns 查询。
 如果你对 dns 解析部分不清楚，可以直接使用 -initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream: 方法初始化 PLStreamingSession 对象。
 
 @see        PLVideoStreamingConfiguration
 @see        PLAudioStreamingConfiguration
 @see        PLStream
 @see        initWithVideoStreamingConfiguration:audioStreamingConfiguration:stream:
 
 @since      v1.0.0
 */
- (instancetype)initWithVideoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration
                        audioStreamingConfiguration:(PLAudioStreamingConfiguration *)audioStreamingConfiguration
                                             stream:(PLStream *)stream
                                                dns:(QNDnsManager *)dns;

/*!
    @method     initWithVideoConfiguration:audioConfiguration:stream:dns:
    @abstract   初始化方法

    @param      videoConfiguration 视频编码及推流的配置信息
    @param      audioConfiguration 音频编码及推流的配置信息
    @param      stream Stream 对象
    @param      dns dnsmanager，自定义 dns 查询，使用 HappyDNS

    @return     PLStreamingSession 对象

    @discussion 初始化时根据推流的需求传递参数，比如推流同时包含音视频，那么就需要同时传递 videoConfiguraiton 和 audioConfiguraiton，类比的，如果只需要
                推送音频，那么 videoConfiguraiton 就可以为 nil, 只传递 audioConfiguraiton 便可。<br>
                对于 stream 参数，当没有从服务端获取到 stream 对象时，传递 nil，获取到后再设置，但务必在调用 startWithCompleted: 方法前设置正确的 stream。<br>
                PLStreamingSession 对象默认会使用 HappyDNS 做 dns 解析，如果你期望自己配置 dns 解析的规则，可以通过传递自己定义的 dns manager 来做 dns 查询。
                如果你对 dns 解析部分不清楚，可以直接使用 -initWithVideoConfiguration:audioConfiguration:stream: 方法初始化 PLStreamingSession 对象。

    @see        PLVideoStreamingConfiguration
    @see        PLAudioStreamingConfiguration
    @see        PLStream
    @see        initWithVideoConfiguration:audioConfiguration:stream:

    @since      v1.0.0
    @deprecated v1.2.0 该方法已废弃，请使用
 */
- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration
                        audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration
                                    stream:(PLStream *)stream
                                       dns:(QNDnsManager *)dns DEPRECATED_ATTRIBUTE;

/*!
    @method     destroy
    @abstract   销毁方法

    @discussion 当一个 PLStreamingSession 对象被创建并初始化后，它便是可用的。可以通过调用 -startWithCompleted: 和 -stop 方法开始和停止推流，
                即便 -stop 后，仍然可以调用 -startWithCompleted: 方法重新推流。
                当你调用了 -destroy 方法后，PLStreamingSession 对象便不再可用，不可再调用 -startWithCompleted: 和 -stop 方法，如果还希望推流，就需要
                再创建一个新的 PLStreamingSession 对象。另外，-destroy 方法调用前可以不用调用 -stop 方法，-destroy 方法内部会有调用。

    @see        startWithCompleted:
    @see        stop

    @since      v1.0.0
 */
- (void)destroy;

///---------------------------
/// @name 推流操作
///---------------------------

/*!
    @method     startWithCompleted:
    @abstract   开始推流

    @param      handler 流连接的结果会通过该回调方法返回，携带是否已连接成功的布尔值

    @discussion 当 Streaming Session 创建并初始化好后（务必确认 stream 对象已设置好），就可以调用此方法开始推流。当要停止一次推流但是并不销毁 Streaming Session
                对象时，调用 -stop 方法即可，便于在需要重新推流时再重新调用该方法进行推流。如果确认不再使用对应 stream 进行推流，可以调用 -destroy 销毁
                Streaming Session 对象，销毁后的对象不可再用于推流或做其他操作，如有需求，需要创建一个新的 Streaming Session 对象。<br>
                handler 回调的线程会优先使用 delegateQueue, 如果 delegateQueue 未设置，会在主线程异步调用。

    @warning    当前版本中，当 App 进入后台，Streaming Session 内部会主动调用 -stop 停止推流，回到前台后并不会重新调用该方法，回到前台后可以根据业务需求不同重新推流或者销毁
                Streaming Session。
                当采用 dynamic 认证且过期时，需要更新 Stream 对象，否则推流将失败。

    @see        stop
    @see        destroy

    @since      @v1.0.0
 */
- (void)startWithCompleted:(void (^)(BOOL success))handler;

/*!
    @method     restartWithCompleted:
    @abstract   重新开始推流
 
    @param      handler 流连接的结果会通过该回调方法返回，携带是否已连接成功的布尔值
 
    @discussion 当 Streaming Session 处于正在推流过程中，由于业务原因（如用户网络从 4G 切到 WIFI）需要快速重新推流时，可以调用此方法重新推流。当要停止一次推流但是并不销毁 Streaming Session
                对象时，调用 -stop 方法即可，便于在需要重新推流时再重新调用该方法进行推流。如果确认不再使用对应 stream 进行推流，可以调用 -destroy 销毁
                Streaming Session 对象，销毁后的对象不可再用于推流或做其他操作，如有需求，需要创建一个新的 Streaming Session 对象。<br>
                handler 回调的线程会优先使用 delegateQueue, 如果 delegateQueue 未设置，会在主线程异步调用。
 
    @warning    当前 Streaming Session 处于正在推流状态时调用此方法时才会重新推流，其它状态时调用无效
                Streaming Session。
                当采用 dynamic 认证且过期时，需要更新 Stream 对象，否则推流将失败。
 
 @see        stop
 @see        destroy
 
 @since      @v1.2.2
 */
- (void)restartWithCompleted:(void (^)(BOOL success))handler;

/*!
    @method     stop
    @abstract   结束推流

    @discussion 当要结束一次推流时，调用该方法。在 stop 后，如果还需要推流，可以重新调用 -startWithCompleted: 方法，如果要销毁一个 Streaming Session 直接调用 -destroy
                便可，不用额外调用 -stop。

    @see        startWithCompleted:
    @see        destroy

    @since      @v1.0.0
 */
- (void)stop;

/*!
 @method     reloadVideoStreamingConfiguration:
 @abstract   重新设置视频编码配置信息
 
 @param      videoStreamingConfiguration 新设置的视频编码配置对象
 
 @discussion 调用该方法可以更改视频编码的配置。请务必在调用该方法时传递的 videoStreamingConfiguration 与 Streaming Session 所持有的 videoStreamingConfiguration 不是一个同一个对象（指针不同）。
 
 @see        PLVideoStreamingConfiguration
 
 @since      v1.1.0
 */
- (void)reloadVideoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration;

/*!
    @method     reloadVideoConfiguration:
    @abstract   重新设置视频编码配置信息

    @param      videoConfiguration 新设置的视频编码配置对象

    @discussion 调用该方法可以更改视频编码的配置。请务必在调用该方法时传递的 videoConfiguration 与 Streaming Session 所持有的 videoConfiguration 不是一个同一个对象（指针不同）。

    @see        PLVideoStreamingConfiguration

    @since      v1.1.0
 
    @deprecated v1.2.0, 该方法已弃用，请使用 `-reloadVideoStreamingConfiguration:` 方法
 */
- (void)reloadVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration DEPRECATED_ATTRIBUTE;

/*!
 @method     reloadAudioStreamingConfiguration:
 @abstract   重新设置音频编码配置信息
 
 @param      audioStreamingConfiguration 新设置的音频编码配置对象
 
 @discussion 调用该方法可以更改音频编码的配置。请务必在调用该方法时传递的 audioStreamingConfiguration 与 Streaming Session 所持有的 audioStreamingConfiguration 不是一个同一个对象（指针不同）。
 
 @see        PLAudioStreamingConfiguration
 
 @since      @v1.2.0
 */
- (void)reloadAudioStreamingConfiguration:(PLAudioStreamingConfiguration *)audioStreamingConfiguration;

/*!
 @method     reloadAudioConfiguration:
 @abstract   重新设置音频编码配置信息
 
 @param      audioConfiguration 新设置的音频编码配置对象
 
 @discussion 调用该方法可以更改音频编码的配置。请务必在调用该方法时传递的 audioConfiguration 与 Streaming Session 所持有的 audioConfiguration 不是一个同一个对象（指针不同）。
 
 @see        PLAudioStreamingConfiguration
 
 @since      v1.1.0
 
 @deprecated v1.2.0, 该方法已弃用，请使用 `-reloadAudioStreamingConfiguration` 方法
 */
- (void)reloadAudioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration DEPRECATED_ATTRIBUTE;

///------------------
/// @name 发送视频数据
///------------------

/*!
    @method     pushVideoSampleBuffer:
    @abstract   发送视频 SampleBuffer 数据。

    @param      sampleBuffer 待发送的 CMSampleBuffer 数据

    @discussion 该方法内部异步执行编码操作，所以如果你期望在调用完该方法后还要对 sampleBuffer 做操作，请调用 -pushVideoSampleBuffer:completion: 方法，并在 handler block 中完成操作。

    @see        pushVideoSampleBuffer:completion:
    @see        pushPixelBuffer:
    @see        pushPixelBuffer:completion:

    @since      @v1.0.0
 */
- (void)pushVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/*!
    @method     pushVideoSampleBuffer:completion:
    @abstract   发送视频 SampleBuffer 数据。

    @param      sampleBuffer 待发送的 CMSampleBuffer 数据
    @param      handler 在编码 sampleBuffer 数据完成时的回调 block, 带是否编码成功的参数 success

    @discussion 如果你期望在调用完该方法后还要对 sampleBuffer 做操作，请调用该方法，并在 handler block 中完成操作。调用 -pushVideoSampleBuffer: 相当于调用该方法并以 nil 作为 handler 参数。

    @warning    该方法的 handler 回调并不在主线程，也不在 delegateQueue 线程，所以除了对 sampleBuffer 做 unlock 或者销毁等操作，务必不要做额外高计算量的操作，或者长时间让 handler 无法结束运行。

    @see        pushVideoSampleBuffer:
    @see        pushPixelBuffer:
    @see        pushPixelBuffer:completion:

    @since      @v1.1.0
 */
- (void)pushVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer completion:(void (^)(BOOL success))handler;

/*!
    @method     pushPixelBuffer:
    @abstract   发送视频 CVPixelBufferRef 数据。

    @param      pixelBuffer 待发送的 CVPixelBufferRef 数据

    @discussion 该方法内部异步执行编码操作，所以如果你期望在调用完该方法后还要对 pixelBuffer 做操作，请调用 -pushPixelBuffer:completion: 方法，并在 handler block 中完成操作。

    @see        pushVideoSampleBuffer:
    @see        pushVideoSampleBuffer:completion:
    @see        pushPixelBuffer:completion:

    @since      @v1.0.0
 */
- (void)pushPixelBuffer:(CVPixelBufferRef)pixelBuffer;

/*!
    @method     pushPixelBuffer:completion:
    @abstract   发送视频 CVPixelBufferRef 数据。

    @param      pixelBuffer 待发送的 CVPixelBufferRef 数据
    @param      handler 在编码 pixelBuffer 数据完成时的回调 block, 带是否编码成功的参数 success

    @discussion 如果你期望在调用完该方法后还要对 pixelBuffer 做操作，请调用该方法，并在 handler block 中完成操作。调用 -pushPixelBuffer: 相当于调用该方法并以 nil 作为 handler 参数。

    @warning    该方法的 handler 回调并不在主线程，也不在 delegateQueue 线程，所以除了对 pixelBuffer 做 unlock 或者销毁等操作，务必不要做额外高计算量的操作，或者长时间让 handler 无法结束运行。

    @see        pushVideoSampleBuffer:
    @see        pushVideoSampleBuffer:completion:
    @see        pushPixelBuffer:

    @since      @v1.1.0
 */
- (void)pushPixelBuffer:(CVPixelBufferRef)pixelBuffer completion:(void (^)(BOOL success))handler;

///------------------
/// @name 发送音频数据
///------------------

/*!
    @method     pushAudioSampleBuffer:
    @abstract   发送音频 SampleBuffer 数据。

    @param      sampleBuffer 待发送的音频 CMSampleBuffer 数据

    @discussion 该方法内部异步执行编码操作，所以如果你期望在调用完该方法后还要对 sampleBuffer 做操作，请调用 -pushAudioSampleBuffer:completion: 方法，并在 handler block 中完成操作。

    @see        pushAudioSampleBuffer:completion:
    @see        pushAudioBuffer:
    @see        pushAudioBuffer:completion:

    @since      @v1.0.0
 */
- (void)pushAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;

/*!
    @method     pushAudioSampleBuffer:completion:
    @abstract   发送音频 CMSampleBufferRef 数据。

    @param      sampleBuffer 待发送的音频 CMSampleBufferRef 数据
    @param      handler 在编码 sampleBuffer 数据完成时的回调 block, 带是否编码成功的参数 success

    @discussion 如果你期望在调用完该方法后还要对 sampleBuffer 做操作，请调用该方法，并在 handler block 中完成操作。调用 -pushAudioSampleBuffer: 相当于调用该方法并以 nil 作为 handler 参数。

    @warning    该方法的 handler 回调并不在主线程，也不在 delegateQueue 线程，所以除了对 sampleBuffer 做 unlock 或者销毁等操作，务必不要做额外高计算量的操作，或者长时间让 handler 无法结束运行。
    @see        pushAudioSampleBuffer:
    @see        pushAudioBuffer:
    @see        pushAudioBuffer:completion:

    @since      @v1.1.0
 */
- (void)pushAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer completion:(void (^)(BOOL success))handler;


///------------------
/// @name 发送音频数据
///------------------

/*!
 @method     pushAudioBuffer:asbd:
 @abstract   发送音频 AudioBuffer 数据。
 
 @param      buffer 待发送的音频 AudioBuffer 数据
 @param      asbd 待发送 AudioBuffer 对应的 AudioStreamBasicDescription
 
 @discussion 该方法内部异步执行编码操作，所以如果你期望在调用完该方法后还要对 sampleBuffer 做操作，请调用 -pushAudioSampleBuffer:completion: 方法，并在 handler block 中完成操作。
 
 @see        pushAudioSampleBuffer:completion:
 @see        pushAudioBuffer:
 @see        pushAudioBuffer:completion:
 
 @since      @v1.1.6
 */
- (void)pushAudioBuffer:(AudioBuffer *)buffer asbd:(const AudioStreamBasicDescription *)asbd;

/*!
 @method     pushAudioBuffer:asbd:completion:
 @abstract   发送音频 AudioBuffer 数据。
 
 @param      audioBuffer 待发送的音频 AudioBuffer 数据
 @param      asbd 待发送 AudioBuffer 对应的 AudioStreamBasicDescription
 @param      handler 在编码 AudioBuffer 数据完成时的回调 block, 带是否编码成功的参数 success
 
 @discussion 如果你期望在调用完该方法后还要对 AudioBuffer 做操作，请调用该方法，并在 handler block 中完成操作。调用 -pushAudioBuffer:asbd: 相当于调用该方法并以 nil 作为 handler 参数。
 
 @warning    该方法的 handler 回调并不在主线程，也不在 delegateQueue 线程，所以除了对 AudioBuffer 做 unlock 或者销毁等操作，务必不要做额外高计算量的操作，或者长时间让 handler 无法结束运行。
 @see        pushAudioSampleBuffer:
 @see        pushAudioBuffer:
 @see        pushAudioBuffer:completion:
 
 @since      @v1.1.6
 */
- (void)pushAudioBuffer:(AudioBuffer *)audioBuffer asbd:(const AudioStreamBasicDescription *)asbd completion:(void (^)(BOOL success))handler;

@end

#pragma mark - Category (Network)

///-------------------------------
/// @name   网络操作相关的参数与操作
///-------------------------------

/*!
    @category   PLStreamingSession (Network)
    @abstract   PLStreamingSession 网络操作相关的参数与操作

    @since      v1.1.4
 */
@interface PLStreamingSession (Network)

/*!
    @property   receiveTimeout
    @abstract   网络连接和接收数据超时。

    @discussion 以秒作为单位。默认为 15s, 设定最小数值不得低于 3s，否则不变更。

    @see        sendTimeout

    @since      v1.1.4
 */
@property (nonatomic, assign) int   receiveTimeout;

/*!
     @property   sendTimeout
     @abstract   网络发送数据超时。

     @discussion 以秒作为单位。默认为 3s, 设定最小数值不得低于 3s，否则不变更。

     @see        receiveTimeout

     @since      v1.1.4
 */
@property (nonatomic, assign) int   sendTimeout;

@end

#pragma mark - Category (SendingBuffer)

/*!
     @category   PLStreamingSession (SendingBuffer)
     @abstract   PLStreamingSession 发送队列相关参数

     @since      v1.0.0
 */
@interface PLStreamingSession (SendingBuffer)

/*!
     @property   bufferDelegate
     @abstract   发送队列的代理对象。

     @discussion 发送队列的代理对象需呀实现 PLStreamingSendingBufferDelegate 协议，可以在发送队列空或者满时获取到回调。

     @see        PLStreamingSendingBufferDelegate

     @since      v1.0.0
 */
@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/*!
    @property   threshold
    @abstract   发送丢列触发丢包策略时会丢掉的阈值。

    @discussion 当队列满时，会触发发送队列内的包被丢弃，即丢帧。此时需要一个边界来停止丢帧行为，就是这个阈值字段。
                可设定范围 [0..1], 不可超出这个范围, 默认为 0.5。

    @see        v1.0.0
 */
@property (nonatomic, assign) CGFloat   threshold;

/*!
    @property   maxCount
    @abstract   发送队列最大容纳包数量。

    @discussion 该数量囊括音频与视频包，默认为 300 个。当网络不佳时，发送队列就可能出现队列满的情况，此时会触发队列丢包。

    @see        v1.0.0
 */
@property (nonatomic, assign) NSUInteger    maxCount;

/*!
    @property   currentCount
    @abstract   发送队列当前已有包数，只读属性。

    @see        v1.0.0
 */
@property (nonatomic, assign, readonly) NSUInteger    currentCount;

@end

#pragma mark - Categroy (Application)

///------------------
/// @name 应用状态管理
///------------------

/*!
    @category   PLCameraStreamingSession(Application)
    @abstract   与应用状态相关的接口

    @since      v1.0.0
 */
@interface PLStreamingSession (Application)

/*!
 @property   idleTimerDisable
 @abstract   控制系统屏幕自动锁屏是否关闭。

 @discussion 默认为 YES。控制系统屏幕自动锁屏是否关闭。

 @see        v1.0.0
 */
@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end


#pragma mark - Category (Info)

///------------------
/// @name SDK 信息相关
///------------------

/*!
 @category   PLStreamingSession (Info)
 @abstract   SDK 信息相关

 @since      v1.1.1
 */
@interface PLStreamingSession (Info)

/*!
    @method     versionInfo
    @abstract   PLStreamingKit 的 SDK 版本。

    @since      v1.1.1
 */
+ (NSString *)versionInfo;

@end
