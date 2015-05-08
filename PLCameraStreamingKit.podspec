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
  s.version          = "1.1.5"
  s.summary          = "Pili iOS camera streaming framework via RTMP."
  s.homepage         = "https://github.com/pili-io/PLCameraStreamingKit"
  s.license          = 'Apache License, Version 2.0'
  s.author           = { "0dayZh" => "0day.zh@gmail.com" }
  s.source           = { :git => "https://github.com/pili-io/PLCameraStreamingKit.git", :tag => s.version.to_s }

  s.platform     = :ios, '7.0'
  s.requires_arc = true

  s.public_header_files = 'Pod/Library/include/**/*.h'
  s.source_files = 'Pod/Library/include/**/*.h'

  s.frameworks = ['UIKit', 'AVFoundation', 'CoreGraphics', 'CFNetwork', 'AudioToolbox', 'CoreMedia', 'VideoToolbox']
  s.libraries = 'z', 'c++'

  s.default_subspec = "precompiled"

  s.subspec "precompiled" do |ss|
    ss.preserve_paths         = "Pod/Library/include/**/*.h", 'Pod/Library/lib/*.a'
    ss.vendored_libraries   = 'Pod/Library/lib/*.a'
    ss.libraries = 'PLCameraStreamingKit'
    ss.xcconfig = { 'HEADER_SEARCH_PATHS' => "${PODS_ROOT}/#{s.name}/PLCameraStreamingKit/lib/include" }
  end
end
