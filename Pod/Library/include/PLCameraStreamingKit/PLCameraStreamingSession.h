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

@class PLCameraStreamingSession;
@protocol PLCameraStreamingSessionDelegate <NSObject>

@optional
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state;
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetCameraAuthorizationStatus:(PLAuthorizationStatus)status;
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didGetMicrophoneAuthorizationStatus:(PLAuthorizationStatus)status;

@end

@interface PLCameraStreamingSession : NSObject

@property (nonatomic, PL_STRONG) PLCameraStreamingConfiguration *configuration;  // reset will not work until startWithPushURL: invoked.
@property (nonatomic, assign) AVCaptureVideoOrientation    videoOrientation;
@property (nonatomic, PL_WEAK) id<PLCameraStreamingSessionDelegate> delegate;
@property (nonatomic, assign, readonly) PLStreamState               streamState;
@property (nonatomic, assign, readonly) BOOL                        isRunning;

@property (nonatomic, PL_STRONG, readonly) NSURL *pushURL;   // rtmp only now.
@property (nonatomic, PL_WEAK) UIView *previewView;

- (instancetype)initWithConfiguration:(PLCameraStreamingConfiguration *)configuration videoOrientation:(AVCaptureVideoOrientation)videoOrientation; // return nil when has no camera.

// RTMP Operations
- (void)startWithPushURL:(NSURL *)pushURL completed:(void (^)(BOOL success))handler;
- (void)stop;

- (void)updatePreviewViewSize:(CGSize)size; // This will not modify the stream encoding video size

@end

#pragma mark - Category (CameraSource)

@interface PLCameraStreamingSession (CameraSource)

@property (nonatomic, assign) PLCaptureDevicePosition   captureDevicePosition;   // default as AVCaptureDevicePositionBack.
@property (nonatomic, assign, getter=isTorchOn) BOOL    torchOn;                 // default as NO.
@property (nonatomic, assign) CGPoint   focusPointOfInterest;                    // default as (0.5, 0.5), (0,0) is top-left, (1,1) is bottom-right.
@property (nonatomic, assign, getter=isContinuousAutofocusEnable) BOOL  continuousAutofocusEnable;  // default as YES.
@property (nonatomic, assign, getter=isTouchToFocusEnable) BOOL touchToFocusEnable; // default as YES.

- (void)toggleCamera;

- (UIImage *)stillCaptureImage;

@end

#pragma mark - Category (MicrophoneSource)

@interface PLCameraStreamingSession (MicrophoneSource)

@property (nonatomic, assign, getter=isMuted)   BOOL    muted;                   // default as NO.

@end

#pragma mark - Categroy (Application)

@interface PLCameraStreamingSession (Application)

@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end

#pragma mark - Category (Authorization)

@interface PLCameraStreamingSession (Authorization)

// Camera
+ (PLAuthorizationStatus)cameraAuthorizationStatus;
+ (void)requestCameraAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

// Microphone
+ (PLAuthorizationStatus)microphoneAuthorizationStatus;
+ (void)requestMicrophoneAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

@end