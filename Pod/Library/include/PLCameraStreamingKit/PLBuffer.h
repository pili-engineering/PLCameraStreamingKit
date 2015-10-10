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
- (void)streamingSessionSendingBufferFillDidLowerThanLowThreshold:(id)session;
- (void)streamingSessionSendingBufferFillDidHigherThanHighThreshold:(id)session;
- (void)streamingSessionSendingBufferDidEmpty:(id)session;
- (void)streamingSessionSendingBufferDidFull:(id)session;
- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items;
- (void)streamingSession:(id)session sendingBufferCurrentDurationDidChange:(NSTimeInterval)currentDuration;

@end

#endif /* PLBuffer_h */
