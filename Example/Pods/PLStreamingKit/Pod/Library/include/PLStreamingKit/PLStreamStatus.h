//
//  PLStreamStatus.h
//  PLStreamingKit
//
//  Created by 0dayZh on 16/1/9.
//  Copyright © 2016年 pili-engineering. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PLStreamStatus : NSObject <NSCopying>

/// video frame per second
@property (nonatomic, assign, readonly) double  videoFPS;
/// audio frame per second
@property (nonatomic, assign, readonly) double  audioFPS;
/// video and audio bitrate in bps(Bit Per Second)
@property (nonatomic, assign, readonly) double  totalBitrate;

- (instancetype)initWithVideoFPS:(double)vfps
                        audioFPS:(double)afps
                    totalBitrate:(double)totalBitrate;

@end
