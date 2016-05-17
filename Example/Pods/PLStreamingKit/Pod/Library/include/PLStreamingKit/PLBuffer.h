//
//  PLBuffer.h
//  PLCameraStreamingKit
//
//  Created on 15/10/10.
//  Copyright © 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLBuffer_h
#define PLBuffer_h

/*!
    @protocol  PLStreamingSendingBufferDelegate
    @abstract  发送队列的代理协议。

    @since     v1.0.0
 */
@protocol PLStreamingSendingBufferDelegate <NSObject>

@optional
/*!
    @method     streamingSessionSendingBufferDidEmpty:
    @abstract   当发送队列从有数据变为无数据时，会触发该队列为空的回调。

    @param      session 调用该代理方法的 session 对象

    @since      v1.0.0
 */
- (void)streamingSessionSendingBufferDidEmpty:(id)session;

/*!
    @method     streamingSessionSendingBufferDidFull:
    @abstract   当发送队列包满时，会触发该队列已满的回调。

    @param      session 调用该代理方法的 session 对象

    @since      v1.0.0
 */
- (void)streamingSessionSendingBufferDidFull:(id)session;

@end

#endif /* PLBuffer_h */
