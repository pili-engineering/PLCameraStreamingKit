# PLCameraStreamingKit 1.6.0 to 1.6.1 API Differences

## General Headers

- *Modified* `PLCameraStreamingSessionDelegate`
    - *Added* `- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status;`
- *Added* property `@property (nonatomic, assign) NSTimeInterval    statusUpdateInterval;`