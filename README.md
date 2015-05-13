# PLCameraStreamingKit

PLCameraStreamingKit 是为 **pili 流媒体云服务** 流媒体云服务提供的一套推送直播流 SDK, 旨在解决 iOS 端快速、轻松实现 iOS 设备利用摄像头直播接入，便于 **pili 流媒体云服务** 的开发者专注于产品业务本身，而不必在技术细节上花费不必要的时间。

## 内容摘要

- [快速开始](#快速开始)
	- [配置工程](#配置工程)
	- [示例代码](#示例代码)
- [编码参数](#编码参数)
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

```PLCameraStreamingSession``` 是核心类，你只需要关注并使用这个类就可以完成通过摄像头推流、预览的工作

推流前务必要先检查摄像头 / 麦克风的授权，并记得设置预览界面

```Objective-C
// 授权后执行
void (^permissionBlock)(void) = ^{
        PLCameraStreamingConfiguration *configuration = [PLCameraStreamingConfiguration defaultConfiguration];
        self.session = [[PLCameraStreamingSession alloc] initWithConfiguration:configuration];
        self.session.delegate = self;
        self.session.previewView = self.view;
};

void (^noPermissionBlock)(void) = ^{ // 处理未授权情况 };
    
// 检查摄像头是否有授权
PLAuthorizationStatus status = [PLCameraStreamingSession cameraAuthorizationStatus];
   
if (PLAuthorizationStatusNotDetermined == status) {
    [PLCameraStreamingSession requestCameraAccessWithCompletionHandler:^(BOOL granted) {
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
// 开始推流，这里的推流地址应该是你自己的服务端通过 pili 流媒体云服务请求到的
[self.session startWithPushURL:[NSURL URLWithString:@"YOUR_RTMP_PUSH_URL_HERE"] completed:^(BOOL success) {
	// 这里的代码在主线程运行，所以可以放心对 UI 控件做操作
	if (success) {
		// 连接成功后的处理
	} else {
    	// 连接失败后的处理
	}
}];

// 停止推流
[self.session stop];
```

## 编码参数

移动端因网络环境不稳定及用户电量宝贵等原因，并不建议直接使用最高码率和分辨率来做推流，以最佳编码参数来做设置可以带来更好的推流效果和用户体验。

苹果官方推荐的编码参数如下图：

![Encode 推荐](https://github.com/pili-io/PLCameraStreamingKit/blob/master/streaming-encode-recommendations.jpg?raw=true)

你无需辛苦的一个个参数设置，```PLCameraStreamingKit``` 提供了一个编码配置的类来帮你快速完成配置。

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

// 网络类型
typedef NS_ENUM(NSUInteger, PLStreamingNetworkType) {
    PLStreamingNetworkTypeCELL,
    PLStreamingNetworkTypeWiFi,
    PLStreamingNetworkTypeEither
};
```

你只需要明确以上两者，便可以直接获取到最佳编码配置。

```Objective-C
// 默认情况下，PLCameraStreamingKit 会使用 4:3 的 640x480 分辨率，及 PLStreamingNetworkTypeEither 作为参数初始化编码配置类的实例.
PLCameraStreamingConfiguration *configuration = [PLCameraStreamingConfiguration defaultConfiguration];


// 当然你也可以自己指定，比如你希望输出直播视频是 16:9 的 960x540 的分辨率，并且你已经明确用户当前是在 Wi-Fi 环境下，你可以这样来设置编码配置
PLCameraStreamingConfiguration *configuration = [PLCameraStreamingConfiguration configurationWithDimension:PLStreamingDimension_16_9__960x540 network:PLStreamingNetworkTypeWiFi];

// 当已有的分辨率无法满足你的需求时，你可以自己定义视频的大小
PLCameraStreamingConfiguration *configuration = [PLCameraStreamingConfiguration configurationWithUserDefineDimension:CGSizeMake(width, height) network:PLStreamingNetworkTypeWiFi];
```

在创建好编码配置对象后，就可以用它来初始化 ```PLCameraStreamingSession``` 了。

## 文档支持

PLCameraStreamingKit 使用 HeaderDoc 注释来做文档支持。
开发者无需单独查阅文档，直接通过 Xcode 就可以查看接口和类的相关信息，减少不必要的麻烦。

![Encode 推荐](https://github.com/pili-io/PLCameraStreamingKit/blob/master/header-doc.png?raw=true)

## 功能特性

- [x] 硬件编解码
- [x] 多码率可选
- [x] H.264 视频编码
- [x] AAC 音频编码
- [x] 前后摄像头支持
- [x] 自动对焦支持
- [x] 手动调整对焦点支持
- [x] 闪光灯开关
- [x] 多分辨率编码支持
- [x] HeaderDoc 文档支持

## 系统要求

- iOS Target : >= iOS 7

## 版本历史

- 1.1.6 ([Release Notes](https://github.com/pili-io/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.6.md) && [API Diffs](https://github.com/pili-io/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.6.md))
	- 尝试修复音频变声问题
- 1.1.5 ([Release Notes](https://github.com/pili-io/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.5.md) && [API Diffs](https://github.com/pili-io/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.5.md))
	- 修复了重连后无法播放的问题
- 1.1.4 ([Release Notes](https://github.com/pili-io/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.4.md) && [API Diffs](https://github.com/pili-io/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.4.md))
	- 更新了码率和采样率。现在统一使用 128Kbps 和 44100KHZ，用以解决噪音的问题
	- 更新了网络 socket。对 socket 在调试情况下出现的 SIGPIPE 进行了处理，不会再出现
	- 更新了网络状态的返回情况。当不是主动调用 close 接口导致的断开都将以 error 状态通知 delegate
- 1.1.3 ([Release Notes](https://github.com/pili-io/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.3.md) && [API Diffs](https://github.com/pili-io/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.3.md))
	- 更新了获取视频截图的方法
	- 添加 HeaderDoc 格式注释文档
- 1.1.2 ([Release Notes](https://github.com/pili-io/PLCameraStreamingKit/blob/master/ReleaseNotes/release-notes-1.1.2.md) && [API Diffs](https://github.com/pili-io/PLCameraStreamingKit/blob/master/APIDiffs/api-diffs-1.1.2.md))
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