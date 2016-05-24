# PLCameraStreamingKit 1.6.3 to 1.7.0 API Differences

## General Headers

- *Added* Header `PLVideoCaptureConfiguration.h`
- *Added* Class `PLVideoCaptureConfiguration`
- *Added* Header `PLAudioCaptureConfiguration.h`
- *Added* Class `PLAudioCaptureConfiguration`

```
PLVideoCaptureConfiguration.h
```

- *Added* method `+ (instancetype)defaultConfiguration;`
- *Added* method `- (instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset horizontallyMirrorFrontFacingCamera:(BOOL)horizontallyMirrorFrontFacingCamera horizontallyMirrorRearFacingCamera:(BOOL)horizontallyMirrorRearFacingCamera;`
- *Added* property `@property (nonatomic, assign) NSUInteger videoFrameRate;`
- *Added* property `@property (nonatomic, copy) NSString *sessionPreset;`
- *Added* property `@property (nonatomic, assign) BOOL horizontallyMirrorFrontFacingCamera;`
- *Added* property `@property (nonatomic, assign) BOOL horizontallyMirrorRearFacingCamera;`

```
PLAudioCaptureConfiguration.h
```

- *Added* method `+ (instancetype)defaultConfiguration;`
- *Added* property `@property (nonatomic, assign, readonly) NSUInteger sampleRate;`
- *Added* property `@property (nonatomic, assign) NSUInteger channelsPerFrame;`

```
PLCameraStreamingSession.h
```

- *Modified* method `- (instancetype)initWithVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration audioConfiguration:(PLAudioStreamingConfiguration *)audioConfiguration stream:(PLStream *)stream videoOrientation:(AVCaptureVideoOrientation)videoOrientation;` to `- (instancetype)initWithVideoCaptureConfiguration:(PLVideoCaptureConfiguration *)videoCaptureConfiguration audioCaptureConfiguration:(PLAudioCaptureConfiguration *)audioCaptureConfiguration videoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration audioStreamingConfiguration:(PLAudioStreamingConfiguration *)audioStreamingConfiguration stream:(PLStream *)stream videoOrientation:(AVCaptureVideoOrientation)videoOrientation;`
- *Modified* method `- (void)reloadVideoConfiguration:(PLVideoStreamingConfiguration *)videoConfiguration;` to `- (void)reloadVideoStreamingConfiguration:(PLVideoStreamingConfiguration *)videoStreamingConfiguration videoCaptureConfiguration:(PLVideoCaptureConfiguration *)videoCaptureConfiguration;`

- *Deleted* method `- (void)updatePreviewViewSize:(CGSize)size;`
- *Deleted* property `@property (nonatomic, PL_STRONG) PLVideoStreamingConfiguration  *videoConfiguration;`
- *Deleted* property `@property (nonatomic, PL_STRONG) PLAudioStreamingConfiguration  *audioConfiguration;`

- *Added* typeDefine `typedef NSNumber * PLFilterHandler;`
- *Added* enum `PLVideoFillModeType`  
- *Added* property `@property (nonatomic, copy, readonly) PLVideoCaptureConfiguration  *videoCaptureConfiguration;`
- *Added* property `@property (nonatomic, copy, readonly) PLAudioCaptureConfiguration  *audioCaptureConfiguration;`
- *Added* property `@property (nonatomic, copy, readonly) PLVideoStreamingConfiguration  *videoStreamingConfiguration;`
- *Added* property `@property (nonatomic, copy, readonly) PLAudioStreamingConfiguration  *audioStreamingConfiguration;`
- *Added* property `@property(readwrite, nonatomic) PLVideoFillModeType fillMode;`
- *Added* property `@property (nonatomic, strong, readonly) NSArray<AVCaptureDeviceFormat *> *videoFormats;`
- *Added* method `- (PLFilterHandler)addWaterMark:(UIImage *)waterMark origin:(CGPoint)origin;`
- *Added* method `- (PLFilterHandler)addBeautyFilter;`
- *Added* method `- (PLFilterHandler)addGPUImageFilter:(GPUImageFilter *)filter;`
- *Added* method `- (void)removeFilter:(PLFilterHandler)handler;`
- *Added* method `- (void)adjustBritness:(CGFloat)brightness forFilter:(PLFilterHandler)handler;`
- *Added* method `- (void)adjustSaturation:(CGFloat)saturation forFilter:(PLFilterHandler)handler;`
- *Added* method `- (void)resetBritnessAndSaturationForFilter:(PLFilterHandler)handler;`
- *Added* method `- (void)setSmoothIntensity:(CGFloat)smoothIntensity forFilter:(PLFilterHandler)handler;`
