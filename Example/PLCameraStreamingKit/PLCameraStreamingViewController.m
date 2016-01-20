//
//  PLCameraStreamingViewController.m
//  PLCameraStreamingKit
//
//  Created on 01/10/2015.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#import "PLCameraStreamingViewController.h"
#import "Reachability.h"
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>
#import <asl.h>

const char *stateNames[] = {
    "Unknow",
    "Connecting",
    "Connected",
    "Disconnecting",
    "Disconnected",
    "Error"
};

const char *networkStatus[] = {
    "Not Reachable",
    "Reachable via WiFi",
    "Reachable via CELL"
};

// 假设在 videoFPS 低于预期 50% 的情况下就触发降低推流质量的操作，这里的 40% 是一个假定数值，你可以更改数值来尝试不同的策略
#define kMaxVideoFPSPercent 0.5

// 假设当 videoFPS 在 10s 内与设定的 fps 相差都小于 5% 时，就尝试调高编码质量
#define kMinVideoFPSPercent 0.05
#define kHigherQualityTimeInterval  10

static NSArray *ConsoleLogs() {
    NSMutableArray *consoleLog = [NSMutableArray array];
    
    aslclient client = asl_open(NULL, NULL, ASL_OPT_STDERR);
    
    aslmsg query = asl_new(ASL_TYPE_QUERY);
    asl_set_query(query, ASL_KEY_MSG, NULL, ASL_QUERY_OP_NOT_EQUAL);
    aslresponse response = asl_search(client, query);
    
    asl_free(query);
    
    aslmsg message;
    while((message = asl_next(response)))
    {
        const char *msg = asl_get(message, ASL_KEY_MSG);
        [consoleLog addObject:[NSString stringWithCString:msg encoding:NSUTF8StringEncoding]];
    }
    
    asl_release(response);
    asl_close(client);
    
    return consoleLog;
}

static NSString *LogString() {
    NSArray *logs = ConsoleLogs();
    NSString *log = [logs componentsJoinedByString:@"\n"];
    return log;
}

@interface PLCameraStreamingViewController ()
<
PLCameraStreamingSessionDelegate,
PLStreamingSendingBufferDelegate
>

@property (nonatomic, strong) PLCameraStreamingSession  *session;
@property (nonatomic, strong) Reachability *internetReachability;
@property (nonatomic, strong) dispatch_queue_t sessionQueue;
@property (nonatomic, strong) NSArray   *videoConfigurations;
@property (nonatomic, strong) NSDate    *keyTime;

@end

@implementation PLCameraStreamingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 预先设定几组编码质量，之后可以切换
    CGSize videoSize = CGSizeMake(320, 480);
    self.videoConfigurations = @[
                                 [PLVideoStreamingConfiguration configurationWithVideoSize:videoSize videoQuality:kPLVideoStreamingQualityMedium1],
                                 [PLVideoStreamingConfiguration configurationWithVideoSize:videoSize videoQuality:kPLVideoStreamingQualityMedium2],
                                 [PLVideoStreamingConfiguration configurationWithVideoSize:videoSize videoQuality:kPLVideoStreamingQualityMedium3],
                                 ];
    self.sessionQueue = dispatch_queue_create("pili.queue.streaming", DISPATCH_QUEUE_SERIAL);
    
    // 网络状态监控
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
    self.internetReachability = [Reachability reachabilityForInternetConnection];
    [self.internetReachability startNotifier];
    
    // PLCameraStreamingKit 使用开始
    //
    // streamJSON 是从服务端拿回的
    //
    // 从服务端拿回的 streamJSON 结构如下：
    //    @{@"id": @"stream_id",
    //      @"title": @"stream_title",
    //      @"hub": @"hub_name",
    //      @"publishKey": @"publish_key",
    //      @"publishSecurity": @"dynamic", // or static
    //      @"disabled": @(NO),
    //      @"profiles": @[@"480p", @"720p"],    // or empty Array []
    //      @"hosts": @{
    //              ...
    //      }
#warning 如果要运行 demo 这里应该填写服务端返回的某个流的 json 信息
    NSDictionary *streamJSON;
    
    PLStream *stream = [PLStream streamWithJSON:streamJSON];
    
    void (^permissionBlock)(void) = ^{
        dispatch_async(self.sessionQueue, ^{
            // 视频编码配置
            PLVideoStreamingConfiguration *videoConfiguration = [self.videoConfigurations lastObject];
            // 音频编码配置
            PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
            
            // 推流 session
            self.session = [[PLCameraStreamingSession alloc] initWithVideoConfiguration:videoConfiguration
                                                                     audioConfiguration:audioConfiguration
                                                                                 stream:stream
                                                                       videoOrientation:AVCaptureVideoOrientationPortrait];
            self.session.delegate = self;
            self.session.bufferDelegate = self;
            dispatch_async(dispatch_get_main_queue(), ^{
                self.session.previewView = self.view;
            });
        });
    };
    
    void (^noAccessBlock)(void) = ^{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"No Access", nil)
                                                            message:NSLocalizedString(@"!", nil)
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                                  otherButtonTitles:nil];
        [alertView show];
    };
    
    switch ([PLCameraStreamingSession cameraAuthorizationStatus]) {
        case PLAuthorizationStatusAuthorized:
            permissionBlock();
            break;
        case PLAuthorizationStatusNotDetermined: {
            [PLCameraStreamingSession requestCameraAccessWithCompletionHandler:^(BOOL granted) {
                granted ? permissionBlock() : noAccessBlock();
            }];
        }
            break;
        default:
            noAccessBlock();
            break;
    }
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kReachabilityChangedNotification object:nil];
    
    dispatch_sync(self.sessionQueue, ^{
        [self.session destroy];
    });
    self.session = nil;
    self.sessionQueue = nil;
}

#pragma mark - Notification Handler

- (void)reachabilityChanged:(NSNotification *)notif{
    Reachability *curReach = [notif object];
    NSParameterAssert([curReach isKindOfClass:[Reachability class]]);
    NetworkStatus status = [curReach currentReachabilityStatus];
    
    if (NotReachable == status) {
        // 对断网情况做处理
        [self stopSession];
    }
    
    NSLog(@"Networkt Status: %s", networkStatus[status]);
    self.textView.text = LogString();
}

#pragma mark - <PLStreamingSendingBufferDelegate>

- (void)streamingSessionSendingBufferDidFull:(id)session {
    NSLog(@"Buffer is full");
    self.textView.text = LogString();
}

- (void)streamingSession:(id)session sendingBufferDidDropItems:(NSArray *)items {
    NSLog(@"Frame dropped");
    self.textView.text = LogString();
}

#pragma mark - <PLCameraStreamingSessionDelegate>

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    NSLog(@"Stream State: %s", stateNames[state]);
    self.textView.text = LogString();
    
    // 除 PLStreamStateError 外的其余状态会回调在这个方法
    // 这个回调会确保在主线程，所以可以直接对 UI 做操作
    if (PLStreamStateConnected == state) {
        [self.actionButton setTitle:NSLocalizedString(@"Stop", nil) forState:UIControlStateNormal];
    } else if (PLStreamStateDisconnected == state) {
        [self.actionButton setTitle:NSLocalizedString(@"Start", nil) forState:UIControlStateNormal];
    }
}

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session didDisconnectWithError:(NSError *)error {
    NSLog(@"Stream State: Error. %@", error);
    self.textView.text = LogString();
    // PLStreamStateError 都会回调在这个方法
    // 尝试重连，注意这里需要你自己来处理重连尝试的次数以及重连的时间间隔
    [self.actionButton setTitle:NSLocalizedString(@"Reconnecting", nil) forState:UIControlStateNormal];
    [self startSession];
}

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStatusDidUpdate:(PLStreamStatus *)status {
    NSLog(@"%@", status);
    self.textView.text = LogString();
    
    NSDate *now = [NSDate date];
    if (!self.keyTime) {
        self.keyTime = now;
    }
    
    double expectedVideoFPS = (double)self.session.videoConfiguration.videoFrameRate;
    double realtimeVideoFPS = status.videoFPS;
    if (realtimeVideoFPS < expectedVideoFPS * (1 - kMaxVideoFPSPercent)) {
        // 当得到的 status 中 video fps 比设定的 fps 的 50% 还小时，触发降低推流质量的操作
        self.keyTime = now;
        
        [self lowerQuality];
    } else if (realtimeVideoFPS >= expectedVideoFPS * (1 - kMinVideoFPSPercent)) {
        if (-[self.keyTime timeIntervalSinceNow] > kHigherQualityTimeInterval) {
            self.keyTime = now;
            
            [self higherQuality];
        }
    }
}

#pragma mark -

- (void)higherQuality {
    NSUInteger idx = [self.videoConfigurations indexOfObject:self.session.videoConfiguration];
    NSAssert(idx != NSNotFound, @"Oops");
    
    if (idx >= self.videoConfigurations.count - 1) {
        return;
    }
    PLVideoStreamingConfiguration *newConfiguration = self.videoConfigurations[idx + 1];
    [self.session reloadVideoConfiguration:newConfiguration];
}

- (void)lowerQuality {
    NSUInteger idx = [self.videoConfigurations indexOfObject:self.session.videoConfiguration];
    NSAssert(idx != NSNotFound, @"Oops");
    
    if (0 == idx) {
        return;
    }
    PLVideoStreamingConfiguration *newConfiguration = self.videoConfigurations[idx - 1];
    [self.session reloadVideoConfiguration:newConfiguration];
}

#pragma mark - Operation

- (void)stopSession {
    dispatch_async(self.sessionQueue, ^{
        self.keyTime = nil;
        [self.session stop];
    });
}

- (void)startSession {
    self.keyTime = nil;
    self.actionButton.enabled = NO;
    dispatch_async(self.sessionQueue, ^{
        [self.session startWithCompleted:^(BOOL success) {
            dispatch_async(dispatch_get_main_queue(), ^{
                self.actionButton.enabled = YES;
            });
        }];
    });
}

#pragma mark - Action

- (IBAction)actionButtonPressed:(id)sender {
    if (PLStreamStateConnected == self.session.streamState) {
        [self stopSession];
    } else {
        [self startSession];
    }
}

- (IBAction)toggleCameraButtonPressed:(id)sender {
    dispatch_async(self.sessionQueue, ^{
        [self.session toggleCamera];
    });
}

- (IBAction)torchButtonPressed:(id)sender {
    dispatch_async(self.sessionQueue, ^{
        self.session.torchOn = !self.session.isTorchOn;
    });
}

- (IBAction)muteButtonPressed:(id)sender {
    dispatch_async(self.sessionQueue, ^{
        self.session.muted = !self.session.isMuted;
    });
}

- (IBAction)cancelButtonPressed:(id)sender {
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

@end
