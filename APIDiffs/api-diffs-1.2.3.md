# PLCameraStreamingKit 1.2.2 to 1.2.3 API Differences

## General Headers

```PLCameraStreamingSession.h```

- *Added* `- (instancetype)initWithConfiguration:(PLCameraStreamingConfiguration *)configuration
                               stream:(PLStream *)stream
                     videoOrientation:(AVCaptureVideoOrientation)videoOrientation`

- *Deprecated* `- (instancetype)initWithConfiguration:(PLCameraStreamingConfiguration *)configuration
                               stream:(PLStream *)stream
                      rtmpPublishHost:(NSString *)rtmpPublishHost
                     videoOrientation:(AVCaptureVideoOrientation)videoOrientation`

```PLStream.h```

- *Added* `@property (nonatomic, strong) NSArray   *profiles;`
- *Added* `@property (nonatomic, strong) NSDictionary   *hosts;`