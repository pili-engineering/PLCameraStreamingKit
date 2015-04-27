# PLCameraStreamingKit 1.1.2 to 1.1.3 API Differences

## General Headers

```
PLCameraStreamingSession.h
```

- *Modified* - (void)stillCaptureImage

|      | Description                                                               |
| ---- | ------------------------------------------------------------------------- |
| From | ```- (void)stillCaptureImage```                                           |
| To   | ```- (void)requestCaptureImageWithComplete:(void (^)(UIImage *))block;``` |