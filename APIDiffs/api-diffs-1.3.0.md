# PLCameraStreamingKit 1.2.8 to 1.3.0 API Differences

## General Headers

```PLVideoStreamingConfiguration.h```

- *Modified* `@property (nonatomic, PL_STRONG) NSString *videoQuality;` from `readonly` to `readwrite`

```PLAudioStreamingConfiguration.h```

- *Modified* `@property (nonatomic, PL_STRONG) NSString   *audioQuality;` from `readonly` to `readwrite`

```PLCameraStreamingSession.h```

- *Added* protocol `PLStreamingSendingBufferDelegate`
- *Added* category `SendingBuffer`