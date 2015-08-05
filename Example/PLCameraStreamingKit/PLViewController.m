//
//  PLViewController.m
//  PLCameraStreamingKit
//
//  Created on 01/10/2015.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#import "PLViewController.h"
#import "Reachability.h"
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>

const char *stateNames[] = {
    "Unknow",
    "Connecting",
    "Connected",
    "Disconnected",
    "Error"
};

const char *networkStatus[] = {
    "Not Reachable",
    "Reachable via WiFi",
    "Reachable via CELL"
};

@interface PLViewController ()
<
PLCameraStreamingSessionDelegate
>

@property (nonatomic, strong) PLCameraStreamingSession  *session;
@property (nonatomic, strong) Reachability *internetReachability;

@end

@implementation PLViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
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
    //              @"publish": @{
    //                      @"rtmp": @"rtmp_publish_host"
    //                      },
    //              @"play": @{
    //                      @"rtmp": @"rtmp_play_host",
    //                      @"hls": @"hls_play_host"
    //                      }
    //              }
    //      }
    NSDictionary *streamJSON;
    PLStream *stream = [PLStream streamWithJSON:streamJSON];
    
    void (^permissionBlock)(void) = ^{
        // 视频编码配置
        PLVideoStreamingConfiguration *videoConfiguration = [PLVideoStreamingConfiguration configurationWithUserDefineDimension:self.view.bounds.size
                                                                                                                   videoQuality:kPLVideoStreamingQualityMedium2];
        // 音频编码配置
        PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
        
        // 推流 session
        self.session = [[PLCameraStreamingSession alloc] initWithVideoConfiguration:videoConfiguration
                                                                 audioConfiguration:audioConfiguration
                                                                             stream:stream
                                                                   videoOrientation:AVCaptureVideoOrientationPortrait];
        self.session.delegate = self;
        self.session.previewView = self.view;
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
                // 回调确保在主线程，可以安全对 UI 做操作
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
    
    [self.session destroy];
    self.session = nil;
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
}

#pragma mark - <PLCameraStreamingSessionDelegate>

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    NSLog(@"Stream State: %s", stateNames[state]);
    
    if (PLStreamStateConnected == state) {
        [self.actionButton setTitle:NSLocalizedString(@"Stop", nil) forState:UIControlStateNormal];
    } else if (PLStreamStateError == state) {
        // 尝试重连，如果你在霹雳创建的 stream 的 publishSecurity 为 static 时，可以如以下代码一样直接重连;
        // 如果是 dynamic 这里需要重新更新推流地址。注意这里需要你自己来处理重连常识的次数
        [self.actionButton setTitle:NSLocalizedString(@"Reconnecting", nil) forState:UIControlStateNormal];
        [self startSession];
    } else {
        [self.actionButton setTitle:NSLocalizedString(@"Start", nil) forState:UIControlStateNormal];
    }
}

#pragma mark - Operation

- (void)stopSession {
    [self.session stop];
}

- (void)startSession {
    self.actionButton.enabled = NO;
    [self.session startWithCompleted:^(BOOL success) {
        if (success) {
            NSLog(@"Publish URL: %@", self.session.pushURL.absoluteString);
        }
        
        self.actionButton.enabled = YES;
    }];
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
    [self.session toggleCamera];
}

- (IBAction)torchButtonPressed:(id)sender {
    self.session.torchOn = !self.session.isTorchOn;
}

- (IBAction)muteButtonPressed:(id)sender {
    self.session.muted = !self.session.isMuted;
}

@end
