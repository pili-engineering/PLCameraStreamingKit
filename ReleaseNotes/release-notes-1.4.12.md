# PLCameraStreamingKit Release Notes for 1.4.12

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

### 摄像头

- 添加 `smoothAutoFocusEnabled` 参数，确保拍摄时对焦的平滑过渡

### bug

- 修复无网络环境或其他情况触发断流时引起的 crash
- 修复频繁切换摄像头时引起的 crash
- 修复内存 leak