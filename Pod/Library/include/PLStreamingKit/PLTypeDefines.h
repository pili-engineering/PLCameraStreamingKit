//
//  PLTypeDefines.h
//  PLCameraStreamingKit
//
//  Created on 15/3/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLCameraStreamingKit_PLTypeDefines_h
#define PLCameraStreamingKit_PLTypeDefines_h

#pragma mark - Stream State

/*!
    @typedef    PLStreamState
    @abstract   PLStreamingSessin 的流状态。
    @since      v1.0.0
 */
typedef NS_ENUM(NSUInteger, PLStreamState) {
    /// PLStreamStateUnknow 未知状态，只会作为 init 的初始状态
    PLStreamStateUnknow = 0,
    /// PLStreamStateConnecting 连接中状态
    PLStreamStateConnecting,
    /// PLStreamStateConnected 已连接状态
    PLStreamStateConnected,
    /// PLStreamStateDisconnecting 断开连接中状态
    PLStreamStateDisconnecting,
    /// PLStreamStateDisconnected 已断开连接状态
    PLStreamStateDisconnected,
    /// PLStreamStateError 错误状态
    PLStreamStateError
};

#pragma mark - Error

/*!
    @typedef    PLStreamError
    @abstract   错误状态码。

    @constant   PLStreamErrorUnknow 未知错误
    @constant   PLStreamErrorUnknowOption rtmp 推流未知配置
    @constant   PLStreamErrorAccessDNSFailed dns 无法访问
    @constant   PLStreamErrorFailedToConnectSocket socket 连接失败
    @constant   PLStreamErrorSocksNegotiationFailed sockket negotiation 失败
    @constant   PLStreamErrorFailedToCreateSocket 创建 socket 失败
    @constant   PLStreamErrorHandshakeFailed    握手失败
    @constant   PLStreamErrorRTMPConnectFailed  rtmp 连接失败
    @constant   PLStreamErrorSendFailed 发送数据包失败
    @constant   PLStreamErrorServerRequestedClose   被服务端关闭
    @constant   PLStreamErrorNetStreamFailed    NetStream 失败
    @constant   PLStreamErrorNetStreamPlayFailed    NetStreamPlay 失败
    @constant   PLStreamErrorNetStreamPlayStreamNotFound    NetStreamPlay 为找到对应的流
    @constant   PLStreamErrorNetConnectionConnectInvalidApp 连接到无效的 rtmp 应用
    @constant   PLStreamErrorSanityFailed Sanity 失败
    @constant   PLStreamErrorSocketClosedByPeer Socket 被关闭
    @constant   PLStreamErrorRTMPConnectStreamFailed    rtmp 连接流失败
    @constant   PLStreamErrorSocketTimeout  socket 超时
    @constant   PLStreamErrorTLSConnectFailed   TLS 连接失败
    @constant   PLStreamErrorNoSSLOrTLSSupport  没有 SSL 或者 TLS 支持
    @constant   PLStreamErrorDNSResolveFailed   DNS 解析失败
 
    @since      v1.0.0
 */
typedef enum {
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
    PLStreamErrorSocketTimeout = -1015,
    
    // SSL errors
    PLStreamErrorTLSConnectFailed = -1200,
    PLStreamErrorNoSSLOrTLSSupport = -1201,
    
    // DNS error
    PLStreamErrorDNSResolveFailed = -1300,
    
    // reconnect error
    PLStreamErrorReconnectFailed = -1400,
} PLStreamError;

#pragma mark - Video Streaming Quality

/*!
    @constant   kPLVideoStreamingQualityLow1
    @abstract   视频编码推流质量 low 1。

    @discussion 具体参数 fps: 12, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 150Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityLow1;

/*!
    @constant   kPLVideoStreamingQualityLow1
    @abstract   视频编码推流质量 low 2。

    @discussion 具体参数 fps: 15, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 264Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityLow2;

/*!
    @constant   kPLVideoStreamingQualityLow3
    @abstract   视频编码推流质量 low 3。

    @discussion 具体参数 fps: 15, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 350Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityLow3;

/*!
    @constant   kPLVideoStreamingQualityMedium1
    @abstract   视频编码推流质量 medium 1。
 
    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 512Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityMedium1;

/*!
    @constant   kPLVideoStreamingQualityMedium2
    @abstract   视频编码推流质量 medium 2。

    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 800Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityMedium2;

/*!
    @constant   kPLVideoStreamingQualityMedium3
    @abstract   视频编码推流质量 medium 3。

    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 1000Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityMedium3;

/*!
    @constant   kPLVideoStreamingQualityHigh1
    @abstract   视频编码推流质量 high 1。

    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 1200Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityHigh1;

/*!
    @constant   kPLVideoStreamingQualityHigh2
    @abstract   视频编码推流质量 high 2。

    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 1500Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityHigh2;

/*!
    @constant   kPLVideoStreamingQualityHigh3
    @abstract   视频编码推流质量 high 3。

    @discussion 具体参数 fps: 30, profile level: AVVideoProfileLevelH264Baseline31, video bitrate: 2000Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLVideoStreamingQualityHigh3;

#pragma mark - Audio Streaming Quality

/*!
    @constant   kPLAudioStreamingQualityHigh1
    @abstract   音频编码推流质量 high 1。

    @discussion 具体参数 audio bitrate: 64Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh1;

/*!
    @constant   kPLAudioStreamingQualityHigh2
    @abstract   音频编码推流质量 high 2。

    @discussion 具体参数 audio bitrate: 96Kbps。
 
    @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh2;

/*!
 @constant   kPLAudioStreamingQualityHigh3
 @abstract   音频编码推流质量 high 3。
 
 @discussion 具体参数 audio bitrate: 128Kbps。
 
 @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh3;

#pragma mark - Audio BitRate

/*!
    @typedef    PLStreamingAudioBitRate
    @abstract   音频编码码率。
 
    @constant   PLStreamingAudioBitRate_64Kbps 64Kbps 音频码率
    @constant   PLStreamingAudioBitRate_96Kbps 96Kbps 音频码率
    @constant   PLStreamingAudioBitRate_128Kbps 128Kbps 音频码率

    @since      v1.0.0
 */
typedef enum {
    PLStreamingAudioBitRate_64Kbps = 64000,
    PLStreamingAudioBitRate_96Kbps = 96000,
    PLStreamingAudioBitRate_128Kbps = 128000,
} PLStreamingAudioBitRate;

#endif
