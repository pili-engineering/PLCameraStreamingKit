# PLCameraStreamingKit

PLCameraStreamingKit 是一个适用于 iOS 的 RTMP 直播推流 SDK，可高度定制化和二次开发。特色是支持 iOS Camera 画面捕获并进行 H.264 硬编码，以及支持 iOS 麦克风音频采样并进行 AAC 硬编码；同时，还根据移动网络环境的多变性，实现了一套可供开发者灵活选择的编码参数集合。借助 PLCameraStreamingKit，开发者可以快速构建一款类似 [Meerkat](https://meerkatapp.co/) 或 [Periscope](https://www.periscope.tv/) 的手机直播应用。

## 功能特性

- [x] 采集模块源码开放
- [x] 支持硬件编码
- [x] 多码率可选
- [x] 支持 H.264 视频编码
- [x] 支持 AAC 音频编码
- [x] 支持前后摄像头
- [x] 支持自动对焦
- [x] 支持手动调整对焦点
- [x] 支持闪光灯操作
- [x] 支持多分辨率编码
- [x] 支持 HeaderDoc 文档
- [x] 支持构造带安全授权凭证的 RTMP 推流地址
- [x] 支持 ARMv7, ARM64, i386, x86_64 架构
- [x] 支持 RTMP 协议直播推流
- [x] 支持音视频配置分离
- [x] 支持推流时可变码率
- [x] 提供发送 buffer
- [x] 支持 Zoom 操作
- [x] 支持音频 Mute 操作
- [x] 支持视频 Orientation 操作
- [x] 支持自定义 DNS 解析
- [x] 支持弱网丢帧策略
- [x] 支持纯音频或纯视频推流
- [x] 支持后台音频推流
- [x] 支持水印功能
- [x] 支持美颜功能

## 内容摘要

- [快速开始](#快速开始)
	- [配置工程](#配置工程)
	- [示例代码](#示例代码)
- [编码参数](#编码参数)
- [流状态变更及错误处理](#流状态变更及处理处理)
- [变更推流质量及策略](#变更推流质量及策略)
- [手动导入到工程](#手动导入到工程)
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

在 `AppDelegate.m` 中进行 SDK 初始化，如果未进行 SDK 初始化，在核心类 `PLStreamingSession` 初始化阶段将抛出异常

```Objective-C
#import <PLStreamingKit/PLStreamingEnv.h>

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [PLStreamingEnv initEnv];
    // Override point for customization after application launch.
    return YES;
}
```

在需要的地方添加

```Objective-C
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>
```

`PLCameraStreamingSession` 是核心类，你只需要关注并使用这个类就可以完成通过摄像头推流、预览的工作。如果你只需要做纯音频推流，在创建 `PLCameraStreamingSession `时音频相关的 Configuration 传入 nil 即可。

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
//            ...
//      }
NSDictionary *streamJSON;
PLStream *stream = [PLStream streamWithJSON:streamJSON];
// 授权后执行
void (^permissionBlock)(void) = ^{
			PLVideoCaptureConfiguration *videoCaptureConfiguration = [self.videoCaptureConfigurations defaultConfiguration];
			PLAudioCaptureConfiguration *audioCaptureConfiguration = [PLAudioCaptureConfiguration defaultConfiguration];
			PLVideoStreamingConfiguration *videoStreamingConfiguration = [self.videoStreamingConfigurations defaultConfiguration];
			PLAudioStreamingConfiguration *audioStreamingConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];

      self.session = [[PLCameraStreamingSession alloc] initWithVideoCaptureConfiguration:videoCaptureConfiguration audioCaptureConfiguration:audioCaptureConfiguration videoStreamingConfiguration:videoStreamingConfiguration audioStreamingConfiguration:audioStreamingConfiguration stream:stream videoOrientation:orientation];

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

## 采集参数



## 编码参数

移动端因网络环境不稳定及用户电量宝贵等原因，并不建议直接使用最高码率和分辨率来做推流，以最佳编码参数来做设置可以带来更好的推流效果和用户体验。

如果你不能确定该如何配置各个编码参数，也不用担心，```PLCameraStreamingKit``` 提供了一个编码配置的类来帮你快速完成配置，你可以通过使用 SDK 预先定义好的 quality 来构建编码推流配置。

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

需要明确以上两者，便可以直接获取到最佳的视频编码配置。

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
    @constant   kPLAudioStreamingQualityHigh1
    @abstract   音频编码推流质量 high 1。

    @discussion 具体参数 audio bitrate: 64Kbps。

    @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh1;

/*!
    @constant   kPLAudioStreamingQualityHigh2
    @abstract   音频编码推流质量 high 2。

    @discussion 具体参数 audio bitrate: 96Kbps。

    @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh2;

/*!
 @constant   kPLAudioStreamingQualityHigh3
 @abstract   音频编码推流质量 high 3。

 @discussion 具体参数 audio bitrate: 128Kbps。

 @since      v1.0.0
 */
extern NSString *kPLAudioStreamingQualityHigh3;

```

生成音频编码配置

```
// 音频配置默认使用 high2 作为质量选项
PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];

// 如果你需要自己定义音频质量
PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration configurationWithAudioQuality:kPLAudioStreamingQualityHigh1];
```

### Audio Quality 具体参数

| Quality | Audio BitRate(Kbps) |
|---|---|
|kPLAudioStreamingQualityHigh1|64|
|kPLAudioStreamingQualityHigh2|96|
|kPLAudioStreamingQualityHigh3|128|

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
@interface PLCameraStreamingSession (SendingBuffer)

@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;

/// [0..1], 不可超出这个范围, 默认为 0.5
@property (nonatomic, assign) CGFloat    threshold;

/// Buffer 最多可包含的包数，默认为 300
@property (nonatomic, assign) NSUInteger    maxCount;
@property (nonatomic, assign, readonly) NSUInteger    currentCount;

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

## 手动导入到工程

我们建议使用 CocoaPods 导入，如果由于特殊原因需要手动导入，可以按照如下步骤进行：

 - 将 Pod 目录下的文件加入到工程中；
 - 依照 [https://github.com/pili-engineering/PLStreamingKit](https://github.com/pili-engineering/PLStreamingKit) 中的手动导入到工程指南将 PLStreamingKit 及其依赖库导入到工程中；

## 文档支持

PLCameraStreamingKit 使用 HeaderDoc 注释来做文档支持。
开发者无需单独查阅文档，直接通过 Xcode 就可以查看接口和类的相关信息，减少不必要的麻烦。

![Encode 推荐](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/header-doc.png?raw=true)

## 系统要求

- iOS Target : >= iOS 7

## 版本历史

- 1.8.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.8.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.8.0.md))
- 功能
  - 更新依赖的 PLStreamingKit 到 1.2.4
  - 新增内置美颜功能，美颜效果可调节
  - 新增单独调节前后置摄像头预览及推流是否镜像的接口
  - 新增动态帧率功能，通过降低采集帧率来适配弱网环境
- 缺陷
  - 修复音视频时间戳偶尔出现的非单调递增的缺陷
  - 修复特殊机型可能出现的没有声音的问题
- 优化
  - 优化水印性能，降低 CPU 和内存消耗
- 1.7.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.7.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.7.2.md))
  - 功能
    - 更新依赖的 PLStreamingKit 到 1.2.2
    - 新增回调队列配置功能
    - 新增默认摄像头位置配置项
    - 新增录制音量调节选项（由于系统原因，仅对除 iPhone 6s 系列以外的机型生效）
    - 支持初始化的时候传入 stream 为 nil
    - 支持快速重连操作，方便 4G 推流时切换 WIFI 场景快速切换网络
  - 缺陷
    - 修复特殊场景可能出现的电流音问题
    - 修复特殊场景可能出现的没有声音的问题
    - 修复后台推流时被音频打断结束之后无法正常恢复推流的问题
- 1.7.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.7.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.7.1.md))
  - 缺陷
    - 修复切换前置摄像头之后无法缩放的问题
    - 修复不添加水印无法正常推流的问题
- 1.7.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.7.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.7.0.md))
  - 功能
    - 新增后台推流功能（仅音频，因 iOS 隐私限制不允许后台访问摄像头）
    - 新增水印功能
    - 新增可选定制美颜功能（需联系工作人员）

  - 缺陷
    - 修复特定机型上的电流音等杂音问题

  - 版本
    - 更新依赖 PLStreamingKit 的版本到 `v1.2.0`

  - 其他
    - 部分接口重命名
- 1.6.3 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.6.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.6.2.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.1.6`
    - 更新 Demo 适应 PLStreamingKit `v1.1.6` 接口变更
- 1.6.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.6.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.6.2.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.1.5`
    - 支持 Zoom 操作
- 1.6.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.6.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.6.1.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.1.3`
    - 修复调用 `- (void)reloadVideoConfiguraiton` 方法，fps 变更失败的问题
- 1.6.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.6.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.6.0.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.1.2`
    - 添加 `PLStreamStatus` 回调，便于客户端监控推流状态
    - 修复采集源 fps 变更失败的问题
- 1.5.2 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.5.2.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.5.2.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.0.3`
- 1.5.1 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.5.1.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.5.1.md))
    - 更新依赖 PLStreamingKit 的版本到 `v1.0.2`
    - 更改音频数据源采集部分，转换为单声道
- 1.5.0 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.5.0.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.5.0.md))
    - 编码推流改为依赖 `PLStreamingKit`
    - 去除 `PLAudioStreamingSession` 类, 建议单纯推音频的 App 直接接入 `PLStreamingKit`
    - 摄像头及音频采集部分开源
    - 底层添加了 HappyDNS, 解决部分地区 DNS 感染严重导致推流失败问题
    - 优化 TCP 层发包，极大程度解决了网络正常但推流中经常断开的问题
- 1.4.16 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.16.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.16.md))
    - 解决 destroy 方法可能卡死线程的问题
- 1.4.15 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.15.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.15.md))
    - 修复类库 .a 未更新问题
- 1.4.14 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.14.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.14.md))
    - 解决视频质量切换时导致 crash 的问题
    - 解决推流时触发的只有音频无视频问题
    - 解决音频采样导致的电流声
    - 解决潜在内存问题，整体提升稳定性
    - 视频编码统一 profileLevel 为 baseline31
    - 限定最高支持分辨率到 720p
- 1.4.13 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.13.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.13.md))
    - 更新 mute 行为，mute 后不会中断发包
    - 更新音频设备采样率获取途径
    - 更新底层库，避免冲突
- 1.4.12 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.12.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.12.md))
    - 添加 `smoothAutoFocusEnabled` 参数，确保拍摄时对焦的平滑过渡
    - 修复无网络环境或其他情况触发断流时引起的 crash
    - 修复频繁切换摄像头时引起的 crash
    - 修复内存 leak
- 1.4.11 ([Release Notes](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.4.11.md) && [API Diffs](https://github.com/pili-engineering/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.4.11.md))
    - 修复 iPhone 6s 系列机型直播无声音问题
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
