# PLCameraStreamingKit Release Notes for 1.4.3

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

### 推流 session

- 优化网络层调用，添加 `Disconnecting` 流状态
- 抽离可导致推流中断的错误
- 添加错误回调，同时将错误信息返回，便于 debug 及异常处理

## 架构

- 支持 iOS 9 的 bitcode

### bug

- 修复 `destroy` 调用导致的崩溃问题