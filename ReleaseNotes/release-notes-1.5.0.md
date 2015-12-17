# PLCameraStreamingKit Release Notes for 1.5.0

## 内容

- [简介](#简介)
- [问题反馈](#问题反馈)
- [记录](#记录)
	
## 简介

PLCameraStreamingKit 为 iOS 开发者提供直播推流 SDK。

## 问题反馈

当你遇到任何问题时，可以通过在 GitHub 的 repo 提交 ```issues``` 来反馈问题，请尽可能的描述清楚遇到的问题，如果有错误信息也一同附带，并且在 ```Labels``` 中指明类型为 bug 或者其他。

[通过这里查看已有的 issues 和提交 Bug](https://github.com/pili-engineering/PLCameraStreamingKit/issues)

## 记录

### 架构

- 编码推流改为依赖 `PLStreamingKit`
- 摄像头及音频采集部分开源
- 去除 `PLAudioStreamingSession` 类, 建议单纯推音频的 App 直接接入 `PLStreamingKit`

### 网络

- 底层添加了 HappyDNS, 解决部分地区 DNS 感染严重导致推流失败问题
- 优化 TCP 层发包，极大程度解决了网络正常但推流中经常断开的问题