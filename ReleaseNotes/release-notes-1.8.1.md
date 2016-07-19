# PLCameraStreamingKit Release Notes for 1.8.1

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
  - 更新依赖的 PLStreamingKit 到 1.2.5
  - 新增自动重连功能，默认关闭
  - 新增模拟器编译支持
  - 水印和美颜功能调整为支持 iPhone 5、iPad 3、iPod touch 4 及以上设备
- 缺陷
  - 修复已知场景可能存在的没有声音的问题
  - 修复填充模式改变可能导致 preview 的 frame 发生改变的问题
  - 修复触摸屏幕对焦无效的问题
  - 修复无法正确设置采集相机位置的问题
  - 修复 capture session 设置不正确可能导致 crash 的问题
  - 修复偶尔出现的画面卡住无法正常推流的问题
