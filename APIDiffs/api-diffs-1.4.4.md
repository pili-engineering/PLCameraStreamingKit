# PLCameraStreamingKit 1.4.3 to 1.4.4 API Differences

## General Headers

```PLCameraStreamingSession.h```

- *Added* Property `@property (nonatomic, assign) NSUInteger    maxCount;`
- *Added* Property `@property (nonatomic, assign, readonly) NSUInteger    currentCount;`
- *Deprecated* Property `@property (nonatomic, assign) NSTimeInterval    maxDuration;`
- - *Deprecated* Property `@property (nonatomic, assign, readonly) NSTimeInterval    currentDuration;`