# PLCameraStreamingKit Release Notes for 1.7.0

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
    - 新增后台推流功能（仅音频，因 iOS 隐私限制不允许后台访问摄像头）
    - 新增水印功能
    - 新增可选定制美颜功能（需联系工作人员）
- 缺陷
    - 修复特定机型上的电流音等杂音问题
- 版本
    - 更新依赖 PLStreamingKit 的版本到 `v1.2.0`
- 其他
    - 部分接口重命名
