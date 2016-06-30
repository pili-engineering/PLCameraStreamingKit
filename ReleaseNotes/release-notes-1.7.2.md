# PLCameraStreamingKit Release Notes for 1.7.2

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
