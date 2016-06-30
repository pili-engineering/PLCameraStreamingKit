# PLCameraStreamingKit Release Notes for 1.8.0

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
