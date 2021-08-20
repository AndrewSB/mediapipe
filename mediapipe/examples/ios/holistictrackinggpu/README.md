Run the commands from mediapipe root folder

## Generate framework

bazelisk build --copt=-fembed-bitcode --apple_bitcode=embedded --config=ios_arm64 mediapipe/examples/ios/holistictrackinggpu:HyperHolisticTracking --sandbox_debug --verbose_failures

## Patch framework

./mediapipe/examples/ios/holistictrackinggpu/patch_ios_framework.sh bazel-bin/mediapipe/examples/ios/holistictrackinggpu/HyperHolisticTracking.zip ObjcppLib.h   

## Run Example

- Create a new iOS project in XCode
- Replace the files of the project with the files in HyperHolisticTracker folder
- Copy and Paste HyperHolisticTracking framework in the root of the new project
- Go to Runner (HyperHolisticTracker) -> General
- Drag the framework to Frameworks, Libraries and Embedded Content
- Embed and Sign the framework 
- Run the app

## For Holistic, remember to pass CMTime timestamp in process video

Example:

```
func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
          let pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer)
          let timestamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer)
        tracker.processVideoFrame(pixelBuffer, timestamp: timestamp)
        

      }
```


