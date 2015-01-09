//
//  PLCaptureManager.h
//  PLCameraStreamingKit
//
//  Created by 0day on 14/10/29.
//  Copyright (c) 2014年 qgenius. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef  NS_ENUM(NSUInteger, PLCaptureDeviceAuthorizedStatus) {
    PLCaptureDeviceAuthorizedStatusUnknow,
    PLCaptureDeviceAuthorizedStatusGranted,
    PLCaptureDeviceAuthorizedStatusUngranted
};

typedef NS_ENUM(NSUInteger, PLStreamState) {
    PLStreamStateUnknow,
    PLStreamStateConnecting,
    PLStreamStateConnected,
    PLStreamStateDisconnected,
    PLStreamStateError
};

typedef NS_ENUM(NSUInteger, PLStreamBitrateMode) {
    PLStreamBitrateMode_1Mbps,
    PLStreamBitrateMode_500Kbps,
    PLStreamBitrateMode_160Kbps,
    PLStreamBitrateMode_Adaptive,
    PLStreamBitrateMode_Default = PLStreamBitrateMode_1Mbps
};

typedef NS_ENUM(NSUInteger, PLCaptureDevicePosition) {
    PLCaptureDevicePositionBack,
    PLCaptureDevicePositionFront
};

@class PLCaptureManager;
@protocol PLCaptureManagerDelegate <NSObject>

- (void)captureManager:(PLCaptureManager *)manager streamStateDidChange:(PLStreamState)state;

@end


@interface PLCaptureManager : NSObject

@property (nonatomic, assign) PLCaptureDevicePosition   captureDevicePosition;   // default as AVCaptureDevicePositionBack
@property (nonatomic, assign, getter=isMuted)   BOOL    muted;                   // default as NO
@property (nonatomic, assign, getter=isTorchOn) BOOL    torchOn;                 // default as NO

@property (nonatomic, weak) id<PLCaptureManagerDelegate>    delegate;
@property (nonatomic, assign, readonly) PLStreamState streamState;
@property (nonatomic, assign) PLStreamBitrateMode   streamBitrateMode;

@property (nonatomic, strong) NSURL *pushURL;   // rtmp only now.
@property (nonatomic, weak) UIView *previewView;

+ (instancetype)sharedManager;

// Authorize
+ (PLCaptureDeviceAuthorizedStatus)captureDeviceAuthorizedStatus;
+ (void)requestCaptureDeviceAccessWithCompletionHandler:(void (^)(BOOL granted))block;

// Operations
- (void)connect;
- (void)disconnect;

@end
