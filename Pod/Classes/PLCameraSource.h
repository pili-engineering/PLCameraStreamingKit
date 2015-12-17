//
//  PLCameraSource.h
//  PLCameraStreamingKit
//
//  Created by 0day on 15/3/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <PLStreamingKit/PLStreamingKit.h>

#import "PLSourceAccessProtocol.h"
#import "PLTypeDefines.h"
#import "PLMacroDefines.h"

/*!
 * @abstract 摄像头位置
 */
typedef NS_ENUM(NSUInteger, PLCaptureDevicePosition) {
    /// 后置摄像头
    PLCaptureDevicePositionBack = 0,
    /// 前置摄像头
    PLCaptureDevicePositionFront
};

@class PLCameraSource;
@protocol PLCameraSourceDelegate <NSObject>

@optional
- (CMSampleBufferRef)cameraSource:(PLCameraSource *)source didGetSampleBuffer:(CMSampleBufferRef)sampleBuffer;

@end

@interface PLCameraSource : NSObject
<
PLSourceAccessProtocol
>

@property (nonatomic, PL_WEAK) id<PLCameraSourceDelegate>   delegate;

@property (nonatomic, PL_WEAK) PLVideoStreamingConfiguration    *videoConfiguration;
@property (nonatomic, readwrite, assign) PLCaptureDevicePosition  cameraPosition;
@property (nonatomic, readwrite, assign, getter=isTorchOn) BOOL torchOn;
@property (nonatomic, readonly, PL_WEAK) AVCaptureVideoPreviewLayer  *previewLayer;
@property (nonatomic, PL_STRONG) UIView *previewView;
@property (nonatomic, readonly, assign) BOOL isRunning;
@property (nonatomic, assign) AVCaptureVideoOrientation videoOrientation;
@property (nonatomic, assign) CGPoint   focusPointOfInterest;
@property (nonatomic, assign) BOOL  continuousAutofocusEnable;
@property (nonatomic, assign, getter=isSmoothAutoFocusEnabled) BOOL  smoothAutoFocusEnabled;

+ (BOOL)hasCameraForPosition:(PLCaptureDevicePosition)position;

- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)configuration
                            cameraPosition:(PLCaptureDevicePosition)cameraPosition
                          videoOrientation:(AVCaptureVideoOrientation)videoOrientation;

- (void)toggleCamera;

- (void)startRunning;
- (void)stopRunning;

- (void)reloadVideoConfiguration;

@end