# PLCameraStreamingKit 1.5.2 to 1.6.0 API Differences

## General Headers

- *Modified* `PLCameraStreamingSessionDelegate`
    - *Added* `- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status;`
- *Added* property `@property (nonatomic, assign) NSTimeInterval    statusUpdateInterval;`
