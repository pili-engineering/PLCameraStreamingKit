# PLCameraStreamingKit 1.7.2 to 1.8.0 API Differences

```
PLCameraStreamingSession.h
```
- *Deleted* enum `typedef NSNumber * PLFilterHandler;`
- *Added* method `-(void)setBeautifyModeOn:(BOOL)beautifyModeOn;`
- *Added* method `-(void)setBeautify:(CGFloat)beautify;`
- *Added* method `-(void)setWhiten:(CGFloat)whiten;`
- *Added* method `-(void)setRedden:(CGFloat)redden;`
- *Added* method `-(void)clearWaterMark;`
- *Deleted* method `- (void)removeFilter:(PLFilterHandler)handler;`
- *Modified* method `- (void)adjustSaturation:(CGFloat)saturation forFilter:(PLFilterHandler)handler
;` to `-(void)setWaterMarkWithImage:(UIImage *)wateMarkImage position:(CGPoint)position;`
-

```
PLVideoCaptureConfiguration.h
```

- *Added* property `@property (nonatomic, assign) BOOL horizontallyMirrorFrontFacingCamera;`
- *Added* property `@property (nonatomic, assign) BOOL previewMirrorRearFacing;`
- *Added* property `@property (nonatomic, assign) BOOL streamMirrorFrontFacing;`
- *Added* property `@property (nonatomic, assign) BOOL streamMirrorRearFacing;`
- *Added* property `@property (nonatomic, assign) AVCaptureVideoOrientation videoOrientation;`
- *Modified* method from `- (instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset horizontallyMirrorFrontFacingCamera:(BOOL)horizontallyMirrorFrontFacingCamera horizontallyMirrorRearFacingCamera:(BOOL)horizontallyMirrorRearFacingCamera cameraPosition:(AVCaptureDevicePosition)position;` to `-(instancetype)initWithVideoFrameRate:(NSUInteger)videoFrameRate sessionPreset:(NSString *)sessionPreset previewMirrorFrontFacing:(BOOL)previewMirrorFrontFacing previewMirrorRearFacing:(BOOL)previewMirrorRearFacing streamMirrorFrontFacing:(BOOL)streamMirrorFrontFacing streamMirrorRearFacing:(BOOL)streamMirrorRearFacing cameraPosition:(AVCaptureDevicePosition)position videoOrientation:(AVCaptureVideoOrientation)videoOrientation;`
- *Deleted* property `@property (nonatomic, assign) BOOL horizontallyMirrorFrontFacingCamera;`
- *Deleted* property `@property (nonatomic, assign) BOOL horizontallyMirrorRearFacingCamera;`

```
PLAudioCaptureConfiguration.h
```

- *Deleted* property `@property (nonatomic, assign, readonly) NSUInteger sampleRate;`

## General Headers
