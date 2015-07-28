# PLCameraStreamingKit Release Notes for 1.2.7

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

### Bug

- 修复断网后停止流时导致的 UI 卡死
- 修复停止推流时概率出现的 crash 问题
- 修复反复进出持有 streamingSession 的 viewController 出现 IO 错误或者 crash 的问题

## 推流 session

- 添加 `- (void)destroy` 方法到 `PLCameraStreamingSession`

## 视频编码

- 暂且去除 iOS 8 编码，待进一步优化后再做开放