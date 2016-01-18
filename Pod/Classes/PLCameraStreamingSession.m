//
//  PLCameraStreamingSession.m
//  PLCameraStreamingKit
//
//  Created on 15/4/1.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import "PLCameraStreamingSession.h"

#import "PLCameraSource.h"
#import "PLMicrophoneSource.h"


NSString *PLStreamStateDidChangeNotification = @"PLStream.state.didChange.notification";
NSString *PLCameraAuthorizationStatusDidGetNotificaiton = @"PLStream.camera.authorizationStatus.didGet.notification";
NSString *PLMicrophoneAuthorizationStatusDidGetNotificaiton = @"PLStream.microphone.authorizationStatus.didGet.notification";

NSString *PLCameraDidStartRunningNotificaiton = @"PLStream.camera.state.didStartRunning";
NSString *PLMicrophoneDidStartRunningNotificaiton = @"PLStream.microphone.state.didStartRunning";

@interface PLCameraStreamingSession ()
<
PLCameraSourceDelegate,
PLMicrophoneSourceDelegate,
PLStreamingSessionDelegate,
PLStreamingSendingBufferDelegate
>

@property (nonatomic, PL_STRONG) PLStreamingSession *streamingSession;
@property (nonatomic, PL_STRONG) NSArray    *observers;

@property (nonatomic, PL_STRONG) PLCameraSource *cameraSource;
@property (nonatomic, PL_STRONG) PLMicrophoneSource *microphoneSource;

@property (nonatomic, assign, readwrite)    CGSize videoSize;   // rewrite
@property (nonatomic, assign, readwrite)    int fps;  // rewrite
@property (nonatomic, assign) int64_t bitrate;

@property (nonatomic, PL_STRONG) UITapGestureRecognizer *tapGestureRecognizer;

// Category rewrite
@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;
@property (nonatomic, assign) CGFloat    lowThreshold;
@property (nonatomic, assign) CGFloat    highThreshold;
@property (nonatomic, assign) NSTimeInterval    maxDuration;
@property (nonatomic, assign, readonly) NSTimeInterval    currentDuration;
@property (nonatomic, assign) PLCaptureDevicePosition   captureDevicePosition;
@property (nonatomic, assign, getter=isTorchOn) BOOL    torchOn;
@property (nonatomic, assign) CGPoint   focusPointOfInterest;
@property (nonatomic, assign, getter=isContinuousAutofocusEnable) BOOL  continuousAutofocusEnable;
@property (nonatomic, assign, getter=isTouchToFocusEnable) BOOL touchToFocusEnable;
@property (nonatomic, assign, getter=isMuted)   BOOL    muted;
@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;
@property (nonatomic, assign, getter=isCustomRenderEnable) BOOL customRenderEnable;
@property (nonatomic, assign, getter=isSmoothAutoFocusEnabled) BOOL  smoothAutoFocusEnabled;

@end

@implementation PLCameraStreamingSession {
    // camera source
    CGPoint _focusPointOfInterest;
    BOOL    _continuousAutofocusEnable;
    PLCaptureDevicePosition _captureDevicePosition;
    
    // microphone source
    BOOL    _muted;
    
    // application
    BOOL    _idleTimerDisable;
    BOOL    _cachedIdleTimerDisable;
}

- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration
                        audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration
                                    stream:(PLStream *)stream
                          videoOrientation:(AVCaptureVideoOrientation)videoOrientation {
    self = [super init];
    if (self) {
        self.streamingSession = [[PLStreamingSession alloc] initWithVideoConfiguration:videoConfiguration
                                                                    audioConfiguration:audioConfiguration
                                                                                stream:stream];
        self.streamingSession.delegate = self;
        self.streamingSession.bufferDelegate = self;
        // default value init
        self.videoConfiguration = videoConfiguration;
        self.audioConfiguration = audioConfiguration;
        self.videoOrientation = videoOrientation;
        self.stream = stream;
        
        if ([PLCameraSource hasCameraForPosition:PLCaptureDevicePositionBack]) {
            _captureDevicePosition = PLCaptureDevicePositionBack;
        } else if ([PLCameraSource hasCameraForPosition:PLCaptureDevicePositionFront]) {
            _captureDevicePosition = PLCaptureDevicePositionFront;
        } else {
            return nil;
        }
        
        self.muted = NO;
        self.torchOn = NO;
        self.focusPointOfInterest = CGPointMake(0.5, 0.5);
        self.continuousAutofocusEnable = YES;
        self.touchToFocusEnable = YES;
        self.idleTimerDisable = YES;
        
        [self initCameraSource];
        [self initMicophoneSource];
        
        id observer1 = [[NSNotificationCenter defaultCenter] addObserverForName:UIApplicationDidEnterBackgroundNotification object:nil queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notif) {
            if (self.isRunning) {
                [self stop];
            }
        }];
        
        self.observers = @[observer1];
    }
    
    return self;
}

- (void)stop {
    if (self.microphoneSource.isRunning) {
        [self.microphoneSource stopRunning];
    }
    
    [self.streamingSession stop];
}

- (void)destroy {
    if (self.microphoneSource.isRunning) {
        [self.microphoneSource stopRunning];
    }
    
    [self.streamingSession destroy];
    
    [self.observers enumerateObjectsUsingBlock:^(id observer, NSUInteger idx, BOOL *stop) {
        [[NSNotificationCenter defaultCenter] removeObserver:observer];
    }];
    self.observers = nil;
    
    self.cameraSource = nil;
    self.microphoneSource = nil;
}

- (void)reloadVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration {
    if ([self.videoConfiguration isEqual:videoConfiguration]) {
        return;
    }
    
    self.streamingSession.videoConfiguration = videoConfiguration;
    [self.streamingSession reloadVideoConfiguration:videoConfiguration];
}

#pragma mark - <PLStreamingSessionDelegate>

- (void)streamingSession:(PLStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:streamStateDidChange:)]) {
        [self.delegate cameraStreamingSession:self streamStateDidChange:state];
    }
}

/// @abstract 因产生了某个 error 而断开时的回调
- (void)streamingSession:(PLStreamingSession *)session didDisconnectWithError:(NSError *)error {
    if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:didDisconnectWithError:)]) {
        [self.delegate cameraStreamingSession:self didDisconnectWithError:error];
    }
}

/// @abstract 当开始推流时，会每间隔 3s 调用该回调方法来反馈该 3s 内的流状态，包括视频帧率、音频帧率、音视频总码率
- (void)streamingSession:(PLStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status {
    if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:streamStatusDidUpdate:)]) {
        [self.delegate cameraStreamingSession:self streamStatusDidUpdate:status];
    }
}

#pragma mark - <PLStreamingSendingBufferDelegate>

- (void)streamingSessionSendingBufferFillDidLowerThanLowThreshold:(id)session {
    if ([self.bufferDelegate respondsToSelector:@selector(streamingSessionSendingBufferFillDidLowerThanLowThreshold:)]) {
        [self.bufferDelegate streamingSessionSendingBufferFillDidLowerThanLowThreshold:self];
    }
}

- (void)streamingSessionSendingBufferFillDidHigherThanHighThreshold:(id)session {
    if ([self.bufferDelegate respondsToSelector:@selector(streamingSessionSendingBufferFillDidHigherThanHighThreshold:)]) {
        [self.bufferDelegate streamingSessionSendingBufferFillDidHigherThanHighThreshold:self];
    }
}

- (void)streamingSessionSendingBufferDidEmpty:(id)session {
    if ([self.bufferDelegate respondsToSelector:@selector(streamingSessionSendingBufferDidEmpty:)]) {
        [self.bufferDelegate streamingSessionSendingBufferDidEmpty:self];
    }
}

- (void)streamingSessionSendingBufferDidFull:(id)session {
    if ([self.bufferDelegate respondsToSelector:@selector(streamingSessionSendingBufferDidFull:)]) {
        [self.bufferDelegate streamingSessionSendingBufferDidFull:self];
    }
}

- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items {
    if ([self.bufferDelegate respondsToSelector:@selector(streamingSession:sendingBufferDidDropItems:)]) {
        [self.bufferDelegate streamingSession:self sendingBufferDidDropItems:items];
    }
}

#pragma mark -

- (void)initCameraSource {
    void (^permissionBlock)(void) = ^{
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLCameraAuthorizationStatusDidGetNotificaiton object:nil userInfo:@{@"status": @(PLAuthorizationStatusAuthorized)}];
        });
        
        if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:didGetCameraAuthorizationStatus:)]) {
            [self.delegate cameraStreamingSession:self didGetCameraAuthorizationStatus:PLAuthorizationStatusAuthorized];
        }
        
        if (self.videoConfiguration) {
            self.cameraSource = [[PLCameraSource alloc] initWithVideoConfiguration:self.videoConfiguration
                                                                    cameraPosition:self.captureDevicePosition
                                                                  videoOrientation:self.videoOrientation];
        }
        
        self.cameraSource.delegate = self;
        self.cameraSource.focusPointOfInterest = self.focusPointOfInterest;
        self.cameraSource.continuousAutofocusEnable = self.isContinuousAutofocusEnable;
        
        [self.cameraSource startRunning];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLCameraDidStartRunningNotificaiton object:nil userInfo:nil];
        });
    };
    
    void (^noAccessBlock)(PLAuthorizationStatus status) = ^(PLAuthorizationStatus status) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLCameraAuthorizationStatusDidGetNotificaiton object:nil userInfo:@{@"status": @(status)}];
        });
        if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:didGetCameraAuthorizationStatus:)]) {
            [self.delegate cameraStreamingSession:self didGetCameraAuthorizationStatus:status];
        }
    };
    
    PLAuthorizationStatus status = [PLCameraStreamingSession cameraAuthorizationStatus];
    switch (status) {
        case PLAuthorizationStatusAuthorized:
            permissionBlock();
            break;
        case PLAuthorizationStatusNotDetermined: {
            [PLCameraStreamingSession requestCameraAccessWithCompletionHandler:^(BOOL granted) {
                granted ? permissionBlock() : noAccessBlock(PLAuthorizationStatusDenied);
            }];
        }
            break;
        default:
            noAccessBlock(status);
            break;
    }
}

- (void)initMicophoneSource {
    void (^permissionBlock)(void) = ^{
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLMicrophoneAuthorizationStatusDidGetNotificaiton object:nil userInfo:@{@"status": @(PLAuthorizationStatusAuthorized)}];
        });
        if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:didGetMicrophoneAuthorizationStatus:)]) {
            [self.delegate cameraStreamingSession:self didGetMicrophoneAuthorizationStatus:PLAuthorizationStatusAuthorized];
        }
        
        self.microphoneSource = [[PLMicrophoneSource alloc] initWithSampleRate:self.audioConfiguration.audioSampleRate
                                                              channelsPerFrame:1];
        self.microphoneSource.delegate = self;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLMicrophoneDidStartRunningNotificaiton object:nil userInfo:nil];
        });
    };
    
    void (^noAccessBlock)(PLAuthorizationStatus status) = ^(PLAuthorizationStatus status) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:PLMicrophoneAuthorizationStatusDidGetNotificaiton object:nil userInfo:@{@"status": @(status)}];
        });
        if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:didGetMicrophoneAuthorizationStatus:)]) {
            [self.delegate cameraStreamingSession:self didGetMicrophoneAuthorizationStatus:status];
        }
    };
    
    PLAuthorizationStatus status = [PLCameraStreamingSession microphoneAuthorizationStatus];
    switch (status) {
        case PLAuthorizationStatusAuthorized:
            permissionBlock();
            break;
        case PLAuthorizationStatusNotDetermined: {
            [PLCameraStreamingSession requestMicrophoneAccessWithCompletionHandler:^(BOOL granted) {
                granted ? permissionBlock() : noAccessBlock(PLAuthorizationStatusDenied);
            }];
        }
            break;
        default:
            noAccessBlock(status);
            break;
    }
}

#pragma mark - <PLCameraSourceDelegate>

- (CMSampleBufferRef)cameraSource:(PLCameraSource *)source didGetSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    if ([self.delegate respondsToSelector:@selector(cameraStreamingSession:cameraSourceDidGetSampleBuffer:)]) {
        sampleBuffer = [self.delegate cameraStreamingSession:self cameraSourceDidGetSampleBuffer:sampleBuffer];
    }
    
    if (PLStreamStateConnected == self.streamingSession.streamState) {
        [self.streamingSession pushVideoSampleBuffer:sampleBuffer];
    }
    
    return sampleBuffer;
}

#pragma mark - <PLMicrophoneSourceDelegate>

- (void)microphoneSource:(PLMicrophoneSource *)source didGetAudioBuffer:(AudioBuffer *)buffer {
    if (PLStreamStateConnected == self.streamingSession.streamState) {
        [self.streamingSession pushAudioBuffer:buffer];
    }
}

#pragma mark -

- (void)previewViewTaped:(UITapGestureRecognizer *)tap {
    UIView *view = tap.view;
    CGPoint location = [tap locationInView:view];
    CGPoint focusPoint = (CGPoint){location.x / CGRectGetWidth(view.frame), location.y / CGRectGetHeight(view.frame)};
    
    self.focusPointOfInterest = focusPoint;
}

#pragma mark - Property

- (CGFloat)lowThreshold {
    return self.streamingSession.lowThreshold;
}

- (void)setLowThreshold:(CGFloat)lowThreshold {
    [self willChangeValueForKey:@"lowThreshold"];
    self.streamingSession.lowThreshold = lowThreshold;
    [self didChangeValueForKey:@"lowThreshold"];
}

- (CGFloat)highThreshold {
    return self.streamingSession.highThreshold;
}

- (void)setHighThreshold:(CGFloat)highThreshold {
    [self willChangeValueForKey:@"highThreshold"];
    self.streamingSession.highThreshold = highThreshold;
    [self didChangeValueForKey:@"highThreshold"];
}

- (NSUInteger)maxCount {
    return self.streamingSession.maxCount;
}

- (void)setMaxCount:(NSUInteger)maxCount {
    [self willChangeValueForKey:@"maxCount"];
    self.streamingSession.maxCount = maxCount;
    [self didChangeValueForKey:@"maxCount"];
}

- (NSUInteger)currentCount {
    return self.streamingSession.currentCount;
}

- (PLStream *)stream {
    return self.streamingSession.stream;
}

- (void)setStream:(PLStream *)stream {
    [self willChangeValueForKey:@"stream"];
    
    self.streamingSession.stream = stream;
    
    [self didChangeValueForKey:@"stream"];
}

- (void)setPreviewView:(UIView *)previewView {
    if (_previewView == previewView) {
        return;
    }
    
    [self willChangeValueForKey:@"previewView"];
    _previewView = previewView;
    
    if (self.isCustomRenderEnable) {
        if (self.cameraSource.previewView.superview) {
            [self.cameraSource.previewView removeFromSuperview];
        }
        self.cameraSource.previewView.frame = previewView.bounds;
        [previewView insertSubview:self.cameraSource.previewView atIndex:0];
    } else {
        if (self.cameraSource.previewLayer.superlayer) {
            [self.cameraSource.previewLayer removeFromSuperlayer];
        }
        self.cameraSource.previewLayer.frame = previewView.bounds;
        [previewView.layer insertSublayer:self.cameraSource.previewLayer atIndex:0];
    }
    
    
    
    if (self.isTouchToFocusEnable && !self.tapGestureRecognizer) {
        previewView.userInteractionEnabled = YES;
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(previewViewTaped:)];
        [previewView addGestureRecognizer:tap];
        self.tapGestureRecognizer = tap;
    }
    
    [self didChangeValueForKey:@"previewView"];
}

- (void)setTouchToFocusEnable:(BOOL)touchToFocusEnable {
    [self willChangeValueForKey:@"touchToFocusEnable"];
    
    _touchToFocusEnable = touchToFocusEnable;
    if (touchToFocusEnable && !self.tapGestureRecognizer && self.previewView) {
        self.previewView.userInteractionEnabled = YES;
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(previewViewTaped:)];
        [self.previewView addGestureRecognizer:tap];
        self.tapGestureRecognizer = tap;
    } else if (touchToFocusEnable && self.tapGestureRecognizer) {
        self.tapGestureRecognizer.enabled = YES;
    } else if (!touchToFocusEnable && self.tapGestureRecognizer) {
        self.tapGestureRecognizer.enabled = NO;
    }
    
    [self didChangeValueForKey:@"touchToFocusEnable"];
}

- (PLCaptureDevicePosition)captureDevicePosition {
    return _captureDevicePosition;
}

- (void)setCaptureDevicePosition:(PLCaptureDevicePosition)captureDevicePosition {
    [self willChangeValueForKey:@"captureDevicePosition"];
    self.cameraSource.cameraPosition = captureDevicePosition;
    _captureDevicePosition = self.cameraSource.cameraPosition;
    [self didChangeValueForKey:@"captureDevicePosition"];
}

- (BOOL)isTorchOn {
    return self.cameraSource.isTorchOn;
}

- (void)setTorchOn:(BOOL)torchOn {
    [self willChangeValueForKey:@"torchOn"];
    self.cameraSource.torchOn = torchOn;
    [self didChangeValueForKey:@"torchOn"];
}

- (void)setVideoOrientation:(AVCaptureVideoOrientation)videoOrientation {
    [self willChangeValueForKey:@"videoOrientation"];
    _videoOrientation = videoOrientation;
    self.cameraSource.videoOrientation = videoOrientation;
    [self didChangeValueForKey:@"videoOrientation"];
}

- (void)setFocusPointOfInterest:(CGPoint)focusPointOfInterest {
    [self willChangeValueForKey:@"focusPointOfInterest"];
    _focusPointOfInterest = focusPointOfInterest;
    self.cameraSource.focusPointOfInterest = focusPointOfInterest;
    [self didChangeValueForKey:@"focusPointOfInterest"];
}

- (void)setContinuousAutofocusEnable:(BOOL)continuousAutofocusEnable {
    [self willChangeValueForKey:@"continuousAutofocusEnable"];
    _continuousAutofocusEnable = continuousAutofocusEnable;
    self.cameraSource.continuousAutofocusEnable = continuousAutofocusEnable;
    [self didChangeValueForKey:@"continuousAutofocusEnable"];
}

- (void)setMuted:(BOOL)muted {
    [self willChangeValueForKey:@"muted"];
    _muted = muted;
    self.microphoneSource.muted = muted;
    [self didChangeValueForKey:@"muted"];
}

- (BOOL)isSmoothAutoFocusEnabled {
    return self.cameraSource.isSmoothAutoFocusEnabled;
}

- (void)setSmoothAutoFocusEnabled:(BOOL)smoothAutoFocusEnabled {
    self.cameraSource.smoothAutoFocusEnabled = smoothAutoFocusEnabled;
}

- (BOOL)isRunning {
    return self.streamingSession.isRunning;
}

- (PLStreamState)streamState {
    return self.streamingSession.streamState;
}

- (NSTimeInterval)statusUpdateInterval {
    return self.streamingSession.statusUpdateInterval;
}

- (void)setStatusUpdateInterval:(NSTimeInterval)statusUpdateInterval {
    self.streamingSession.statusUpdateInterval = statusUpdateInterval;
}

#pragma mark - RTMP Operations

- (void)startWithCompleted:(void (^)(BOOL success))handler {
    if (self.streamingSession.isRunning) {
        return;
    }
    
    [self makeIdleTimerDisableEffect];
    
    if (!self.microphoneSource.isRunning) {
        [self.microphoneSource startRunning];
    }
    
    [self.streamingSession startWithCompleted:handler];
}

#pragma mark -

- (void)updatePreviewViewSize:(CGSize)size {
    self.cameraSource.previewLayer.frame = (CGRect){{0, 0}, size};
}

#pragma mark - Category (CameraSource)

- (void)toggleCamera {
    [self.cameraSource toggleCamera];
}

- (void)startCaptureSession {
    [self.cameraSource startRunning];
}

- (void)stopCaptureSession {
    [self.cameraSource stopRunning];
}

#pragma mark - Categroy (Application)

- (void)makeIdleTimerDisableEffect {
    _cachedIdleTimerDisable = [UIApplication sharedApplication].isIdleTimerDisabled;
    [UIApplication sharedApplication].idleTimerDisabled = self.idleTimerDisable;
}

- (void)restoreIdleTimerDisable {
    [UIApplication sharedApplication].idleTimerDisabled = _cachedIdleTimerDisable;
}

#pragma mark - Category (Authorization)

+ (PLAuthorizationStatus)cameraAuthorizationStatus {
    return [PLCameraSource deviceAuthorizationStatus];
}

+ (void)requestCameraAccessWithCompletionHandler:(void (^)(BOOL granted))handler {
    [PLCameraSource requestDeviceAccessWithCompletionHandler:handler];
}

+ (PLAuthorizationStatus)microphoneAuthorizationStatus {
    return [PLMicrophoneSource deviceAuthorizationStatus];
}

+ (void)requestMicrophoneAccessWithCompletionHandler:(void (^)(BOOL granted))handler {
    [PLMicrophoneSource requestDeviceAccessWithCompletionHandler:handler];
}

@end
