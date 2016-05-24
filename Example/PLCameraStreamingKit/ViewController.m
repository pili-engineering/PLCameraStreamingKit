//
//  ViewController.m
//  PLCameraStreamingKit
//
//  Created by WangSiyu on 5/24/16.
//  Copyright © 2016 Pili Engineering, Qiniu Inc. All rights reserved.
//

#import "ViewController.h"
#import <PLCameraStreamingKit/PLCameraStreamingKit.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [PLVideoCaptureConfiguration defaultConfiguration];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
