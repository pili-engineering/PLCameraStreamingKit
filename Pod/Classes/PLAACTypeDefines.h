//
//  PLAACTypeDefines.h
//  PLCameraStreamingKit
//
//  Created by 0day on 15/4/9.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#ifndef PLCameraStreamingKit_PLAACTypeDefines_h
#define PLCameraStreamingKit_PLAACTypeDefines_h

typedef NS_ENUM(NSInteger, PLAACSampleRate) {
    PLAACSampleRate8000HZ  = 8000,
    PLAACSampleRate11025HZ = 11025,
    PLAACSampleRate22050HZ = 22050,
    PLAACSampleRate44100HZ = 44100,
    PLAACSampleRate48000HZ = 48000
};

typedef NS_ENUM(uint8_t, PLAACPacketType) {
    PLAACPacketTypeSequenceHeader = 0,
    PLAACPacketTypeRaw,
};

#endif
