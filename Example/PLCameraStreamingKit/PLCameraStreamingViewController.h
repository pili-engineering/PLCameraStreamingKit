//
//  PLCameraStreamingViewController.h
//  PLCameraStreamingKit
//
//  Created on 01/10/2015.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PLCameraStreamingViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIButton *actionButton;
@property (weak, nonatomic) IBOutlet UIButton *toggleCameraButton;
@property (weak, nonatomic) IBOutlet UIButton *torchButton;
@property (weak, nonatomic) IBOutlet UIButton *muteButton;
@property (weak, nonatomic) IBOutlet UITextView *textView;

@end
