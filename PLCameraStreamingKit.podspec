#
# Be sure to run `pod lib lint PLCameraStreamingKit.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# Any lines starting with a # are optional, but encouraged
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = "PLCameraStreamingKit"
  s.version          = "1.7.1"
  s.summary          = "Pili iOS camera streaming framework via RTMP."
  s.homepage         = "https://github.com/pili-engineering/PLCameraStreamingKit"
  s.license          = 'Apache License, Version 2.0'
  s.author           = { "hzwangsiyu" => "hzwangsiyu@gmail.com" }
  s.source           = { :git => "https://github.com/pili-engineering/PLCameraStreamingKit.git", :tag => "v#{s.version}" }

  s.platform     = :ios
  s.requires_arc = true

  s.public_header_files = 'Pod/Library/include/PLCameraStreamingKit/*.h'
  s.source_files = 'Pod/Library/include/PLCameraStreamingKit/*.h'

  s.dependency 'PLStreamingKit', '~> 1.2.0'
  s.dependency 'GPUImage', '0.1.7'
  s.frameworks = ['UIKit', 'AVFoundation', 'CoreGraphics', 'CFNetwork', 'AudioToolbox', 'CoreMedia', 'VideoToolbox']
  s.libraries = 'z', 'c++'

  s.default_subspec = "precompiled"

  s.subspec "precompiled" do |ss|
    ss.vendored_libraries   = 'Pod/Library/lib/*.a'
  end
end
