//
//  PLStreamStatus.h
//  PLStreamingKit
//
//  Created by 0dayZh on 16/1/9.
//  Copyright © 2016年 pili-engineering. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
    @class      PLStreamStatus
    @abstract   流的实时状态信息。
 
    @discussion 通过实现 PLStreamingSession 对象的 PLStreamingSessionDelegate 代理协议，并实现 -streamingSession:streamStatusDidUpdate: 方法，
                可以获取到流的实时状态信息。
 
    @see        PLStreamingSessionDelegate
 
    @since      v1.1.1
 */
@interface PLStreamStatus : NSObject <NSCopying>

/*!
    @property   videoFPS
    @abstract   视频的帧率。
 
    @since      v1.1.1
 */
@property (nonatomic, assign, readonly) double  videoFPS;

/*!
    @property   audioFPS
    @abstract   音频的帧率。

    @since      v1.1.1
 */
@property (nonatomic, assign, readonly) double  audioFPS;

/*!
    @property   totalBitrate
    @abstract   音频和视频总共的码率。

    @discussion 该属性的单位为 bps(Bits per Seccond)。在使用时请注意 Bit 和 Byte 的区别，不要混淆。

    @since      v1.1.1
 */
@property (nonatomic, assign, readonly) double  totalBitrate;

/*!
    @method     initWithVideoFPS:audioFPS:totalBitrate:
    @abstract   初始化 PLStreamStatus 对象。
 
    @discussion 一般你不需要自己创建或初始化 PLStreamStatus 对象，该类的实例都是在创建并初始化好后通过  PLStreamingSessionDelegate 代理的
                -streamingSession:streamStatusDidUpdate: 方法回调给开发者。
 
    @see        PLStreamingSessionDelegate
 
    @since      v1.1.1
 */
- (instancetype)initWithVideoFPS:(double)vfps
                        audioFPS:(double)afps
                    totalBitrate:(double)totalBitrate;

@end
