# PLCameraStreamingKit Release Notes for 1.4.5

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

- 为 PLCameraStreamingSession 多个属性添加 kvo 支持
- 由时间计数改为包数计数
- 针对弱网坏境更改丢帧策略，确保不会出现花屏和声音抖动，取而代之会出现间断的无数据，但是可以保证画面和声音的流畅
- 发送队列的默认上下阈值更改为 0.3 和 0.7