//
//  PLAudioStreamingViewController.m
//  PLCameraStreamingKit
//
//  Created by 0dayZh on 15/9/28.
//  Copyright © 2015年 0dayZh. All rights reserved.
//

#import "PLAudioStreamingViewController.h"
#import "Reachability.h"
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>

extern const char *stateNames[];

extern const char *networkStatus[];

@interface PLAudioStreamingViewController ()
<
PLAudioStreamingSessionDelegate
>

@property (nonatomic, strong) PLAudioStreamingSession  *session;
@property (nonatomic, strong) Reachability *internetReachability;

@end

@implementation PLAudioStreamingViewController

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
    //              ...
    //      }
    NSDictionary *streamJSON;
    PLStream *stream = [PLStream streamWithJSON:streamJSON];
    
    void (^permissionBlock)(void) = ^{
        // 音频编码配置
        PLAudioStreamingConfiguration *audioConfiguration = [PLAudioStreamingConfiguration defaultConfiguration];
        
        // 推流 session
        self.session = [[PLAudioStreamingSession alloc] initWithConfiguration:audioConfiguration stream:stream];
        self.session.delegate = self;
        
        // 可以设置为进入后台持续推流
        self.session.backgroundMode = PLStreamingBackgroundModeKeepAlive;
    };
    
    void (^noAccessBlock)(void) = ^{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"No Access", nil)
                                                            message:NSLocalizedString(@"!", nil)
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                                  otherButtonTitles:nil];
        [alertView show];
    };
    
    switch ([PLAudioStreamingSession microphoneAuthorizationStatus]) {
        case PLAuthorizationStatusAuthorized:
            permissionBlock();
            break;
        case PLAuthorizationStatusNotDetermined: {
            [PLAudioStreamingSession requestMicrophoneAccessWithCompletionHandler:^(BOOL granted) {
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

#pragma mark - <PLAudioStreamingSessionDelegate>

- (void)audioStreamingSession:(PLAudioStreamingSession *)session streamStateDidChange:(PLStreamState)state {
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

- (void)audioStreamingSession:(PLAudioStreamingSession *)session didDisconnectWithError:(NSError *)error {
    NSLog(@"Stream State: Error. %@", error);
    
    [self.actionButton setTitle:NSLocalizedString(@"Start", nil) forState:UIControlStateNormal];
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

- (IBAction)cancelButtonPressed:(id)sender {
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

@end
