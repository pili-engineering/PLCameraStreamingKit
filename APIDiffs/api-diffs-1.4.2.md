# PLCameraStreamingKit 1.4.1 to 1.4.2 API Differences

## General Headers

- *Added* `PLBuffer.h`

```PLAudioStreamingSession.h```

- *Added* Category `SendingBuffer`
    - *Added* `@property (nonatomic, PL_WEAK) id<PLStreamingSendingBufferDelegate> bufferDelegate;`
    - *Added* `@property (nonatomic, assign) CGFloat    lowThreshold;`
    - *Added* `@property (nonatomic, assign) CGFloat    highThreshold;`
    - *Added* `@property (nonatomic, assign) NSTimeInterval    maxDuration;`
    - *Added* `@property (nonatomic, assign, readonly) NSTimeInterval    currentDuration;`