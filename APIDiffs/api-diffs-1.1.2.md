# PLCameraStreamingKit 1.1.1 to 1.1.2 API Differences

## General Headers

```
PLCameraStreamingConfiguration.h
```

- *Added* + (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize network:(PLStreamingNetworkType)network

```
PLCameraStreamingSession.h
```

- *Added* - (UIImage *)stillCaptureImage
- *Added* Categroy (Application)
- *Added* Category (CameraSource)
- *Added* Category (MicrophoneSource)
- *Added* @property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable

```
PLTypeDefines.h
```

- *Modified* PLStreamingDimension
	- *Added* PLStreamingDimension_UserDefine