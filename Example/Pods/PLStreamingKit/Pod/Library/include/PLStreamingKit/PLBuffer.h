//
//  PLBuffer.h
//  PLCameraStreamingKit
//
//  Created on 15/10/10.
//  Copyright © 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLBuffer_h
#define PLBuffer_h

@protocol PLStreamingSendingBufferDelegate <NSObject>

@optional
- (void)streamingSessionSendingBufferDidEmpty:(id)session;
- (void)streamingSessionSendingBufferDidFull:(id)session;

/// 已弃用
- (void)streamingSessionSendingBufferFillDidLowerThanLowThreshold:(id)session DEPRECATED_ATTRIBUTE;
- (void)streamingSessionSendingBufferFillDidHigherThanHighThreshold:(id)session DEPRECATED_ATTRIBUTE;
- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items DEPRECATED_ATTRIBUTE;

@end

#endif /* PLBuffer_h */
