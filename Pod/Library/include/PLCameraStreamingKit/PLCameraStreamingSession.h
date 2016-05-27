//
//  PLCameraStreamingSession.h
//  PLCameraStreamingKit
//
//  Created on 15/4/1.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>
#import <PLStreamingKit/PLStreamingKit.h>
#import <GPUImage/GPUImage.h>
#import "PLSourceAccessProtocol.h"
#import "PLVideoCaptureConfiguration.h"
#import "PLAudioCaptureConfiguration.h"

// post with userinfo @{@"state": @(state)}. always posted via MainQueue.
extern NSString *PLStreamStateDidChangeNotification;
extern NSString *PLCameraAuthorizationStatusDidGetNotificaiton;
extern NSString *PLMicrophoneAuthorizationStatusDidGetNotificaiton;

extern NSString *PLCameraDidStartRunningNotificaiton;
extern NSString *PLMicrophoneDidStartRunningNotificaiton;
extern NSString *PLAudioComponentFailedToCreateNotification;

typedef NSNumber * PLFilterHandler;

typedef enum {
    /**
     @brief Stretch to fill the full view, which may distort the image outside of its normal aspect ratio
     */
    PLVideoFillModeStretch,
    
    /**
     @brief Maintains the aspect ratio of the source image, adding bars of the specified background color
     */
    PLVideoFillModePreserveAspectRatio,
    
    /**
     @brief Maintains the aspect ratio of the source image, zooming in on its center to fill the view
     */
    PLVideoFillModePreserveAspectRatioAndFill
} PLVideoFillModeType;

@class PLCameraStreamingSession;

/// @abstract delegate 对象可以实现对应的方法来获取流的状态及设备授权情况。
@protocol PLCameraStreamingSessionDelegate <NSObject>

@optional
/// @abstract 流状态已变更的回调
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state;

/// @abstract 因产生了某个 error 而断开时的回调
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didDisconnectWithError:(NSError *)error;

/// @abstract 当开始推流时，会每间隔 3s 调用该回调方法来反馈该 3s 内的流状态，包括视频帧率、音频帧率、音视频总码率
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status;

/// @abstract 摄像头授权状态发生变化的回调
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetCameraAuthorizationStatus:(PLAuthorizationStatus)status;

/// @abstract 麦克风授权状态发生变化的回调
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetMicrophoneAuthorizationStatus:(PLAuthorizationStatus)status;

/// @abstract 获取到摄像头原数据时的回调, 便于开发者做滤镜等处理
- (CVPixelBufferRef)cameraStreamingSession:(PLCameraStreamingSession *)session cameraSourceDidGetPixelBuffer:(CVPixelBufferRef)pixelBuffer;

@end

/*!
 * @abstract 推流中的核心类。
 *
 * @discussion 一个 PLCameraStreamingSession 实例会包含了对视频源、音频源的控制，并且对流的操作及流状态的返回都是通过它来完成的。
 */
@interface PLCameraStreamingSession : NSObject

#pragma mark - readonly properties

/// 视频编码及推流配置，只读
@property (nonatomic, copy, readonly) PLVideoCaptureConfiguration  *videoCaptureConfiguration;

/// 音频编码及推流配置，只读
@property (nonatomic, copy, readonly) PLAudioCaptureConfiguration  *audioCaptureConfiguration;

/// 视频编码及推流配置，只读
@property (nonatomic, copy, readonly) PLVideoStreamingConfiguration  *videoStreamingConfiguration;

/// 音频编码及推流配置，只读
@property (nonatomic, copy, readonly) PLAudioStreamingConfiguration  *audioStreamingConfiguration;

/// 流的状态，只读属性
@property (nonatomic, assign, readonly) PLStreamState   streamState;

/*!
 * 是否开始推流，只读属性
 *
 * @discussion 该状态表达的是 streamingSession 有没有开始推流。当 streamState 为 PLStreamStateConnecting 或者 PLStreamStateConnected 时, isRunning 都会为 YES，所以它为 YES 时并不表示流一定已经建立连接，其从广义上表达 streamingSession 作为客户端对象的状态。
 */
@property (nonatomic, assign, readonly) BOOL    isRunning;

/*!
 * @abstract 摄像头的预览视图，在 PLCameraStreamingSession 初始化之后可以获取该视图
 *
 */
@property (nonatomic, PL_STRONG, readonly) UIView * previewView;

#pragma mark - readwrite properties

/// 流对象
@property (nonatomic, PL_STRONG) PLStream   *stream;

/// 获取和设置视频方向
@property (nonatomic, assign) AVCaptureVideoOrientation videoOrientation;

/// 代理对象
@property (nonatomic, PL_WEAK) id<PLCameraStreamingSessionDelegate> delegate;

/// 默认为 3s，可设置范围为 [1..30] 秒
@property (nonatomic, assign) NSTimeInterval    statusUpdateInterval;

/**
 @brief previewView 中视频的填充方式
 */
@property(readwrite, nonatomic) PLVideoFillModeType fillMode;

#pragma mark - basic
/*!
 * 初始化方法
 *
 * @param videoCaptureConfiguration 视频采集的配置信息
 *
 * @param audioCaptureConfiguration 音频采集的配置信息
 *
 * @param videoStreamingConfiguration 视频编码及推流的配置信息
 *
 * @param audioStreamingConfiguration 音频编码及推流的配置信息
 *
 * @param stream Stream 对象
 *
 * @param videoOrientation 视频方向
 *
 * @return PLCameraStreamingSession 实例
 *
 * @discussion 该方法会检查传入参数，当 videoCaptureConfiguration 或 videoStreamingConfiguration 为 nil 时为纯音频推流模式，当 audioCaptureConfiguration 或 audioStreamingConfiguration 为 nil 时为纯视频推流模式，当初始化方法会优先使用后置摄像头，如果发现设备没有后置摄像头，会判断是否有前置摄像头，如果都没有，便会返回 nil。
 */
- (instancetype)initWithVideoCaptureConfiguration:(PLVideoCaptureConfiguration *)videoCaptureConfiguration
                        audioCaptureConfiguration:(PLAudioCaptureConfiguration *)audioCaptureConfiguration
                      videoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration
                      audioStreamingConfiguration:(PLAudioStreamingConfiguration *)audioStreamingConfiguration
                                           stream:(PLStream *)stream
                                 videoOrientation:(AVCaptureVideoOrientation)videoOrientation;

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

/**
 @brief 重新加载视频推流配置
 
 @param videoStreamingConfiguration 新的视频编码配置
 @param videoCaptureConfiguration   新的视频采集配置
 */
- (void)reloadVideoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration videoCaptureConfiguration:(PLVideoCaptureConfiguration *)videoCaptureConfiguration;

@end

#pragma mark - Category (SendingBuffer)

@interface PLCameraStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// [0..1], 不可超出这个范围, 默认为 0.5
@property (nonatomic, assign) CGFloat threshold;

/// Buffer 最多可包含的包数，默认为 300
@property (nonatomic, assign) NSUInteger    maxCount;
@property (nonatomic, assign, readonly) NSUInteger    currentCount;

@end

#pragma mark - Category (CameraSource)

/*!
 * @category PLCameraStreamingSession(CameraSource)
 *
 * @discussion 与摄像头相关的接口
 */
@interface PLCameraStreamingSession (CameraSource)

/// default as AVCaptureDevicePositionBack.
@property (nonatomic, assign) AVCaptureDevicePosition   captureDevicePosition;

/// default as NO.
@property (nonatomic, assign, getter=isTorchOn) BOOL    torchOn;

/// default as (0.5, 0.5), (0,0) is top-left, (1,1) is bottom-right.
@property (nonatomic, assign) CGPoint   focusPointOfInterest;

/// default as YES.
@property (nonatomic, assign, getter=isContinuousAutofocusEnable) BOOL  continuousAutofocusEnable;

/// default as YES.
@property (nonatomic, assign, getter=isTouchToFocusEnable) BOOL touchToFocusEnable;

/// default as YES.
@property (nonatomic, assign, getter=isSmoothAutoFocusEnabled) BOOL  smoothAutoFocusEnabled;

/// 默认为 1.0，设置的数值需要小于等于 videoActiveForat.videoMaxZoomFactor，如果大于会设置失败。
@property (nonatomic, assign) CGFloat videoZoomFactor;

@property (nonatomic, strong, readonly) NSArray<AVCaptureDeviceFormat *> *videoFormats;

@property (nonatomic, retain) AVCaptureDeviceFormat *videoActiveFormat;

- (void)toggleCamera;

/*!
 * 开启摄像头 session
 *
 * @discussion 这个方法一般不需要调用，但当你的 App 中需要同时使用到 AVCaptureSession 时，在调用过 - (void)stopCaptureSession 方法后，
 * 如果要重新启用推流的摄像头，可以调用这个方法
 *
 * @see - (void)stopCaptureSession
 */
- (void)startCaptureSession;

/*!
 * 停止摄像头 session
 *
 * @discussion 这个方法一般不需要调用，但当你的 App 中需要同时使用到 AVCaptureSession 时，当你需要暂且切换到你自己定制的摄像头做别的操作时，
 * 你需要调用这个方法来暂停当前 streaming session 对 captureSession 的占用。当需要恢复时，调用 - (void)startCaptureSession 方法。
 *
 * @see - (void)startCaptureSession
 */
- (void)stopCaptureSession;

@end

#pragma mark - Category (MicrophoneSource)

/*!
 * @category PLCameraStreamingSession(MicrophoneSource)
 *
 * @discussion 与麦克风相关的接口
 */
@interface PLCameraStreamingSession (MicrophoneSource)

@property (nonatomic, assign, getter=isMuted)   BOOL    muted;                   // default as NO.

@end

#pragma mark - Categroy (Application)

/*!
 * @category PLCameraStreamingSession(Application)
 *
 * @discussion 与系统相关的接口
 */
@interface PLCameraStreamingSession (Application)

@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end

#pragma mark - Category (Authorization)

/*!
 * @category PLCameraStreamingSession(Authorization)
 *
 * @discussion 与设备授权相关的接口
 */
@interface PLCameraStreamingSession (Authorization)

// Camera
+ (PLAuthorizationStatus)cameraAuthorizationStatus;
+ (void)requestCameraAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

// Microphone
+ (PLAuthorizationStatus)microphoneAuthorizationStatus;
+ (void)requestMicrophoneAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

@end

#pragma mark - Category (Processing)

@interface PLCameraStreamingSession (Processing)

/**
 @brief 添加水印
 
 @param waterMark 水印图片
 @param origin    水印坐标
 
 @return 如果传入的水印图片为 nil 或水印边界超过采集图像的大小则返回 nil, 否则返回创建成功的水印 filter 对应的 handler
 */
- (PLFilterHandler)addWaterMark:(UIImage *)waterMark origin:(CGPoint)origin;

/**
 @brief 添加美颜滤镜
 
 @return 美颜 filter 对应的 handler
 */
- (PLFilterHandler)addBeautyFilter;

/**
 @brief 使用 GPUImageFilter 添加 filter
 
 @param filter 需要添加的 GPUImageFilter 实例
 
 @return GPUImageFilter 对应的 handler
 */
- (PLFilterHandler)addGPUImageFilter:(GPUImageFilter *)filter;

/**
 @brief 移除 handler 对应的 filter
 
 @param handler 需要移除的 filter handler
 */
- (void)removeFilter:(PLFilterHandler)handler;

/**
 @brief 以一定的比例调整对应 BeautyFilter 的亮度参数，该参数为增加或降低亮度，而非调整亮度的绝对值
 
 @param brightness 范围从 0 ~ 2，1 为不改变亮度
 @param handler    需要设置的 handler
 
 @warning 仅对 BeautyFilter 该设置有效
 */
- (void)adjustBritness:(CGFloat)brightness forFilter:(PLFilterHandler)handler;

/**
 @brief 以一定的比例调整对应 BeautyFilter 的饱和度参数，该参数为增加或降低饱和度，而非调整饱和度的绝对值
 
 @param brightness 范围从 0 ~ 2，1 为不改变饱和度
 @param handler    需要设置的 handler
 
 @warning 仅对 BeautyFilter 该设置有效
 */
- (void)adjustSaturation:(CGFloat)saturation forFilter:(PLFilterHandler)handler;

/**
 @brief 重置对应的 BeautyFilter 的亮度和饱和度参数到默认值
 
 @param handler 需要设置的 handler
 
 @warning 仅对 BeautyFilter 该设置有效
 */
- (void)resetBritnessAndSaturationForFilter:(PLFilterHandler)handler;
/**
 @brief 设置对应 BeautyFilter 的磨皮程度参数.
 
 @param brightness 范围从 0 ~ 1，0 为不磨皮，默认为 0.5
 @param handler    需要设置的 handler
 
 @warning 仅对 BeautyFilter 该设置有效
 */
- (void)setSmoothIntensity:(CGFloat)smoothIntensity forFilter:(PLFilterHandler)handler;

@end
