# PLCameraStreamingKit Release Notes for 1.1.4

## 内容

- [简介](#简介)
- [问题反馈](#问题反馈)
- [记录](#记录)
	
## 简介

PLCameraStreamingKit 为 iOS 开发者提供直播推流 SDK。

## 问题反馈

当你遇到任何问题时，可以通过在 GitHub 的 repo 提交 ```issues``` 来反馈问题，请尽可能的描述清楚遇到的问题，如果有错误信息也一同附带，并且在 ```Labels``` 中指明类型为 bug 或者其他。

[通过这里查看已有的 issues 和提交 Bug](https://github.com/pili-io/PLCameraStreamingKit/issues)

## 记录

### 音频编码

- 修改了码率和采样率，现在统一使用 128kbps 和 44100KHZ，用以解决噪音的问题
	- 之后会再做细分，但会尽可能避免因码率降低导致的噪音出现

### 网络

- 对 socket 调试情况下出现的 sigpipe 进行了处理，不会再出现
- 更新了网络状态的返回情况，当不是主动调用 close 接口导致的断开都将以 error 状态通知 delegate
	- 之后会对网络错误具体信息进行返回，暂且缺失
