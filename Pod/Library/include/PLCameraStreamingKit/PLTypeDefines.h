//
//  PLTypeDefines.h
//  PLCameraStreamingKit
//
//  Created on 15/3/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLCameraStreamingKit_PLTypeDefines_h
#define PLCameraStreamingKit_PLTypeDefines_h

typedef NS_ENUM(NSUInteger, PLAuthorizationStatus) {
    PLAuthorizationStatusNotDetermined = 0,
    PLAuthorizationStatusRestricted,
    PLAuthorizationStatusDenied,
    PLAuthorizationStatusAuthorized
};

typedef NS_ENUM(NSUInteger, PLStreamState) {
    PLStreamStateUnknow = 0,
    PLStreamStateConnecting,
    PLStreamStateConnected,
    PLStreamStateDisconnected,
    PLStreamStateError
};

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
    PLStreamingDimension_Default = PLStreamingDimension_4_3__640x480
};

typedef NS_ENUM(NSUInteger, PLStreamingAudioBitRate) {
    PLStreamingAudioBitRate_64Kbps,
    PLStreamingAudioBitRate_96Kbps,
    PLStreamingAudioBitRate_128Kbps,
    PLStreamingAudioBitRate_Default = PLStreamingAudioBitRate_64Kbps
};

typedef NS_ENUM(NSUInteger, PLStreamingNetworkType) {
    PLStreamingNetworkTypeCELL,
    PLStreamingNetworkTypeWiFi,
    PLStreamingNetworkTypeEither
};

typedef NS_ENUM(NSUInteger, PLStreamBitrateMode) {
    PLStreamBitrateMode_200Kbps,
    PLStreamBitrateMode_400Kbps,
    PLStreamBitrateMode_600Kbps,
    PLStreamBitrateMode_1200Kbps,
    PLStreamBitrateMode_3500Kbps,
    PLStreamBitrateMode_5000Kbps,
    PLStreamBitrateMode_6500Kbps,
    PLStreamBitrateMode_8500Kbps,
    PLStreamBitrateMode_Default = PLStreamBitrateMode_600Kbps
};

typedef NS_ENUM(NSUInteger, PLCaptureDevicePosition) {
    PLCaptureDevicePositionBack = 0,
    PLCaptureDevicePositionFront
};

#endif
