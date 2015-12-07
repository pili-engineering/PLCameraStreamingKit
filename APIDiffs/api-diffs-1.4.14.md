# PLCameraStreamingKit 1.4.13 to 1.4.14 API Differences

## General Headers

```PLTypeDefines.h```

- *Modified* type `PLStreamError`
    - *Added* `PLStreamErrorSocketTimeout`  
- *Modified* type `PLStreamingDimension`
- *Modified* `VideoStreamingQuality`

```PLVideoStreamingConfiguration.h```

- *Added* method `- (instancetype)initWithUserDefineDimension:(CGSize)videoSize
                             videoFrameRate:(NSUInteger)fps
                   videoMaxKeyframeInterval:(NSUInteger)videoMaxKeyframeInterval
                               videoBitrate:(NSUInteger)videoBitRate`
- *Modified* some property from `readonly` to `writable`
