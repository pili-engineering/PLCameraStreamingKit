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

/// 设备授权状态
typedef NS_ENUM(NSUInteger, PLAuthorizationStatus) {
    /// 还没有确定是否授权
    PLAuthorizationStatusNotDetermined = 0,
    /// 设备受限，一般在家长模式下设备会受限
    PLAuthorizationStatusRestricted,
    /// 拒绝授权
    PLAuthorizationStatusDenied,
    /// 已授权
    PLAuthorizationStatusAuthorized
};

#pragma mark - Stream State

/// 流状态
typedef NS_ENUM(NSUInteger, PLStreamState) {
    /// 未知状态，初始化时状态被设定为未知
    PLStreamStateUnknow = 0,
    /// 连接中
    PLStreamStateConnecting,
    /// 已连接
    PLStreamStateConnected,
    /// 断开中
    PLStreamStateDisconnecting,
    /// 已断开
    PLStreamStateDisconnected,
    /// 连接出错
    PLStreamStateError
};

#pragma mark - Error

typedef NS_ENUM(NSInteger, PLStreamError) {
    PLStreamErrorUnknow =	-1,
    PLStreamErrorUnknowOption = -999,
    PLStreamErrorAccessDNSFailed = -1000,
    PLStreamErrorFailedToConnectSocket = -1001,
    PLStreamErrorSocksNegotiationFailed = -1002,
    PLStreamErrorFailedToCreateSocket = -1003,
    PLStreamErrorHandshakeFailed = -1004,
    PLStreamErrorRTMPConnectFailed = -1005,
    PLStreamErrorSendFailed = -1006,
    PLStreamErrorServerRequestedClose = -1007,
    PLStreamErrorNetStreamFailed = -1008,
    PLStreamErrorNetStreamPlayFailed = -1009,
    PLStreamErrorNetStreamPlayStreamNotFound = -1010,
    PLStreamErrorNetConnectionConnectInvalidApp = -1011,
    PLStreamErrorSanityFailed = -1012,
    PLStreamErrorSocketClosedByPeer = -1013,
    PLStreamErrorRTMPConnectStreamFailed = -1014,
    
    // SSL errors
    PLStreamErrorTLSConnectFailed = -1200,
    PLStreamErrorNoSSLOrTLSSupport = -1201,
};


#pragma mark - Video Dimension

/// 流编码分辨率
typedef NS_ENUM(NSUInteger, PLStreamingDimension) {
    /// 16 : 9 横纵比，416 x 234 分辨率
    PLStreamingDimension_16_9__416x234,
    /// 16 : 9 横纵比，480 x 270 分辨率
    PLStreamingDimension_16_9__480x270,
    /// 16 : 9 横纵比，640 x 360 分辨率
    PLStreamingDimension_16_9__640x360,
    /// 16 : 9 横纵比，960 x 540 分辨率
    PLStreamingDimension_16_9__960x540,
    /// 16 : 9 横纵比，1280 x 720 分辨率
    PLStreamingDimension_16_9__1280x720,
    /// 16 : 9 横纵比，1920 x 1080 分辨率
    PLStreamingDimension_16_9__1920x1080,
    /// 4 : 3 横纵比，400 x 300 分辨率
    PLStreamingDimension_4_3__400x300,
    /// 4 : 3 横纵比，480 x 360 分辨率
    PLStreamingDimension_4_3__480x360,
    /// 4 : 3 横纵比，640 x 480 分辨率
    PLStreamingDimension_4_3__640x480,
    /// 4 : 3 横纵比，960 x 720 分辨率
    PLStreamingDimension_4_3__960x720,
    /// 4 : 3 横纵比，1280 x 960 分辨率
    PLStreamingDimension_4_3__1280x960,
    /// 4 : 3 横纵比，1920 x 1140 分辨率
    PLStreamingDimension_4_3__1920x1140,
    /// 自定义分辨率
    PLStreamingDimension_UserDefine,
    /// 默认分辨率，默认为 4 : 3 横纵比，640 x 480 分辨率
    PLStreamingDimension_Default = PLStreamingDimension_4_3__640x480,
};

#pragma mark - Video Streaming Quality

/*!
 * @abstract Video streaming quality low 1
 *
 * @discussion 具体参数 fps: 12, profile level: baseline30, video bitrate: 150Kbps
 */
extern NSString *kPLVideoStreamingQualityLow1;

/*!
 * @abstract Video streaming quality low 2
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 264Kbps
 */
extern NSString *kPLVideoStreamingQualityLow2;

/*!
 * @abstract Video streaming quality low 3
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 350Kbps
 */
extern NSString *kPLVideoStreamingQualityLow3;

/*!
 * @abstract Video streaming quality medium 1
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 512Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium1;

/*!
 * @abstract Video streaming quality medium 2
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 800Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium2;

/*!
 * @abstract Video streaming quality medium 3
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 1000Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium3;

/*!
 * @abstract Video streaming quality high 1
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1200Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh1;

/*!
 * @abstract Video streaming quality high 2
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1500Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh2;

/*!
 * @abstract Video streaming quality high 3
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 2000Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh3;

#pragma mark - Audio Streaming Quality

/*!
 * @abstract Audio streaming quality high 1
 *
 * @discussion 具体参数 audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLAudioStreamingQualityHigh1;

/*!
 * @abstract Audio streaming quality high 2
 *
 * @discussion 具体参数 audio sample rate: 44MHz, audio bitrate: 128Kbps
 */
extern NSString *kPLAudioStreamingQualityHigh2;

#pragma mark - Audio BitRate

/// 音频码率
typedef NS_ENUM(NSUInteger, PLStreamingAudioBitRate) {
    /// 64Kbps 音频码率
    PLStreamingAudioBitRate_64Kbps = 64000,
    /// 96Kbps 音频码率
    PLStreamingAudioBitRate_96Kbps = 96000,
    /// 128Kbps 音频码率
    PLStreamingAudioBitRate_128Kbps = 128000,
    /// 默认音频码率，默认为 64Kbps
    PLStreamingAudioBitRate_Default = PLStreamingAudioBitRate_64Kbps
};

#pragma mark - Capture Device Position

/*!
 * @abstract 摄像头位置
 */
typedef NS_ENUM(NSUInteger, PLCaptureDevicePosition) {
    /// 后置摄像头
    PLCaptureDevicePositionBack = 0,
    /// 前置摄像头
    PLCaptureDevicePositionFront
};

#pragma mark - BackgroudMode

typedef NS_ENUM(NSUInteger, PLStreamingBackgroundMode) {
    PLStreamingBackgroundModeAutoStop = 0,
    PLStreamingBackgroundModeKeepAlive,
    PLStreamingBackgroundModeDefault = PLStreamingBackgroundModeAutoStop
};

#pragma mark - Deprecated

/*!
 * @deprecated
 *
 * @abstract Streaming quality low 1
 *
 * @discussion 具体参数 fps: 12, profile level: baseline30, video bitrate: 150Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityLow1 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality low 2
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 264Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityLow2 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality low 3
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 350Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityLow3 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality medium 1
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 512Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityMedium1 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality medium 2
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 800Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityMedium2 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality medium 3
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 1000Kbps, audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLStreamingQualityMedium3 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality high 1
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1200Kbps, audio sample rate: 44MHz, audio bitrate: 128Kbps
 */
extern NSString *kPLStreamingQualityHigh1 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality high 2
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1500Kbps, audio sample rate: 44MHz, audio bitrate: 128Kbps
 */
extern NSString *kPLStreamingQualityHigh2 DEPRECATED_ATTRIBUTE;

/*!
 * @deprecated
 *
 * @abstract Streaming quality high 3
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 2000Kbps, audio sample rate: 44MHz, audio bitrate: 128Kbps
 */
extern NSString *kPLStreamingQualityHigh3 DEPRECATED_ATTRIBUTE;

/// @deprecated 创建 Configuration 时请使用 Quality 参数，该参数已失效
typedef NS_ENUM(NSUInteger, PLStreamingNetworkType) {
    /// @deprecated
    PLStreamingNetworkTypeCELL,
    /// @deprecated
    PLStreamingNetworkTypeWiFi,
    /// @deprecated
    PLStreamingNetworkTypeEither
} DEPRECATED_ATTRIBUTE;

#endif
