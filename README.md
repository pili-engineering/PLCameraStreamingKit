# PLCameraStreamingKit

PLCameraStreamingKit 是为 **pili 流媒体云服务** 流媒体云服务提供的一套推送直播流 SDK, 旨在解决 iOS 端快速、轻松实现 iOS 设备利用摄像头直播接入，便于 **pili 流媒体云服务** 的开发者专注于产品业务本身，而不必在技术细节上花费不必要的时间。

## 内容摘要

- [1 快速开始](#1-快速开始)
	- [1.1 配置工程](#1.1-配置工程)
	- [1.2 示例代码](#1.2-示例代码)
- [2 系统要求](#2-系统要求)
- [3 版本历史](#3-版本历史)

## 1 快速开始

先来看看 PLCameraStreamingKit 接入的步骤

### 1.1 配置工程

- 配置你的 Podfile 文件，添加如下配置信息

```
pod 'PLCameraStreamingKit'
```

- 安装 CocoaPods 依赖

```
pod install
```

- Done! 运行你工程的 workspace

### 1.2 示例代码

在需要的地方添加

```Objective-C
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>
```

PLCaptureManager 是核心类，你只需要关注并使用这个类就可以完成通过摄像头推流、预览的工作

推流前务必要先检查摄像头授权，并记得设置预览界面
```Objective-C
	// 检查摄像头是否有授权
	PLCaptureDeviceAuthorizedStatus status = [PLCaptureManager captureDeviceAuthorizedStatus];
    
    if (PLCaptureDeviceAuthorizedStatusUnknow == status) {
        [PLCaptureManager requestCaptureDeviceAccessWithCompletionHandler:^(BOOL granted) {
            if (granted) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [PLCaptureManager sharedManager].previewView = self.previewView;
                });
            }
        }];
    } else if (PLCaptureDeviceAuthorizedStatusGranted == status) {
        [PLCaptureManager sharedManager].previewView = self.previewView;
    } else {
    	// 处理未授权的情况
    }
```

设置推流地址，这里的推流地址应该是你自己的服务端通过 **pili 流媒体云服务** 请求到的
```Objective-C
    // 设置推流地址
    [PLCaptureManager sharedManager].rtmpPushURL = [NSURL URLWithString:@"YOUR_RTMP_PUSH_URL_HERE"];
```

推流操作
```Objective-C
    // 开始推流
    [[PLCaptureManager sharedManager] connect];
    // 停止推流
    [[PLCaptureManager sharedManager] disconnect];
```

## 2 系统要求

- iOS Target : >= iOS 7

## 3 版本历史

- 0.9.0
	- 初始化 CocoaPods 版本