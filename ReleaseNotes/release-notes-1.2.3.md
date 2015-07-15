# PLCameraStreamingKit Release Notes for 1.2.3

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

### 接口

- 更新了 `PLStream` 类结构
- 添加新的 session 创建接口
- 移除 publishHost，`PLStream` 对象支持自动生成推流地址，请搭配服务端 SDK 至少 1.2.0 或以上版本使用