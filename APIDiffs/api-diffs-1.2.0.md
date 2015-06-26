# PLCameraStreamingKit 1.1.8 to 1.2.0 API Differences

## General Headers

```
PLStream.h
```

- *Added* Class `PLStream`

```
PLCameraStreamingKit.h
```

- *Added* Header `#import "PLStream.h"`

```
PLCameraStreamingSession.h
```

- *Added* Property `@property (nonatomic, PL_STRONG) PLStream   *stream;`
- *Added* Property `@property (nonatomic, PL_STRONG) NSString   *rtmpPublishHost;`
- *Added* Method `- (instancetype)initWithConfiguration:(PLCameraStreamingConfiguration *)configuration stream:(PLStream *)stream rtmpPublishHost:(NSString *)rtmpPublishHost videoOrientation:(AVCaptureVideoOrientation)videoOrientation;`
- *Added* Mehtod `- (void)startWithCompleted:(void (^)(BOOL success))handler;`
- *Added* Category `PLCameraStreamingSession (Deprecated)`
- *Deprecated* Method `- (void)startWithPushURL:(NSURL *)pushURL completed:(void (^)(BOOL success))handler`