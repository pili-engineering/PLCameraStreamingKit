//
//  PLStream.h
//  PLCameraStreamingKit
//
//  Created on 15/6/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 * Stream
 * 一个 Stream 对象可以通过从 Pili API 创建获得，以 JSON 形式表达会包含一下信息：
 * @code
 * {
 *      id: 'STREAM_ID',
 *      title: 'STREAM_TITLE'.
 *      hub: 'HUB_NAME',
 *      publishKey: 'PUBLISH_KEY',
 *      publishSecurity: 'PUBLISH_SECURITY',
 *      profiles: ['480p', '720p'],
 *      hosts: {
 *          publish: {
 *              rtmp: 'xxx.xxx.xxx'
 *          },
 *          play: {
 *              rtmp: 'xxx.xxx.xxx',
 *              hls: 'xxxxx.xxxxx'
 *          }
 *      },
 *      disabled: false
 * }
 * @endcode
 */
@interface PLStream : NSObject
<
NSCoding
>

@property (nonatomic, strong) NSString  *streamID;
@property (nonatomic, strong) NSString  *title;
@property (nonatomic, strong) NSString  *hubName;
@property (nonatomic, strong) NSString  *publishKey;
/// 可选 static or dynamic
@property (nonatomic, strong) NSString  *publishSecurity;
@property (nonatomic, assign, getter=isDisabled) BOOL disabled;
@property (nonatomic, strong) NSArray   *profiles;
@property (nonatomic, strong) NSDictionary   *hosts;

+ (instancetype)streamWithJSON:(NSDictionary *)json;
- (instancetype)initWithJSON:(NSDictionary *)json;

@end
