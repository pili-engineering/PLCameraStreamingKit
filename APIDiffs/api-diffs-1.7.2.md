# PLCameraStreamingKit 1.7.1 to 1.7.2 API Differences

```
PLCameraStreamingSession.h
```

- *Added* enum `PLCameraError`
- *Added* property `@property (nonatomic, PL_STRONG) dispatch_queue_t delegateQueue;`
- *Added* property `@property (nonatomic, assign) float inputGain;`
- *Added* method `- (void)restartWithCompleted:(void (^)(BOOL success))handler;`

```
PLVideoCaptureConfiguration.h
```

- *Added* property `@property (nonatomic, assign) AVCaptureDevicePosition position;`
- *Modified* method `- (instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset horizontallyMirrorFrontFacingCamera:(BOOL)horizontallyMirrorFrontFacingCamera horizontallyMirrorRearFacingCamera:(BOOL)horizontallyMirrorRearFacingCamera;` to `- (instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset horizontallyMirrorFrontFacingCamera:(BOOL)horizontallyMirrorFrontFacingCamera horizontallyMirrorRearFacingCamera:(BOOL)horizontallyMirrorRearFacingCamera cameraPosition:(AVCaptureDevicePosition)position;`

```
PLAudioCaptureConfiguration.h
```

- *Deleted* property `@property (nonatomic, assign, readonly) NSUInteger sampleRate;`

## General Headers
