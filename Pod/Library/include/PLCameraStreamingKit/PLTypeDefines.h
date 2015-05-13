//
//  PLTypeDefines.h
//  PLCameraStreamingKit
//
//  Created on 15/3/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLCameraStreamingKit_PLTypeDefines_h
#define PLCameraStreamingKit_PLTypeDefines_h

#pragma mark - Authorization

typedef NS_ENUM(NSUInteger, PLAuthorizationStatus) {
    PLAuthorizationStatusNotDetermined = 0,
    PLAuthorizationStatusRestricted,
    PLAuthorizationStatusDenied,
    PLAuthorizationStatusAuthorized
};

#pragma mark - Stream State

typedef NS_ENUM(NSUInteger, PLStreamState) {
    PLStreamStateUnknow = 0,
    PLStreamStateConnecting,
    PLStreamStateConnected,
    PLStreamStateDisconnected,
    PLStreamStateError
};

#pragma mark - Video Dimension

typedef NS_ENUM(NSUInteger, PLStreamingDimension) {
    PLStreamingDimension_16_9__416x234,
    PLStreamingDimension_16_9__480x270,
    PLStreamingDimension_16_9__640x360,
    PLStreamingDimension_16_9__960x540,
    PLStreamingDimension_16_9__1280x720,
    PLStreamingDimension_16_9__1920x1080,
    PLStreamingDimension_4_3__400x300,
    PLStreamingDimension_4_3__480x360,
    PLStreamingDimension_4_3__640x480,
    PLStreamingDimension_4_3__960x720,
    PLStreamingDimension_4_3__1280x960,
    PLStreamingDimension_4_3__1920x1140,
    PLStreamingDimension_UserDefine,
    PLStreamingDimension_Default = PLStreamingDimension_4_3__640x480,
};

#pragma mark - Audio BitRate

typedef NS_ENUM(NSUInteger, PLStreamingAudioBitRate) {
    PLStreamingAudioBitRate_64Kbps,
    PLStreamingAudioBitRate_96Kbps,
    PLStreamingAudioBitRate_128Kbps,
    PLStreamingAudioBitRate_Default = PLStreamingAudioBitRate_64Kbps
};

#pragma mark - Streaming Quality

/*!
 * Streaming quality low 1
 *
 * @discussion fps: 12, profile level: baseline30, video bitrate: 200Kbps
 */
extern NSString *kPLStreamingQualityLow1;

/*!
 * Streaming quality low 2
 *
 * @discussion fps: 15, profile level: baseline30, video bitrate: 400Kbps
 */
extern NSString *kPLStreamingQualityLow2;

/*!
 * Streaming quality medium 1
 *
 * @discussion fps: 30, profile level: baseline30, video bitrate: 600Kbps
 */
extern NSString *kPLStreamingQualityMedium1;

/*!
 * Streaming quality medium 2
 *
 * @discussion fps: 30, profile level: baseline31, video bitrate: 1200Kbps
 */
extern NSString *kPLStreamingQualityMedium2;

/*!
 * Streaming quality medium 3
 *
 * @discussion fps: 30, profile level: main31, video bitrate: 3500Kbps
 */
extern NSString *kPLStreamingQualityMedium3;

/*!
 * Streaming quality high 1
 *
 * @discussion fps: 30, profile level: main31, video bitrate: 5000Kbps
 */
extern NSString *kPLStreamingQualityHigh1;

/*!
 * Streaming quality high 2
 *
 * @discussion fps: 30, profile level: main31, video bitrate: 6500Kbps
 */
extern NSString *kPLStreamingQualityHigh2;

/*!
 * Streaming quality high 3
 *
 * @discussion fps: 30, profile level: high40, video bitrate: 8500Kbps
 */
extern NSString *kPLStreamingQualityHigh3;

#pragma mark - Capture Device Position

typedef NS_ENUM(NSUInteger, PLCaptureDevicePosition) {
    PLCaptureDevicePositionBack = 0,
    PLCaptureDevicePositionFront
};

#pragma mark - Deprecated

typedef NS_ENUM(NSUInteger, PLStreamingNetworkType) {
    PLStreamingNetworkTypeCELL,
    PLStreamingNetworkTypeWiFi,
    PLStreamingNetworkTypeEither
} DEPRECATED_ATTRIBUTE;

#endif
