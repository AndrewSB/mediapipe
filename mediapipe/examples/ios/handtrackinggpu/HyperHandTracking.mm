#import "HyperHandTracking.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPLayerRenderer.h"

#include "mediapipe/framework/formats/landmark.pb.h"

static NSString* const kGraphName = @"hand_tracking_mobile_gpu";
static const char* kInputStream = "input_video";
static const char* kLandmarksOutputStream = "hand_landmarks";
static const char* kNumHandsInputSidePacket = "num_hands";

@interface HyperHandTracking() <MPPGraphDelegate>
@property(nonatomic) MPPGraph* mediapipeGraph;
@end

@implementation HyperHandTracking {}

#pragma mark - Cleanup methods

- (void)dealloc {
    self.mediapipeGraph.delegate = nil;
    [self.mediapipeGraph cancel];
    // Ignore errors since we're cleaning up.
    [self.mediapipeGraph closeAllInputStreamsWithError:nil];
    [self.mediapipeGraph waitUntilDoneWithError:nil];
}

#pragma mark - MediaPipe graph methods

+ (MPPGraph*)loadGraphFromResource:(NSString*)resource {
  // Load the graph config resource.
  NSError* configLoadError = nil;
  NSBundle* bundle = [NSBundle bundleForClass:[self class]];
  if (!resource || resource.length == 0) {
    return nil;
  }
  NSURL* graphURL = [bundle URLForResource:resource withExtension:@"binarypb"];
  NSData* data = [NSData dataWithContentsOfURL:graphURL options:0 error:&configLoadError];
  if (!data) {
    NSLog(@"Failed to load MediaPipe graph config: %@", configLoadError);
    return nil;
  }

  // Parse the graph config resource into mediapipe::CalculatorGraphConfig proto object.
  mediapipe::CalculatorGraphConfig config;
  config.ParseFromArray(data.bytes, data.length);

  // Create MediaPipe graph with mediapipe::CalculatorGraphConfig proto object.
  MPPGraph* newGraph = [[MPPGraph alloc] initWithGraphConfig:config];
  return newGraph;
}


- (instancetype)init
{
    self = [super init];
    if (self) {
        self.mediapipeGraph = [[self class] loadGraphFromResource:kGraphName];
        self.mediapipeGraph.delegate = self;
        // Set maxFramesInFlight to a small value to avoid memory contention
        // for real-time processing.
        self.mediapipeGraph.maxFramesInFlight = 2;
        NSLog(@"inited graph %@", kGraphName);
    }
    return self;
}

- (void)startGraph {
    NSError* error;
    if (![self.mediapipeGraph startWithError:&error]) {
        NSLog(@"Failed to start graph: %@", error);
    }
    else if (![self.mediapipeGraph waitUntilIdleWithError:&error]) {
        NSLog(@"Failed to complete graph initial run: %@", error);
    }  
    NSLog(@"Started graph %@", kGraphName);
}

#pragma mark - MPPGraphDelegate methods

// Receives a raw packet from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
     didOutputPacket:(const ::mediapipe::Packet&)packet
          fromStream:(const std::string&)streamName {
  if (streamName == kLandmarksOutputStream) {
    if (packet.IsEmpty()) {
      NSLog(@"[TS:%lld] No hand landmarks", packet.Timestamp().Value());
      return;
    }
    const auto& multiHandLandmarks = packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();
    NSLog(@"[TS:%lld] Number of hand instances with landmarks: %lu", packet.Timestamp().Value(),
          multiHandLandmarks.size());

    if (!multiHandLandmarks.size() > 0) {
        return;
    }

    HandsObservation *observation = [HandsObservation new];

    for (int handIndex = 0; handIndex < multiHandLandmarks.size(); ++handIndex) {
        NSMutableArray<HandLandmark *> *landmarkObservations = [NSMutableArray array];

        const auto& landmarks = multiHandLandmarks[handIndex];
        NSLog(@"\tNumber of landmarks for hand[%d]: %d", handIndex, landmarks.landmark_size());
        for (int i = 0; i < landmarks.landmark_size(); ++i) {
            auto* l= [[HandLandmark alloc] init];
            l.i = i;
            l.x = landmarks.landmark(i).x();
            l.y = landmarks.landmark(i).y();
            l.z = landmarks.landmark(i).z();
            [landmarkObservations addObject:l];
        }

        if (handIndex == 0) {
            observation.left = landmarkObservations;
        } else if (handIndex == 1) {
            observation.right = landmarkObservations;
        } else {
            NSLog(@"Fatal: unknown hand index %d", handIndex);
            return;
        }
    }

    [_delegate didReceive:observation];
  }
}


#pragma mark - MPPInputSourceDelegate methods

- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer {
    NSLog(@"sending imageBuffer to %s", kInputStream);
    auto sent = [self.mediapipeGraph sendPixelBuffer:imageBuffer
                                          intoStream:kInputStream
                                          packetType:MPPPacketTypePixelBuffer];
    if (sent) {
        NSLog(@"imageBuffer sent!");
    }
}

@end
