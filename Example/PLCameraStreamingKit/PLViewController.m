//
//  PLViewController.m
//  PLCameraStreamingKit
//
//  Created on 01/10/2015.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#import "PLViewController.h"
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

@interface PLViewController ()
<
PLCameraStreamingSessionDelegate
>

@property (nonatomic, strong) PLCameraStreamingSession  *session;

@end

@implementation PLViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    void (^permissionBlock)(void) = ^{
        PLCameraStreamingConfiguration *configuration = [PLCameraStreamingConfiguration defaultConfiguration];
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

#pragma mark - <PLCameraStreamingSessionDelegate>

- (void)cameraStreamingSession:(PLCameraStreamingSession *)session streamStateDidChange:(PLStreamState)state {
    NSLog(@"Stream State: %s", stateNames[state]);
}

#pragma mark - Action

- (IBAction)actionButtonPressed:(id)sender {
    if (PLStreamStateConnected == self.session.streamState) {
        [self.session stop];
        [self.actionButton setTitle:NSLocalizedString(@"Start", nil) forState:UIControlStateNormal];
    } else {
        self.actionButton.enabled = NO;
        
        [self.session startWithPushURL:[NSURL URLWithString:PUSH_URL] completed:^(BOOL success) {
            self.actionButton.enabled = YES;
            if (success) {
                [self.actionButton setTitle:NSLocalizedString(@"Stop", nil) forState:UIControlStateNormal];
            } else {
                [self.actionButton setTitle:NSLocalizedString(@"Start", nil) forState:UIControlStateNormal];
            }
        }];
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