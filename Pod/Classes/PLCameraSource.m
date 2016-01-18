//
//  PLCameraSource.m
//  PLCameraStreamingKit
//
//  Created by 0day on 15/3/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "PLCameraSource.h"

GLint uniform;
enum {
    ATTRIB_VERTEX,
    ATTRIB_TEXTUREPOSITON,
    NUM_ATTRIBUTES
};

@interface PLCameraSource ()
<
AVCaptureVideoDataOutputSampleBufferDelegate
>

@property (nonatomic, PL_STRONG) AVCaptureSession   *captureSession;
@property (nonatomic, PL_STRONG) AVCaptureDevice    *captureDevice;
@property (nonatomic, assign) BOOL  cameraToggling;

@property (nonatomic, readwrite, assign) BOOL isRunning;    // rewrite
@property (nonatomic, readwrite, PL_WEAK) AVCaptureVideoPreviewLayer  *previewLayer;    // rewrite

@end

@implementation PLCameraSource

+ (BOOL)hasCameraForPosition:(PLCaptureDevicePosition)cameraPosition {
    AVCaptureDevicePosition position = AVCaptureDevicePositionBack;
    if (PLCaptureDevicePositionFront == cameraPosition) {
        position = AVCaptureDevicePositionFront;
    }
    AVCaptureDevice *result = nil;
    
    // find capture device
    NSArray *devices = [AVCaptureDevice devices];
    for (AVCaptureDevice *device in devices) {
        if ([device hasMediaType:AVMediaTypeVideo] && device.position == position) {
            result = device;
            break;
        }
    }
    
    return !!result;
}

- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)configuration
                            cameraPosition:(PLCaptureDevicePosition)cameraPosition
                          videoOrientation:(AVCaptureVideoOrientation)videoOrientation {
    self = [super init];
    if (self) {
        self.videoConfiguration = configuration;
        _cameraPosition = cameraPosition;
        self.isRunning = NO;
        self.torchOn = NO;
        _videoOrientation = videoOrientation;
        self.cameraToggling = NO;
        
        AVCaptureDevicePosition position = AVCaptureDevicePositionBack;
        if (PLCaptureDevicePositionFront == cameraPosition) {
            position = AVCaptureDevicePositionFront;
        }
        
        // find capture device
        NSArray *devices = [AVCaptureDevice devices];
        for (AVCaptureDevice *device in devices) {
            if ([device hasMediaType:AVMediaTypeVideo]) {
                if (device.position == position) {
                    self.captureDevice = device;
                }
                
                NSError *error;
                [device lockForConfiguration:&error];
                if (PL_SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
                    device.activeVideoMinFrameDuration = CMTimeMake(1, self.videoConfiguration.videoFrameRate);
                    device.activeVideoMaxFrameDuration = CMTimeMake(1, self.videoConfiguration.videoFrameRate);
                }
                [device unlockForConfiguration];
            }
        }
        
        if (nil == self.captureDevice) {
            // No capture device found.
            NSLog(@"CameraSource: There is no camera found in position: %@", (PLCaptureDevicePositionFront == cameraPosition) ? @"front" : @"back");
            
            return nil;
        }
        
        AVCaptureSession *captureSession = [[AVCaptureSession alloc] init];
        AVCaptureDeviceInput *input = nil;
        AVCaptureVideoDataOutput *output = nil;
        
        input = [[AVCaptureDeviceInput alloc] initWithDevice:self.captureDevice error:nil];
        output = [[AVCaptureVideoDataOutput alloc] init];
        
        captureSession.sessionPreset = AVCaptureSessionPresetHigh;
        
        // setup output
        output.videoSettings = @{(NSString *)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)};
        
        dispatch_queue_t cameraQueue = dispatch_queue_create("com.pili.camera", 0);
        [output setSampleBufferDelegate:self queue:cameraQueue];
        
        // add input && output
        if ([captureSession canAddInput:input]) {
            [captureSession addInput:input];
        }
        
        if ([captureSession canAddOutput:output]) {
            [captureSession addOutput:output];
        }
        
        [self refreshFPS];
        
        self.captureSession = captureSession;
        
        self.smoothAutoFocusEnabled = YES;
        [self reorientCamera];
    }
    
    return self;
}

- (void)refreshFPS {
    NSError *error = nil;
    AVCaptureDevice *captureDevice = self.captureDevice;
    if (![captureDevice lockForConfiguration:&error]) {
        NSLog(@"fail to lockForConfiguration: %@",error.localizedDescription);
    } else {
        NSUInteger frameRate = self.videoConfiguration.videoFrameRate;
        AVFrameRateRange *range = [captureDevice.activeFormat.videoSupportedFrameRateRanges firstObject];
        if (frameRate < range.maxFrameRate && frameRate > range.minFrameRate) {
            if ([captureDevice respondsToSelector:@selector(activeVideoMaxFrameDuration)]) {
                captureDevice.activeVideoMaxFrameDuration = CMTimeMake(1, (int32_t)frameRate);
                captureDevice.activeVideoMinFrameDuration = CMTimeMake(1, (int32_t)frameRate);
            }
        }
        
        [captureDevice unlockForConfiguration];
    }
}

- (void)dealloc {
    if (self.isRunning) {
        [self stopRunning];
    }
}

- (AVCaptureDevice *)cameraWithPosition:(AVCaptureDevicePosition)pos {
    AVCaptureDevicePosition position = (AVCaptureDevicePosition)pos;
    
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices) {
        if ([device position] == position) {
            return device;
        }
    }
    
    return nil;
}

- (void)setCameraPosition:(PLCaptureDevicePosition)cameraPosition {
    if (cameraPosition == _cameraPosition) {
        return;
    }
    
    if ([PLCameraSource hasCameraForPosition:cameraPosition]) {
        [self toggleCamera];
    }
}

- (void)setVideoOrientation:(AVCaptureVideoOrientation)videoOrientation {
    _videoOrientation = videoOrientation;
    self.previewLayer.connection.videoOrientation = videoOrientation;
    [self reorientCamera];
}

- (void)reorientCamera {
    if (!self.captureSession) {
        return;
    }
    
    AVCaptureSession* session = (AVCaptureSession *)self.captureSession;
    
    for (AVCaptureVideoDataOutput* output in session.outputs) {
        for (AVCaptureConnection * av in output.connections) {
            av.videoOrientation = self.videoOrientation;
            if (self.cameraPosition == PLCaptureDevicePositionFront) {
                if (av.supportsVideoMirroring) {
                    av.videoMirrored = YES;
                }
            }
        }
    }
}

- (void)toggleCamera {
    if(!self.captureSession)
        return;
    
    if (self.cameraToggling) {
        return;
    }
    self.cameraToggling = YES;
    
    AVCaptureSession* session = self.captureSession;
    
    PLCaptureDevicePosition newPosition = PLCaptureDevicePositionFront;
    if (self.cameraPosition == PLCaptureDevicePositionFront) {
        newPosition = PLCaptureDevicePositionBack;
    }
    if (![PLCameraSource hasCameraForPosition:newPosition]) {
        return;
    }
    
    [session beginConfiguration];
    
    AVCaptureInput *currentCameraInput = [session.inputs objectAtIndex:0];
    [session removeInput:currentCameraInput];
    
    AVCaptureDevice *newCamera = nil;
    if (((AVCaptureDeviceInput*)currentCameraInput).device.position == AVCaptureDevicePositionBack) {
        newCamera = [self cameraWithPosition:AVCaptureDevicePositionFront];
    } else {
        newCamera = [self cameraWithPosition:AVCaptureDevicePositionBack];
    }
    
    AVCaptureDeviceInput *newVideoInput = [[AVCaptureDeviceInput alloc] initWithDevice:newCamera error:nil];
    [session addInput:newVideoInput];
    
    [session commitConfiguration];
    
    self.captureDevice = newCamera;
    [self refreshFPS];
    
    _cameraPosition = newPosition;
    
    [self reorientCamera];
    
    __weak typeof(self) wself = self;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        __strong typeof(wself) strongSelf = wself;
        strongSelf.cameraToggling = NO;
    });
}

- (void)startRunning {
    NSLog(@"CameraSource: startRunning");
    [self.captureSession startRunning];
    self.isRunning = YES;
}

- (void)stopRunning {
    NSLog(@"CameraSource: stopRunning");
    [self.captureSession stopRunning];
    self.isRunning = NO;
}

- (void)reloadVideoConfiguration {
    NSArray *devices = [AVCaptureDevice devices];
    for (AVCaptureDevice *device in devices) {
        if ([device hasMediaType:AVMediaTypeVideo]) {
            
            NSError *error;
            [device lockForConfiguration:&error];
            if (PL_SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
                device.activeVideoMinFrameDuration = CMTimeMake(1, self.videoConfiguration.videoFrameRate);
                device.activeVideoMaxFrameDuration = CMTimeMake(1, self.videoConfiguration.videoFrameRate);
            }
            [device unlockForConfiguration];
        }
    }
}

#pragma mark - Property

- (UIView *)previewView {
    if (!_previewView) {
        _previewView = [[UIImageView alloc] initWithFrame:(CGRect){{0, 0}, self.videoConfiguration.videoSize}];
    }
    
    return _previewView;
}

- (AVCaptureVideoPreviewLayer *)previewLayer {
    if (!_previewLayer) {
        AVCaptureSession* session = (AVCaptureSession*)_captureSession;
        AVCaptureVideoPreviewLayer* previewLayer;
        previewLayer =  [AVCaptureVideoPreviewLayer layerWithSession:session];
        previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
        _previewLayer = previewLayer;
    }
    
    return _previewLayer;
}

- (void)setFocusPointOfInterest:(CGPoint)focusPointOfInterest {
    _focusPointOfInterest = focusPointOfInterest;
    
    AVCaptureDevice *device = (AVCaptureDevice *)self.captureDevice;
    bool ret = device.focusPointOfInterestSupported;
    
    if (ret) {
        NSError *err = nil;
        if ([device lockForConfiguration:&err]) {
            [device setFocusPointOfInterest:focusPointOfInterest];
            device.focusMode = device.focusMode;
            [device unlockForConfiguration];
        } else {
            NSLog(@"Error while locking device for focus POI: %@", err);
            ret = false;
        }
    } else {
        NSLog(@"Focus POI not supported");
    }
}

- (void)setContinuousAutofocusEnable:(BOOL)continuousAutofocusEnable {
    _continuousAutofocusEnable = continuousAutofocusEnable;
    
    AVCaptureDevice *device = (AVCaptureDevice *)self.captureDevice;
    AVCaptureFocusMode newMode = continuousAutofocusEnable ?  AVCaptureFocusModeContinuousAutoFocus : AVCaptureFocusModeAutoFocus;
    bool ret = [device isFocusModeSupported:newMode];
    
    if (ret) {
        NSError *err = nil;
        if ([device lockForConfiguration:&err]) {
            device.focusMode = newMode;
            [device unlockForConfiguration];
        } else {
            NSLog(@"Error while locking device for autofocus: %@", err);
            ret = false;
        }
    } else {
        NSLog(@"Focus mode not supported: %@", continuousAutofocusEnable ? @"AVCaptureFocusModeContinuousAutoFocus" : @"AVCaptureFocusModeAutoFocus");
    }
}

- (void)setSmoothAutoFocusEnabled:(BOOL)smoothAutoFocusEnabled {
    if (smoothAutoFocusEnabled == _smoothAutoFocusEnabled) {
        return;
    }
    _smoothAutoFocusEnabled = smoothAutoFocusEnabled;
    
    AVCaptureDevice *device = (AVCaptureDevice *)self.captureDevice;
    if (device.isSmoothAutoFocusSupported) {
        NSError *error = nil;
        [device lockForConfiguration:&error];
        device.smoothAutoFocusEnabled = smoothAutoFocusEnabled;
        [device unlockForConfiguration];
    }
}

- (void)setTorchOn:(BOOL)torchOn {
    bool ret = false;
    if (!self.captureSession || torchOn == _torchOn) {
        return ;
    }
    
    AVCaptureSession* session = (AVCaptureSession *)self.captureSession;
    
    [session beginConfiguration];
    AVCaptureDeviceInput* currentCameraInput = [session.inputs objectAtIndex:0];
    
    if(currentCameraInput.device.torchAvailable) {
        NSError* err = nil;
        if([currentCameraInput.device lockForConfiguration:&err]) {
            [currentCameraInput.device setTorchMode:( torchOn ? AVCaptureTorchModeOn : AVCaptureTorchModeOff ) ];
            [currentCameraInput.device unlockForConfiguration];
            ret = (currentCameraInput.device.torchMode == AVCaptureTorchModeOn);
        } else {
            NSLog(@"Error while locking device for torch: %@", err);
            ret = false;
        }
    } else {
        NSLog(@"Torch not available in current camera input");
    }
    [session commitConfiguration];
    
    _torchOn = ret;
}

#pragma mark - <AVCaptureVideoDataOutputSampleBufferDelegate>

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    if ([self.delegate respondsToSelector:@selector(cameraSource:didGetSampleBuffer:)]) {
        sampleBuffer = [self.delegate cameraSource:self didGetSampleBuffer:sampleBuffer];
    }
}

#pragma mark - <PLSourceAccessProtocol>

+ (PLAuthorizationStatus)deviceAuthorizationStatus {
    return (PLAuthorizationStatus)[AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
}

+ (void)requestDeviceAccessWithCompletionHandler:(void (^)(BOOL granted))handler {
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
        if (handler) {
            dispatch_sync(dispatch_get_main_queue(), ^{
                handler(granted);
            });
        }
    }];
}

@end
