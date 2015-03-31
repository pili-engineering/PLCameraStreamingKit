//
//  PLViewController.m
//  PLCameraStreamingKit
//
//  Created by 0dayZh on 01/10/2015.
//  Copyright (c) 2014 0dayZh. All rights reserved.
//

#import "PLViewController.h"

#import <PLCameraStreamingKit/PLCameraStreamingKit.h>

#define PUSH_URL    @"YOUR_PUSH_URL_HERE"

@interface PLViewController ()
<
PLCaptureManagerDelegate
>

@property (nonatomic, weak) PLCaptureManager  *captureManager;

@end

@implementation PLViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.captureManager = [PLCaptureManager sharedManager];
    
    
    self.captureManager.pushURL = [NSURL URLWithString:PUSH_URL];
    self.captureManager.streamBitrateMode = PLStreamBitrateMode_160Kbps;
    self.captureManager.delegate = self;
    
    // 检查摄像头是否有授权
    PLCaptureDeviceAuthorizedStatus status = [PLCaptureManager captureDeviceAuthorizedStatus];
    
    if (PLCaptureDeviceAuthorizedStatusUnknow == status) {
        // 未知
        [PLCaptureManager requestCaptureDeviceAccessWithCompletionHandler:^(BOOL granted) {
            if (granted) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    self.captureManager.previewView = self.view;
                    [self.captureManager connect];
                });
            }
        }];
    } else if (PLCaptureDeviceAuthorizedStatusGranted == status) {
        // 已授权
        self.captureManager.previewView = self.view;
        [self.captureManager connect];
    } else {
        // 处理未授权的情况
        NSLog(@"Oops!");
    }
}

#pragma mark - <PLCaptureManagerDelegate>

- (void)captureManager:(PLCaptureManager *)manager streamStateDidChange:(PLStreamState)state {
    switch (state) {
        case PLStreamStateConnected:
            NSLog(@"connected");
            break;
        case PLStreamStateConnecting:
            NSLog(@"connecting");
            break;
        case PLStreamStateDisconnected:
            NSLog(@"disconnected");
            break;
        case PLStreamStateError:
            NSLog(@"error");
            break;
        case PLStreamStateUnknow:
        default:
            NSLog(@"unknow");
            break;
    }
}

@end
