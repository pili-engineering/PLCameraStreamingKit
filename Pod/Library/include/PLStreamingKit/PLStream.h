//
//  PLStream.h
//  PLCameraStreamingKit
//
//  Created on 15/6/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
    @class      PLStream
    @abstract   一个 Stream 对象可以通过从 Pili API 创建获得。
 
    @discussion 一个 Stream 对象以 JSON 形式表达会包含一下信息：
                @code
                {
                     id: 'STREAM_ID',
                     title: 'STREAM_TITLE'.
                     hub: 'HUB_NAME',
                     publishKey: 'PUBLISH_KEY',
                     publishSecurity: 'PUBLISH_SECURITY',
                     profiles: ['480p', '720p'],
                     hosts: {
                         ...
                     },
                     disabled: false
                }
                @endcode
 
    @since      v1.0.0
 */
@interface PLStream : NSObject <NSCoding>

/*!
    @property   streamID
    @abstract   用于唯一标识流的 ID。
 
    @since      v1.0.0
 */
@property (nonatomic, copy) NSString  *streamID;

/*!
    @property   title
    @abstract   流标题

    @since      v1.0.0
 */
@property (nonatomic, copy) NSString  *title;

/*!
    @property   hubName
    @abstract   流所归属的 hub 的名字。

    @since      v1.0.0
 */
@property (nonatomic, copy) NSString  *hubName;

/*!
    @property   publishKey
    @abstract   推流时用于鉴权的秘钥。

    @since      v1.0.0
 */
@property (nonatomic, copy) NSString  *publishKey;

/*!
    @property   publishSecurity
    @abstract   鉴权类型。

    @since      v1.0.0
 */
@property (nonatomic, copy) NSString  *publishSecurity;

/*!
    @property   disabled
    @abstract   流是否已被禁用。

    @since      v1.0.0
 */
@property (nonatomic, assign, getter=isDisabled) BOOL disabled;

/*!
    @property   profiles
    @abstract   包含的转码类型。

    @since      v1.0.0
 */
@property (nonatomic, strong) NSArray   *profiles;

/*!
    @property   hosts
    @abstract   所有的相关域名。

    @since      v1.0.0
 */
@property (nonatomic, strong) NSDictionary   *hosts;

/*!
    @method     streamWithJSON
    @abstract   创建并初始化一个 Stream 的类方法。

    @since      v1.0.0
 */
+ (instancetype)streamWithJSON:(NSDictionary *)json;

/*!
    @method     initWithJSON:
    @abstract   初始化一个 Stream 的实例方法。

    @since      v1.0.0
 */
- (instancetype)initWithJSON:(NSDictionary *)json;

@end
