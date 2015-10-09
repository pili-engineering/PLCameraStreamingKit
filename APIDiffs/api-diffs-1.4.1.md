# PLCameraStreamingKit 1.4.0 to 1.4.1 API Differences

## General Headers

```PLCameraStreamingSession.h```

- *Modified* protocol `PLCameraStreamingSessionDelegate`
    - *Added* method `- (CMSampleBufferRef)cameraStreamingSession:(PLCameraStreamingSession *)session cameraSourceDidGetSampleBuffer:(CMSampleBufferRef)sampleBuffer;`