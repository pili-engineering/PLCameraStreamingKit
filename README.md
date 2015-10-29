# PLCameraStreamingKit

PLCameraStreamingKit 是一个适用于 iOS 的 RTMP 直播推流 SDK，可高度定制化和二次开发。特色是支持 iOS Camera 画面捕获并进行 H.264 硬编码，以及支持 iOS 麦克风音频采样并进行 AAC 硬编码；同时，还根据移动网络环境的多变性，实现了一套可供开发者灵活选择的编码参数集合。借助 PLCameraStreamingKit，开发者可以快速构建一款类似 [Meerkat](https://meerkatapp.co/) 或 [Periscope](https://www.periscope.tv/) 的手机直播应用。

## 功能特性

- [x] 硬件编码
- [x] 多码率可选
- [x] H.264 视频编码
- [x] AAC 音频编码
- [x] 支持前后摄像头
- [x] 自动对焦支持
- [x] 手动调整对焦点支持
- [x] 闪光灯开关
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

先来看看 PLCameraStreamingKit 接入的步骤

### 配置工程

- 配置你的 Podfile 文件，添加如下配置信息

```shell
pod 'PLCameraStreamingKit'
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
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>
```

`PLCameraStreamingSession` 是核心类，你只需要关注并使用这个类就可以完成通过摄像头推流、预览的工作。如果你只需要做纯音频推流，那么你可以使用 `PLAudioStreamingSession`

推流前务必要先检查摄像头 / 麦克风的授权，并记得设置预览界面，`StreamingSession` 的创建需要 Stream 对象

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
//              @"publish": @{
//                      @"rtmp": @"rtmp_publish_host"
//                      },
//              @"play": @{
//                      @"rtmp": @"rtmp_play_host",
//                      @"hls": @"hls_play_host"
//                      }
//              }
//      }
NSDicationary *streamJSON;
PLStream *stream = [PLStream streamWithJSON:streamJSON];
// 授权后执行
void (^permissionBlock)(void) = ^{
        PLVideoStreamingConfiguration *videoConfiguration = [[PLVideoStreamingConfiguration defaultConfiguration]; 
        PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
        
        self.session = [[PLCameraStreamingSession alloc] initWithVideoConfiguration:videoConfiguration
                                                                 audioConfiguration:audioConfiguration
                                                                             stream:stream
                                                                   videoOrientation:AVCaptureVideoOrientationPortrait];
        self.session.delegate = self;
        self.session.previewView = self.view;
};

void (^noPermissionBlock)(void) = ^{ // 处理未授权情况 };
    
// 检查摄像头是否有授权
PLAuthorizationStatus status = [PLCameraStreamingSession cameraAuthorizationStatus];
   
if (PLAuthorizationStatusNotDetermined == status) {
    [PLCameraStreamingSession requestCameraAccessWithCompletionHandler:^(BOOL granted) {
    // 回调确保在主线程，可以安全对 UI 做操作
        granted ? permissionBlock() : noPermissionBlock();
    }];
} else if (PLAuthorizationStatusAuthorized == status) {
    permissionBlock();
} else {
	noPermissionBlock();
}
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

你无需辛苦的一个个参数设置，```PLCameraStreamingKit``` 提供了一个编码配置的类来帮你快速完成配置。

### 视频编码参数

```Objective-C
// 初始化编码配置类的实例需要的两个参数

// 视频横纵比及分辨率
typedef NS_ENUM(NSUInteger, PLStreamingDimension) {
    PLStreamingDimension_16_9__416x234,
    PLStreamingDimension_16_9__480x270,
    PLStreamingDimension_16_9__640x360,
    PLStreamingDimension_16_9__960x540,
    PLStreamingDimension_16_9__1280x720,
    PLStreamingDimension_16_9__1920x1080,
    PLStreamingDimension_4_3__400x300,
    PLStreamingDimension_4_3__480x360,
    PLStreamingDimension_4_3__640x480,
    PLStreamingDimension_4_3__960x720,
    PLStreamingDimension_4_3__1280x960,
    PLStreamingDimension_4_3__1920x1140,
    PLStreamingDimension_UserDefine,
    PLStreamingDimension_Default = PLStreamingDimension_4_3__640x480
};

// 视频推流质量
/*!
 * @abstract Video streaming quality low 1
 *
 * @discussion 具体参数 fps: 12, profile level: baseline30, video bitrate: 150Kbps
 */
extern NSString *kPLVideoStreamingQualityLow1;

/*!
 * @abstract Video streaming quality low 2
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 264Kbps
 */
extern NSString *kPLVideoStreamingQualityLow2;

/*!
 * @abstract Video streaming quality low 3
 *
 * @discussion 具体参数 fps: 15, profile level: baseline30, video bitrate: 350Kbps
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
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1200Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh1;

/*!
 * @abstract Video streaming quality high 2
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 1500Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh2;

/*!
 * @abstract Video streaming quality high 3
 *
 * @discussion 具体参数 fps: 30, profile level: main30, video bitrate: 2000Kbps
 */
extern NSString *kPLVideoStreamingQualityHigh3;
```

需要明确以上两者，便可以直接获取到最佳的视频编码配置。

```Objective-C
// 默认情况下，PLCameraStreamingKit 会使用 4:3 的 640x480 分辨率，及 kPLVideoStreamingQualityMedium1 作为参数初始化编码配置类的实例.
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration defaultConfiguration];

// 当然你也可以自己指定，比如你希望输出直播视频是 16:9 的 960x540 的分辨率，并且你已经明确你需要的视频质量为 High1，你可以这样来设置编码配置
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration configurationWithDimension:PLStreamingDimension_16_9__960x540 videoQuality:kPLVideoStreamingQualityHigh1];

// 当已有的分辨率无法满足你的需求时，你可以自己定义视频的大小
PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration configurationWithUserDefineDimension:CGSizeMake(width, height) videoQuality:kPLVideoStreamingQualityHigh1];
```

### Video Quality 具体参数

| Quality | FPS | ProfileLevel | Video BitRate(Kbps)|
|---|---|---|---|
|kPLVideoStreamingQualityLow1|12|Baseline 30|150|
|kPLVideoStreamingQualityLow2|15|Baseline 30|264|
|kPLVideoStreamingQualityLow3|15|Baseline 30|350|
|kPLVideoStreamingQualityMedium1|30|Baseline 31|512|
|kPLVideoStreamingQualityMedium2|30|Baseline 31|800|
|kPLVideoStreamingQualityMedium3|30|Baseline 31|1000|
|kPLVideoStreamingQualityHigh1|30|Main 30|1200|
|kPLVideoStreamingQualityHigh2|30|Main 30|1500|
|kPLVideoStreamingQualityHigh3|30|Main 30|2000|

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

在创建好编码配置对象后，就可以用它来初始化 ```PLCameraStreamingSession``` 了。

## 流状态变更及处理处理

实现 `PLCameraStreamingSessionDelegate` 或 `PLAudioStreamingSessionDelegate` 的回调方法，可以及时的得知流状态的变更及推流错误

```Objective-C
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    // 当流状态变更为非 Error 时，会回调到这里
}

```

```Objective-C
- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didDisconnectWithError:(NSError *)error {
    // 当流状态变为 Error, 会携带 NSError 对象回调这个方法
}

```

## 变更推流质量及策略

在推流时，可以配合发送 buffer 自己设定不同的策略，来满足不同的网络环境。

使用 buffer 可用的方法

```Objective-C
// BufferDelegate
@protocol PLStreamingSendingBufferDelegate <NSObject>

- (void)streamingSessionSendingBufferFillDidLowerThanLowThreshold:(id)session;
- (void)streamingSessionSendingBufferFillDidHigherThanHighThreshold:(id)session;
- (void)streamingSessionSendingBufferDidEmpty:(id)session;
- (void)streamingSessionSendingBufferDidFull:(id)session;
- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items;
- (void)streamingSession:(id)session sendingBufferCurrentDurationDidChange:(NSTimeInterval)currentDuration;

@end

// StreamingSession 中的 buffer 相关内容
@interface PLCameraStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// 最低阈值, [0..1], 不可超出这个范围, 也不可大于 highThreshold - 0.1, 默认为 0.2
@property (nonatomic, assign) CGFloat    lowThreshold;

/// 最高阈值, [0..1], 不可超出这个范围, 也不可小于 lowThreshold + 0.1, 默认为 0.8
@property (nonatomic, assign) CGFloat    highThreshold;

/// Buffer 的最大长度, 默认为 3s, 可设置范围为 [1..5]
@property (nonatomic, assign) NSTimeInterval    maxDuration;

@property (nonatomic, assign, readonly) NSTimeInterval    currentDuration;

@end
```

buffer 是一个可以缓存待发送内容的队列，它按照时间(秒)作为缓存长度的判定，可以通过 maxDuration 来读取和设定，buffer 的下阈值和上阈值设定体现你期望的变更推流质量的策略，默认下阈值为 buffer 的 20%(0.2)，上阈值为 0.8。

当 buffer 变为空时，会回调

`- (void)streamingSessionSendingBufferDidEmpty:(id)session;`

当 buffer 满时，会回调

`- (void)streamingSessionSendingBufferDidFull:(id)session;`

如果 buffer 已经满了，但是还有数据传入时，会触发丢帧，会调用

`- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items;`

buffer 的内容高过上阈值时，会回调 

`- (void)streamingSessionSendingBufferFillDidHigherThanHighThreshold:(id)session;`

这是可以尝试降低 quality 的时机

内容低于下阈值时，会回调 

`- (void)streamingSessionSendingBufferFillDidLowerThanLowThreshold:(id)session;`

这是可以尝试增加 quality 的时机。

当了解了可以触发变更 quality 的时机，那么当你需要变更 quality 时，通过下面的方式来做调用 

```Objective-C
[self.session beginUpdateConfiguration];
self.session.videoConfiguration.videoQuality = kPLVideoStreamingQualityMedium2;
self.session.audioConfiguration.audioQuality = kPLAudioStreamingQualityHigh1;
[self.session endUpdateConfiguration];
```

### 重要事项

**在不断流切换 Video Quality 时需要保证 profileLevel 基本不变，即 baseline 只可与 baseline 的 quality 相互切换。以现在的 quality 为例， low 和 medium 的 quality 可以互相切换，但是 high 的 quality 不可以与 low 及 medium 在不断流的情况下无缝切换，否则会导致播放器花屏。**

## 文档支持

PLCameraStreamingKit 使用 HeaderDoc 注释来做文档支持。
开发者无需单独查阅文档，直接通过 Xcode 就可以查看接口和类的相关信息，减少不必要的麻烦。

![Encode 推荐](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/header-doc.png?raw=true)

## 系统要求

- iOS Target : >= iOS 7

## 版本历史

- 1.4.10 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.10.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.10.md))
    - 修复错误重连或者主动重连时导致 UI 卡顿的问题
- 1.4.9 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.9.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.9.md))
    - 尝试修复 iPhone 6s 音频获取不到权限的问题
    - 优化丢包策略，兼顾高延时和低带宽的情况
    - 更新 demo 为全异步请求
- 1.4.8 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.8.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.8.md))
    - 尝试修复 iPhone 6s 音频获取不到权限的问题
    - 修复弱网环境触发的内存和 CPU 占用升高的问题
- 1.4.7 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.7.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.7.md))
    - 尝试修复音频获取不到权限的问题
    - 添加关闭 SDK 渲染 preview 的接口
- 1.4.6 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.6.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.6.md))
    - 修复弱网环境下切换 VideoQuality 可能触发的 crash 问题 
    - 添加 `PLAudioComponentFailedToCreateNotification` 通知，在音频资源被占用时，创建音频结构失败会发送这个通知
- 1.4.5 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.5.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.5.md))
    - 添加自行绘制 CMSampleBufferRef 的支持
- 1.4.4 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.4.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.4.md))
    - 为 PLCameraStreamingSession 多个属性添加 kvo 支持
    - 由时间计数改为包数计数
    - 针对弱网坏境更改丢帧策略，确保不会出现花屏和声音抖动，取而代之会出现间断的无数据，但是可以保证画面和声音的流畅
    - 发送队列的默认上下阈值更改为 0.3 和 0.7
- 1.4.3 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.3.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.3.md))
    - 优化网络层调用，添加 `Disconnecting` 流状态
    - 抽离可导致推流中断的错误
    - 添加错误回调，同时将错误信息返回，便于 debug 及异常处理
    - 修复 `destroy` 调用导致的崩溃问题
    - 支持 iOS 9 的 bitcode
- 1.4.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.2.md))
    - 添加 SendingBuffer 支持
    - 修复 iPhone 6s 下崩溃的问题
- 1.4.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.1.md))
    - 添加可以获取视频源数据的回调方法，便于开发者做滤镜等处理
    - 对自定义分辨率做了内部修正，保证宽高均为 16 倍数
- 1.4.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.0.md))
    - 添加音频推流的 `PLAudioStreamingSession`, 纯音频推流合并到 `PLCameraStreamingKit`
    - 添加后台推流支持，暂且只在纯音频推流中有效
- 1.3.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.3.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.3.2.md))
    - 修复直播被电话 / 闹钟中断后后续直播无声音的问题
    - 更改音频采集类型: RecordAndPlay -> Record
    - 添加对蓝牙音频设备的支持
- 1.3.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.3.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.3.1.md))
    - 修复调用 destroy 方法后，重新创建 session 进入后台触发的 crash 问题
    - 去除无用依赖, SDK 大小由 17 MB 降低为 4.7 MB
- 1.3.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.3.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.3.0.md))
    - 添加带宽质量检测机制，推流过程中可无缝切换音视频 Quality，实现码率及帧率的动态调节
    - 添加发送队列的配置及状态回调
    - 更新 Example 来简单说明如何使用发送队列和 Quality 组合推流策略
- 1.2.8 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.8.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.8.md))
    - 更新编码配置，分离音视频编码配置，便于提供更灵活的配置方案
    - 去除 SIGPIPE 断点
    - 修复 URL 错误时导致的崩溃
    - 修复创建 Session 后设置 Stream 推流失败的问题
- 1.2.7 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.7.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.7.md))
    - 修复断网后停止流时导致的 UI 卡死
    - 修复停止推流时概率出现的 crash 问题
    - 添加 `- (void)destroy` 方法到 `PLCameraStreamingSession`
    - 暂且去除 iOS 8 编码，待进一步优化后再做开放
    - 修复反复进出持有 streamingSession 的 viewController 出现 IO 错误或者 crash 的问题
- 1.2.6 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.6.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.6.md))
    - 修复 iOS 8 以下推流时，编码导致的崩溃问题
- 1.2.5 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.5.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.5.md))
    - 更新了 Quality 配置
- 1.2.4 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.4.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.4.md))
    - 添加全新的 iOS 8 硬编码器，减少编码延时
- 1.2.3 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.3.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.3.md))
    - 更新了 `PLStream` 类结构
    - 添加新的 session 创建接口
    - 移除 publishHost，`PLStream` 对象支持自动生成推流地址，请搭配服务端 SDK 至少 1.2.0 或以上版本使用
- 1.2.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.2.md))
    - 更新麦克风资源调用方式，添加进入后台后自动停用麦克风功能
- 1.2.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.1.md))
	- 修复初次授权摄像头后预览界面为黑屏的问题
- 1.2.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.2.0.md))
	- 添加了 `PLStream` 类，支持 `Coding` 协议便于打包存储
	- 更新 `StreamingSession` 创建方法，借助传递 `PLStream` 对象再无需推流时等待服务端生成推流地址
	- 完善 HeaderDoc 文档
- 1.1.8 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.8.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.8.md))
	- 添加摄像头开启和关闭的操作，便于开发者控制 AVCaptureSession
- 1.1.7 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.7.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.7.md))
	- 添加推流质量字段，方便开发者指定推流质量
	- 移除原配置中的网络选项
	- 默认情况下关闭了 PLLog
	- 添加摄像头开始运行的通知
	- 添加麦克风开始运行的通知
- 1.1.6 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.6.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.6.md))
	- 尝试修复音频变声问题
- 1.1.5 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.5.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.5.md))
	- 修复了重连后无法播放的问题
- 1.1.4 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.4.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.4.md))
	- 更新了码率和采样率。现在统一使用 128Kbps 和 44100KHZ，用以解决噪音的问题
	- 更新了网络 socket。对 socket 在调试情况下出现的 SIGPIPE 进行了处理，不会再出现
	- 更新了网络状态的返回情况。当不是主动调用 close 接口导致的断开都将以 error 状态通知 delegate
- 1.1.3 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.3.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.3.md))
	- 更新了获取视频截图的方法
	- 添加 HeaderDoc 格式注释文档
- 1.1.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.2.md))
	- 添加自定视频大小的编码配置
	- 添加进入后台后自动停止推流的支持
	- 添加保持常亮的开关
	- 添加获取当前录制截屏的功能
	- 修复进入后台导致崩溃的问题
	- 修复切换前置摄像头失效的问题
	- 添加 Release Notes 说明每个版本变动
	- 添加 API Diffs 说明具体修改细节
- 1.1.1
	- 修复 release 版本无法推流的 bug
	- 内存优化，比 1.1.0 减少 6% 左右内存消耗
- 1.1.0
	- 重构接口
	- 优化编码参数
	- 提供不同网络和分辨率下的多种配置可选
	- 添加消息通知，便于监听
	- 兼顾 arc 及非 arc 的工程
- 1.0.0
	- 发布 CocoaPods 版本