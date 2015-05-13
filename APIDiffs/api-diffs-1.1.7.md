# PLCameraStreamingKit 1.1.6 to 1.1.7 API Differences

## General Headers

```
PLTypeDefines.h
```

- *Added* NSString *kPLStreamingQualityLow1
- *Added* NSString *kPLStreamingQualityLow2
- *Added* NSString *kPLStreamingQualityMedium1
- *Added* NSString *kPLStreamingQualityMedium2
- *Added* NSString *kPLStreamingQualityMedium3
- *Added* NSString *kPLStreamingQualityHigh1
- *Added* NSString *kPLStreamingQualityHigh2
- *Added* NSString *kPLStreamingQualityHigh3
- *Deprecated* PLStreamingNetworkType

```
PLMacroDefines.h
```

- *Modified* #define PL_LOG_ENABLE 1 -> 0

```
PLCameraStreamingConfiguration.h
```

- *Added* + (instancetype)configurationWithDimension:(PLStreamingDimension)dimension quality:(NSString *)quality
- *Added* + (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize quality:(NSString *)quality
- *Deprecated* + (instancetype)configurationWithDimension:(PLStreamingDimension)dimension network:(PLStreamingNetworkType)network
- *Deprecated* + (instancetype)configurationWithUserDefineDimension:(CGSize)videoSize network:(PLStreamingNetworkType)network
- *Deprecated* @property (nonatomic, assign, readonly) PLStreamingNetworkType networkType

```
PLCameraStreamingSession.h
```

- *Added* NSString *PLCameraDidStartRunningNotificaiton
- *Added* NSString *PLMicrophoneDidStartRunningNotificaiton