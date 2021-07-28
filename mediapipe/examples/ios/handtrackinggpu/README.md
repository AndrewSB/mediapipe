Run the commands from mediapipe root folder

## Generate framework

bazelisk build --copt=-fembed-bitcode --apple_bitcode=embedded --config=ios_arm64 mediapipe/examples/ios/handtrackinggpu:HyperHandTracking --sandbox_debug --verbose_failures

## Patch framework

./mediapipe/examples/ios/handtrackinggpu/patch_ios_framework.sh bazel-bin/mediapipe/examples/ios/handtrackinggpu/HyperHandTracking.zip ObjcppLib.h   

## Run Example

- Create a new iOS project in XCode
- Replace the files of the project with the files in HyperHandTracker folder
- Copy and Paste HyperHandTracking framework in the root of the new project
- Go to Runner (HyperHandTracker) -> General
- Drag the framework to Frameworks, Libraries and Embedded Content
- Embed and Sign the framework 
- Run the app
