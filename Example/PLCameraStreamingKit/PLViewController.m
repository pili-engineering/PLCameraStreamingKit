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

#warning 这里更改为自己的推流 URL
#define PUSH_URL    @"YOUR_PUSH_URL_HERE"

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
    void (^permissionBlock)(void) = ^{
        PLCameraStreamingConfiguration *configuration = nil;
        
        /**
         // 默认配置
         configuration = [PLCameraStreamingConfiguration defaultConfiguration];
         
         // 指定已有配置
         configuration = [PLCameraStreamingConfiguration configurationWithDimension:PLStreamingDimension_16_9__960x540
         quality:kPLStreamingQualityHigh1];
         */
        
        // 自定义视频大小的配置
        configuration = [PLCameraStreamingConfiguration configurationWithUserDefineDimension:self.view.bounds.size
                                                                                     quality:kPLStreamingQualityHigh1];
        
        self.session = [[PLCameraStreamingSession alloc] initWithConfiguration:configuration videoOrientation:AVCaptureVideoOrientationPortrait];
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
    [self.session startWithPushURL:[NSURL URLWithString:PUSH_URL] completed:^(BOOL success) {
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
