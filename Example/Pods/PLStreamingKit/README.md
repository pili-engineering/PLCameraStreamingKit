# PLStreamingKit

PLStreamingKit 是一个适用于 iOS 的 RTMP 直播推流 SDK，可高度定制化和二次开发。特色是支持 H.264 硬编码，以及支持 AAC 硬编码；同时，还根据移动网络环境的多变性，实现了一套可供开发者灵活选择的编码参数集合。

PLStreamingKit 不包括摄像头、麦克风等设备相关的资源获取，只包括音视频数据的编码处理和发送。

## 功能特性

- [x] 硬件编码
- [x] 多码率可选
- [x] H.264 视频编码
- [x] AAC 音频编码
- [x] 多分辨率编码支持
- [x] HeaderDoc 文档支持
- [x] 内置生成安全的 RTMP 推流地址
- [x] ARM64 支持
- [x] 支持 RTMP 协议直播推流
- [x] 音视频配置分离
- [x] 推流时可变码率
- [x] 提供发送 buffer

## 内容摘要

- [快速开始](#快速开始)
	- [配置工程](#配置工程)
	- [示例代码](#示例代码)
- [编码参数](#编码参数)
- [流状态变更及错误处理](#流状态变更及处理处理)
- [变更推流质量及策略](#变更推流质量及策略)
    - [重要事项](#重要事项)
- [文档支持](#文档支持)
- [功能特性](#功能特性)
- [系统要求](#系统要求)
- [版本历史](#版本历史)

## 快速开始

先来看看 PLStreamingKit 接入的步骤

### 配置工程

- 配置你的 Podfile 文件，添加如下配置信息

```shell
pod 'PLStreamingKit'
```

- 安装 CocoaPods 依赖

```shell
pod install
```

或

```shell
pod update
```

- Done! 运行你工程的 workspace

### 示例代码

在需要的地方添加

```Objective-C
#import <PLStreamingKit/PLStreamingKit.h>
```

`PLStreamingSession` 是核心类，你只需要关注并使用这个类就可以完成推流工作。

`StreamingSession` 的创建

```Objective-C
// streamJSON 是从服务端拿回的
//
// 从服务端拿回的 streamJSON 结构如下：
//    @{@"id": @"stream_id",
//      @"title": @"stream_title",
//      @"hub": @"hub_name",
//      @"publishKey": @"publish_key",
//      @"publishSecurity": @"dynamic", // or static
//      @"disabled": @(NO),
//      @"profiles": @[@"480p", @"720p"],    // or empty Array []
//      @"hosts": @{
//            ...
//      }
NSDictionary *streamJSON;
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration configurationWithVideoSize:CGSizeMake(320, 576) videoQuality:kPLVideoStreamingQualityLow2];
PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
PLStream *stream = [PLStream streamWithJSON:streamJSON];
    
self.session = [[PLStreamingSession alloc] initWithVideoConfiguration:videoConfiguration audioConfiguration:audioConfiguration stream:stream];
self.session.delegate = self;
```

推流操作

```Objective-C
// 开始推流，无论 security policy 是 static 还是 dynamic，都无需再单独计算推流地址
[self.session startWithCompleted:^(BOOL success) {
	// 这里的代码在主线程运行，所以可以放心对 UI 控件做操作
	if (success) {
		// 连接成功后的处理
		// 成功后，在这里才可以读取 self.session.pushURL，start 失败和之前不能确保读取到正确的 URL
	} else {
    	// 连接失败后的处理
	}
}];

// 停止推流
[self.session stop];
```

销毁推流 session
```Objective-C
[self.session destroy];
```

## 编码参数

移动端因网络环境不稳定及用户电量宝贵等原因，并不建议直接使用最高码率和分辨率来做推流，以最佳编码参数来做设置可以带来更好的推流效果和用户体验。

如果你不能确定该如何配置各个编码参数，也不用担心，```PLStreamingKit``` 提供了一个编码配置的类来帮你快速完成配置，你可以通过使用 SDK 预先定义好的 quality 来构建编码推流配置。

### 视频编码参数

```Objective-C
// 视频推流质量
/*!
 * @abstract Video streaming quality low 1
 *
 * @discussion 具体参数 fps: 12, profile level: baseline31, video bitrate: 150Kbps
 */
extern NSString *kPLVideoStreamingQualityLow1;

/*!
 * @abstract Video streaming quality low 2
 *
 * @discussion 具体参数 fps: 15, profile level: baseline31, video bitrate: 264Kbps
 */
extern NSString *kPLVideoStreamingQualityLow2;

/*!
 * @abstract Video streaming quality low 3
 *
 * @discussion 具体参数 fps: 15, profile level: baseline31, video bitrate: 350Kbps
 */
extern NSString *kPLVideoStreamingQualityLow3;

/*!
 * @abstract Video streaming quality medium 1
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 512Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium1;

/*!
 * @abstract Video streaming quality medium 2
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 800Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium2;

/*!
 * @abstract Video streaming quality medium 3
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 1000Kbps
 */
extern NSString *kPLVideoStreamingQualityMedium3;

/*!
 * @abstract Video streaming quality high 1
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 1200Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh1;

/*!
 * @abstract Video streaming quality high 2
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 1500Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh2;

/*!
 * @abstract Video streaming quality high 3
 *
 * @discussion 具体参数 fps: 30, profile level: baseline31, video bitrate: 2000Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh3;
```

需要明确以上两者，便可以获取到最佳的视频编码配置。

```Objective-C
// 该方法每次都会生成一个新的配置，不是单例方法。默认情况下，对应的参数为分辨率 (320, 480), video quality PLStreamingQualityMedium1
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration defaultConfiguration];

// 你也可以指定自己想要的分辨率和已有的 video quality 参数
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration configurationWithVideoSize:CGSizeMake(320, 480) videoQuality:kPLVideoStreamingQualityHigh1];

// 当已有的分辨率无法满足你的需求时，你可以自己定义所有参数，但请务必确保你清楚参数的含义
PLVideoStreamingConfiguration *videoConfiguration = [[PLVideoStreamingConfiguration alloc] initWithVideoSize:CGSizeMake(width, height) videoFrameRate:30 videoMaxKeyframeInterval:90 videoBitrate:1200 * 1000 videoProfileLevel:AVVideoProfileLevelH264Main32]];
```

### Video Quality 具体参数

| Quality | FPS | ProfileLevel | Video BitRate(Kbps)|
|---|---|---|---|
|kPLVideoStreamingQualityLow1|12|Baseline 31|150|
|kPLVideoStreamingQualityLow2|15|Baseline 31|264|
|kPLVideoStreamingQualityLow3|15|Baseline 31|350|
|kPLVideoStreamingQualityMedium1|30|Baseline 31|512|
|kPLVideoStreamingQualityMedium2|30|Baseline 31|800|
|kPLVideoStreamingQualityMedium3|30|Baseline 31|1000|
|kPLVideoStreamingQualityHigh1|30|Baseline 31|1200|
|kPLVideoStreamingQualityHigh2|30|Baseline 31|1500|
|kPLVideoStreamingQualityHigh3|30|Baseline 31|2000|

### 音频编码参数

```
// 音频推流质量
/*!
 * @abstract Audio streaming quality high 1
 *
 * @discussion 具体参数 audio sample rate: 44MHz, audio bitrate: 96Kbps
 */
extern NSString *kPLAudioStreamingQualityHigh1;

/*!
 * @abstract Audio streaming quality high 2
 *
 * @discussion 具体参数 audio sample rate: 44MHz, audio bitrate: 128Kbps
 */
extern NSString *kPLAudioStreamingQualityHigh2;
```

生成音频编码配置

```
// 音频配置默认使用 high2 作为质量选项
PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];

// 如果你需要自己定义音频质量
PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration configurationWithAudioQuality:kPLAudioStreamingQualityHigh1];
```

### Audio Quality 具体参数

| Quality | Audio Samplerate(MHz)) | Audio BitRate(Kbps) |
|---|---|---|
|kPLAudioStreamingQualityHigh1|44|96|
|kPLAudioStreamingQualityHigh2|44|128|

在创建好编码配置对象后，就可以用它来初始化 ```PLStreamingSession``` 了。

## 流状态变更及处理处理

实现 `PLStreamingSessionDelegate` 的回调方法，可以及时的得知流状态的变更及推流错误

```Objective-C
- (void)streamingSession:(PLStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    // 当流状态变更为非 Error 时，会回调到这里
}
```

```Objective-C
- (void)streamingSession:(PLStreamingSession *)session didDisconnectWithError:(NSError *)error {
    // 当流状态变为 Error, 会携带 NSError 对象回调这个方法
}
```

```Objective-C
- (void)streamingSession:(PLStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status {
    // 当开始推流时，会每间隔 3s 调用该回调方法来反馈该 3s 内的流状态，包括视频帧率、音频帧率、音视频总码率
}
```

## 变更推流质量及策略

在推流时，可以配合发送 buffer 自己设定不同的策略，来满足不同的网络环境。

使用 buffer 可用的方法

```Objective-C
// BufferDelegate
@protocol PLStreamingSendingBufferDelegate <NSObject>

- (void)streamingSessionSendingBufferDidEmpty:(id)session;
- (void)streamingSessionSendingBufferDidFull:(id)session;

@end

// StreamingSession 中的 buffer 相关内容
@interface PLStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// [0..1], 不可超出这个范围, 默认为 0.5
@property (nonatomic, assign) CGFloat    threshold;

/// Buffer 的最大长度, 默认为 300
@property (nonatomic, assign) NSTimeInterval    maxCount;

@property (nonatomic, assign, readonly) NSTimeInterval    currentCount;

@end
```

buffer 是一个可以缓存待发送内容的队列，它按照帧数作为缓存长度的判定，可以通过 maxCount 来读取和设定，buffer 的阈值设定体现你期望的变更推流质量的策略，默认阈值为 buffer 的 50%(0.5)。

当 buffer 变为空时，会回调

`- (void)streamingSessionSendingBufferDidEmpty:(id)session;`

当 buffer 满时，会回调

`- (void)streamingSessionSendingBufferDidFull:(id)session;`

当你希望在 streamStatus 变化，buffer empty 或者 buffer full 时变化 video configuration，可以调用 session 的 reloadVideoConfiguration: 方法

```Objective-C
[self.session reloadVideoConfiguration:newConfiguraiton];
```

### 重要事项

**在调用 `reloadVideoConfiguration:newConfiguraiton` 时，请务必确保 profileLevel 和 videoSize 前后一致，如果该参数有变更，需要先调用 stop, 重新开始推流, 否则可能会因播放器对解码器构建的差异而产生花屏、绿屏等问题。**

## 文档支持

PLStreamingKit 使用 HeaderDoc 注释来做文档支持。
开发者无需单独查阅文档，直接通过 Xcode 就可以查看接口和类的相关信息，减少不必要的麻烦。

![](./header-doc.png?raw=true)

## 系统要求

- iOS Target : >= iOS 7

## 版本历史

- 1.1.3 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.3.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.3.md))
    - 优化网络发包，合并多个小包一起发送，提升带宽利用率
- 1.1.2 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.2.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.2.md))
    - 修复 dynamic 推流 nounce 取值过小，导致安卓端推流后，同一个流在 iOS 端推流会失败的问题
- 1.1.1 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.1.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.1.md))
    - 修复因 video configuration rtmp 发送时没读渠道发送 onMetaData 只有音频信息的问题
    - 添加版本信息读取方法
    - 添加实施推流状态的返回，便于开发者从推流端获取推流信息
- 1.1.0 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.0.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.0.md))
    - 重构 `PLVideoStreamingConfiguration`, 提供给开发者更大的视频编码定制自由度
    - `PLVideoStreamingConfiguration` 提供了 `validate` 方法, 确保 fast fail 减少开发者 app 携带不正确编码参数上线的可能性
    - 优化推送音视频数据, 添加了编码处理完后的回调
- 1.0.3 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.0.3.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.0.3.md))
    - 优化 dns 解析部分，补全 happydns 解析失败后的本地解析
- 1.0.2 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.0.2.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.0.2.md))
    - 修复 dns 解析失败时无 error 回调的问题
    - 优化音频数据默认为单声道，与 iOS 设备单声道采集贴近
    - 针对没有音频 configuration 的推流，优化发送的 onMetaData 信息，只携带视频信息，极大缩短 ffplay, ijkplayer 的等待时间
- 1.0.1 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.0.1.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.0.1.md))
    - 添加 HappyDNS, 优化 DNS 解析
    - 优化 TCP 发送层, 减少发包失败触发的错误
    - 修复推流时内存递增的问题
    - 修复切换 Quality 时，播放卡住的问题
- 1.0.0 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.0.0.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.0.0.md))
    - PLStreamingKit CocoaPods 版本发布
    - H.264 硬件编码
    - AAC 硬件编码
    - RTMP 推流支持
    - 弱网络环境音频优先的丢帧策略
    - 接口简明，便于和 Pili 直接对接使用，减少理解和开发成本