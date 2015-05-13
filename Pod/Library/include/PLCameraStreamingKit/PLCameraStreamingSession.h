//
//  PLCameraStreamingSession.h
//  PLCameraStreamingKit
//
//  Created on 15/4/1.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#import "PLCameraStreamingConfiguration.h"
#import "PLMacroDefines.h"
#import "PLTypeDefines.h"

// post with userinfo @{@"state": @(state)}. always posted via MainQueue.
extern NSString *PLStreamStateDidChangeNotification;
extern NSString *PLCameraAuthorizationStatusDidGetNotificaiton;
extern NSString *PLMicrophoneAuthorizationStatusDidGetNotificaiton;

extern NSString *PLCameraDidStartRunningNotificaiton;
extern NSString *PLMicrophoneDidStartRunningNotificaiton;

@class PLCameraStreamingSession;

/*!
 * @protocol PLCameraStreamingSessionDelegate
 *
 * @discussion delegate 对象可以实现对应的方法来获取流的状态及设备授权情况。
 */
@protocol PLCameraStreamingSessionDelegate <NSObject>

@optional
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state;

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetCameraAuthorizationStatus:(PLAuthorizationStatus)status;
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetMicrophoneAuthorizationStatus:(PLAuthorizationStatus)status;

@end

/*!
 * @class PLCameraStreamingSession
 *
 * @abstract 推流中的核心类。
 *
 * @discussion 一个 PLCameraStreamingSession 实例会包含了对视频源、音频源的控制，并且对流的操作及流状态的返回都是通过它来完成的。
 */
@interface PLCameraStreamingSession : NSObject

/*!
 * @property configuration
 *
 * @abstract 音视频编码信息均包含其中。
 */
@property (nonatomic, PL_STRONG) PLCameraStreamingConfiguration *configuration;  // reset will not work until startWithPushURL: invoked.

/*!
 * @property videoOrientation
 *
 * @abstract 获取和设置视频方向
 */
@property (nonatomic, assign) AVCaptureVideoOrientation    videoOrientation;

/*!
 * @property delegate
 *
 * @abstract 代理对象
 */
@property (nonatomic, PL_WEAK) id<PLCameraStreamingSessionDelegate> delegate;

/*!
 * @property streamState
 *
 * @abstract 流的状态，只读属性
 */
@property (nonatomic, assign, readonly) PLStreamState               streamState;

/*!
 * @property isRunning
 *
 * @abstract 是否在推流，只读属性
 */
@property (nonatomic, assign, readonly) BOOL                        isRunning;

/*!
 * @property pushURL
 *
 * @abstract 推流 URL，只读属性
 */
@property (nonatomic, PL_STRONG, readonly) NSURL *pushURL;   // rtmp only now.

/*!
 * @property previewView
 *
 * @abstract 摄像头的预览视图
 *
 * @discussion 在设置预览视图时，请确保 previewView 的 size 已经设置正确。
 */
@property (nonatomic, PL_WEAK) UIView *previewView;

/*!
 * 初始化方法
 *
 * @param configuration 用于音视频编码的配置信息
 *
 * @param videoOrientation 视频方向
 *
 * @return PLCameraStreamingSession 实例
 *
 * @discussion 初始化方法会优先使用后置摄像头，如果发现设备没有后置摄像头，会判断是否有前置摄像头，如果都没有，便会返回 nil。
 */
- (instancetype)initWithConfiguration:(PLCameraStreamingConfiguration *)configuration videoOrientation:(AVCaptureVideoOrientation)videoOrientation;

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
- (void)startWithPushURL:(NSURL *)pushURL completed:(void (^)(BOOL success))handler;

/*!
 * 结束推流
 */
- (void)stop;

/*!
 * 更新 previewView 的大小
 *
 * @param size 新的大小
 *
 * @discussion 该方法并不会变更编码视频的大小，只会改变当前设备摄像预览视图的尺寸
 */
- (void)updatePreviewViewSize:(CGSize)size;

@end

#pragma mark - Category (CameraSource)

/*!
 * @category PLCameraStreamingSession(CameraSource)
 *
 * @discussion 与摄像头相关的接口
 */
@interface PLCameraStreamingSession (CameraSource)

@property (nonatomic, assign) PLCaptureDevicePosition   captureDevicePosition;   // default as AVCaptureDevicePositionBack.
@property (nonatomic, assign, getter=isTorchOn) BOOL    torchOn;                 // default as NO.
@property (nonatomic, assign) CGPoint   focusPointOfInterest;                    // default as (0.5, 0.5), (0,0) is top-left, (1,1) is bottom-right.
@property (nonatomic, assign, getter=isContinuousAutofocusEnable) BOOL  continuousAutofocusEnable;  // default as YES.
@property (nonatomic, assign, getter=isTouchToFocusEnable) BOOL touchToFocusEnable; // default as YES.

- (void)toggleCamera;

// Desprecated. This method will always return nil now, use requestCaptureImageWithComplete: or requestCaptureImageDataWithQuality:complete: instead
- (UIImage *)stillCaptureImage DEPRECATED_ATTRIBUTE;

/*!
 * 获取视频截图的方法
 *
 * @discussion 图片处理会增加 cpu 和内存占用，请尽可能少的使用该方法。
 *
 * @param block 获取到图片后的回调方法，该回调一定在主线程，所以可以放心的对 UI 控件做操作。
 */
- (void)requestCaptureImageWithComplete:(void (^)(UIImage *))block;

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