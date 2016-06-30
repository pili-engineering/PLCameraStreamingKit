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
- [x] 与 GPUImage 轻松对接

## 内容摘要

- [快速开始](#快速开始)
	- [配置工程](#配置工程)
	- [示例代码](#示例代码)
- [GPUImage 视频滤镜](#GPUImage)
- [编码参数](#编码参数)
- [流状态变更及错误处理](#流状态变更及处理处理)
- [变更推流质量及策略](#变更推流质量及策略)
- [手动导入到工程](#手动导入到工程)
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
#import <PLStreamingKit/PLStreamingKit.h>
```

`PLStreamingSession` 是核心类，你只需要关注并使用这个类就可以完成推流工作。

`PLStreamingSession ` 的创建

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
PLVideoStreamingConfiguration *videoStreamingConfiguration = [PLVideoStreamingConfiguration configurationWithVideoSize:CGSizeMake(320, 576) videoQuality:kPLVideoStreamingQualityLow2];
PLAudioStreamingConfiguration *audioStreamingConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
PLStream *stream = [PLStream streamWithJSON:streamJSON];

self.session = [[PLStreamingSession alloc] initWithVideoStreamingConfiguration:videoStreamingConfiguration audioStreamingConfiguration:audioStreamingConfiguration stream:stream];
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

## <a name="GPUImage"></a>GPUImage 视频滤镜

GPUImage 作为当前 iOS 平台使用率最高的图像渲染引擎，可以轻松与 PLStreamingKit 对接，利用 GPUImage 已有的 125 个内置滤镜满足大部分的直播滤镜需求。

### 接入 GPUImage

接入工程的方式详见官方 README.md https://github.com/BradLarson/GPUImage

### 滤镜实例

```Objective-C
// 使用 GPUImageVideoCamera 获取摄像头数据
GPUImageVideoCamera *videoCamera = [[GPUImageVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480 cameraPosition:AVCaptureDevicePositionBack];
videoCamera.outputImageOrientation = UIInterfaceOrientationPortrait;

// 创建一个 filter
GPUImageSketchFilter *filter = [[GPUImageSketchFilter alloc] init];

CGRect bounds = [UIScreen mainScreen].bounds;
CGFloat width = CGRectGetWidth(bounds);
CGFloat height = width * 640.0 / 480.0;
GPUImageView *filteredVideoView = [[GPUImageView alloc] initWithFrame:(CGRect){0, 64, width, height}];

// Add the view somewhere so it's visible
[self.view addSubview:filteredVideoView];

[videoCamera addTarget:filter];
[filter addTarget:filteredVideoView];

// 创建一个 GPUImageRawDataOutput 作为 filter 的 Target
GPUImageRawDataOutput *rawDataOutput = [[GPUImageRawDataOutput alloc] initWithImageSize:CGSizeMake(480, 640) resultsInBGRAFormat:YES];
[filter addTarget:rawDataOutput];
__weak GPUImageRawDataOutput *weakOutput = rawDataOutput;
__weak typeof(self) wself = self;
[rawDataOutput setNewFrameAvailableBlock:^{
    __strong GPUImageRawDataOutput *strongOutput = weakOutput;
    __strong typeof(wself) strongSelf = wself;
    [strongOutput lockFramebufferForReading];

    //从 GPUImageRawDataOutput 中获取 CVPixelBufferRef
    GLubyte *outputBytes = [strongOutput rawBytesForImage];
    NSInteger bytesPerRow = [strongOutput bytesPerRowInOutput];
    CVPixelBufferRef pixelBuffer = NULL;
    CVPixelBufferCreateWithBytes(kCFAllocatorDefault, 480, 640, kCVPixelFormatType_32BGRA, outputBytes, bytesPerRow, nil, nil, nil, &pixelBuffer);
    [strongOutput unlockFramebufferAfterReading];
    if(pixelBuffer == NULL) {
        return ;
    }

    // 发送视频数据
    [strongSelf.session pushPixelBuffer:pixelBuffer completion:^{
        CVPixelBufferRelease(pixelBuffer);
    }];
}];

[videoCamera startCameraCapture];
```

完整的可运行代码在 Example 中。

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
PLVideoStreamingConfiguration *videoStreamingConfiguration = [PLVideoStreamingConfiguration defaultConfiguration];

// 你也可以指定自己想要的分辨率和已有的 video quality 参数
PLVideoStreamingConfiguration *videoStreamingConfiguration = [PLVideoStreamingConfiguration configurationWithVideoSize:CGSizeMake(320, 480) videoQuality:kPLVideoStreamingQualityHigh1];

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
[self.session reloadVideoStreamingConfiguration:newConfiguraiton];
```

## 手动导入到工程

我们建议使用 CocoaPods 导入，如果由于特殊原因需要手动导入，可以按照如下步骤进行：

 - 将 Pod 目录下的文件加入到工程中；
 - 将 https://github.com/qiniu/happy-dns-objc HappyDNS 目录下的所有文件加入到工程中；
 - 将 https://github.com/pili-engineering/pili-librtmp Pod 目录下的所有文件加入到工程中；
 - 在工程对应 TARGET 中，右侧 Tab 选择 "Build Phases"，在 "Link Binary With Libraries" 中加入 UIKit、AVFoundation、CoreGraphics、CFNetwork、CoreMedia、AudioToolbox 这些 framework，并加入 libc++.tdb、libz.tdb 及 libresolv.tbd；
 - 在工程对应 TARGET 中，右侧 Tab 选择 "Build Settings"，在 "Other Linker Flags" 中加入 "-ObjC" 选项；

## 文档支持

PLStreamingKit 使用 HeaderDoc 注释来做文档支持。
开发者无需单独查阅文档，直接通过 Xcode 就可以查看接口和类的相关信息，减少不必要的麻烦。

![](./header-doc.png?raw=true)

## 系统要求

- iOS Target : >= iOS 7

## 版本历史

- 1.2.4 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.4.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.2.4.md))
- 功能
  - 新增对每次传入非 1024 frame PCM数据进行编码的功能
- 缺陷
  - 修复音视频时间戳偶尔出现的非单调递增的缺陷
- 1.2.3 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.3.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.2.3.md))
  - 功能
    - 更新底层依赖的 pili-librtmp 到 v1.0.3
- 1.2.2 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.2.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.2.2.md))
  - 功能
    - 支持初始化的时候传入 stream 为 nil
    - 支持调节音频编码采样率
    - 支持快速重连操作，方便 4G 推流时切换 WIFI 场景快速切换网络
    - 完善了音频出错时的 log
- 1.2.1 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.1.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.2.1.md))
  - 功能
    - 新增 iOS9 下的纯 IPV6 环境支持
  - 缺陷
    - 修复 dynamic 鉴权方式下重连失效的问题
- 1.2.0 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.2.0.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.2.0.md))
    - 解决 iPhone 6s 上出现的电流音问题
    - 支持后台推流
    - 支持 64kbps 音频码率
    - 部分接口重命名
- 1.1.6 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.6.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.6.md))
	- 拆分 pili-librtmp 为公共依赖，避免模拟器环境下与 PLPlayerKit冲突的问题
	- 解决网络不可达条件下 `- (void)startWithCompleted:(void (^)(BOOL success))handler;` 方法无回调的问题
	- 新增质量上报支持
	- 增加推流中实时变换采集音频参数的接口
- 1.1.5 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.5.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.5.md))
    - 修复 `v1.1.1` 版本引入的断网时引起的 UI 卡死问题，强烈建议 >= `v1.1.1` 的均做更新
- 1.1.4 ([Release Notes](https://github.com/pili-engineering/PLStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.4.md) && [API Diffs](https://github.com/pili-engineering/PLStreamingKit/blob/master/APIDiffs/api-diffs-1.1.4.md))
    - 新增网络连接接收超时接口和发送超时接口
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
