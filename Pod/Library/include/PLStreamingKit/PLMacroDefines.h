//
//  PLMacroDefines.h
//  PLCameraStreamingKit
//
//  Created on 15/3/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#ifndef PLCameraStreamingKit_PLMacroDefines_h
#define PLCameraStreamingKit_PLMacroDefines_h

#pragma mark - Debug Log

#define PL_LOG_ENABLE 0

#pragma mark - Memory Management

#ifndef PL_STRONG
    #if __has_feature(objc_arc)
        #define PL_STRONG strong
    #else
        #define PL_STRONG retain
    #endif
#endif

#ifndef PL_WEAK
    #if __has_feature(objc_arc)
        #define PL_WEAK weak
    #else
        #define PL_WEAK assign
    #endif
#endif

#ifndef PL_DEALLOC
    #if __has_feature(objc_arc)
        #define PL_DEALLOC
    #else
        #define PL_DEALLOC dealloc
    #endif
#endif

#define PLAudioStreamBasicDescriptionIsEqual(a, b)    (a && b && a->mSampleRate == b->mSampleRate && a->mBitsPerChannel == b->mBitsPerChannel && a->mBytesPerFrame == b->mBytesPerFrame && a->mBytesPerPacket == b->mBytesPerPacket && a->mChannelsPerFrame == b->mChannelsPerFrame && a->mFormatFlags == b->mFormatFlags && a->mFormatID == b->mFormatID && a->mFramesPerPacket == b->mFramesPerPacket && a->mReserved == b->mReserved)

#define PL_SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

#endif
