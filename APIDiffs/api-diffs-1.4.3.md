# PLCameraStreamingKit 1.4.2 to 1.4.3 API Differences

## General Headers

```PLTypeDefines.h```

- *Added* Type `PLStreamError`
- *Modified* Type `PLStreamState`
    - *Added* `PLStreamStateDisconnecting`
- *Modified* Type `PLCameraStreamingSession.h`
    - *Added* Method `- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didDisconnectWithError:(NSError *)error;`
- *Modified* Type `PLAudioStreamingSession.h`
    - *Added* Method `- (void)audioStreamingSession:(PLAudioStreamingSession *)session didDisconnectWithError:(NSError *)error;`