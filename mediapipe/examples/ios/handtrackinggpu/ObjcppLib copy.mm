#import "ObjcppLib.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPLayerRenderer.h"

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"

static NSString* const kGraphName = @"hand_tracking_mobile_gpu";
static const char* kInputStream = "input_video";
static const char* kOutputStream = "output_video";
static const char* kLandmarksOutputStream = "hand_landmarks";
static const char* kNumHandsInputSidePacket = "num_hands";
static const char* kHandednessOutputStream = "handedness";
NSMutableArray *listOfLandmarks;

//Hands to detect/process
static const int kNumHands = 2;

@interface HyperHandTracking() <MPPGraphDelegate>
@property(nonatomic) MPPGraph* mediapipeGraph;
@end

@interface HandLandmark()
- (instancetype)initWithI:(uint32_t)i x:(float)x y:(float)y z:(float)z;
@end

@implementation HandLandmark

- (instancetype)initWithI:(uint32_t)i x:(float)x y:(float)y z:(float)z
{
    self = [super init];
    if (self) {
        _i = i;
        _x = x;
        _y = y;
        _z = z;
    }
    return self;
}

@end

@interface HandsObservation()
@property NSArray<HandLandmark *> *left;
@property NSArray<HandLandmark *> *right;
@end


@implementation HandsObservation

HandsObservation *selfReference;

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
  
  // Define the output streams to be accessed with graph
  [newGraph setSidePacket:(mediapipe::MakePacket<int>(kNumHands)) named:kNumHandsInputSidePacket];
  [newGraph addFrameOutputStream:kOutputStream outputPacketType:MPPPacketTypePixelBuffer];
  [newGraph addFrameOutputStream:kLandmarksOutputStream outputPacketType:MPPPacketTypeRaw];
  [newGraph addFrameOutputStream:kHandednessOutputStream outputPacketType:MPPPacketTypeRaw];
  
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

- (void)mediapipeGraph:(MPPGraph*)graph
  didOutputPixelBuffer:(CVPixelBufferRef)pixelBuffer
            fromStream:(const std::string&)streamName {
      if (streamName == kOutputStream) {
          [_delegate handTracker: self didOutputPixelBuffer:pixelBuffer];
      }
}

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
    
    listOfLandmarks = [NSMutableArray array];

//    HandsObservation *observation = [[HandsObservation alloc]init];
    

    for (int handIndex = 0; handIndex < multiHandLandmarks.size(); ++handIndex) {
        NSMutableArray<HandLandmark *> *landmarkObservations = [NSMutableArray array];

        const auto& landmarks = multiHandLandmarks[handIndex];
        NSLog(@"\tNumber of landmarks for hand[%d]: %d", handIndex, landmarks.landmark_size());
        for (int i = 0; i < landmarks.landmark_size(); ++i) {
            auto* l= [[HandLandmark alloc]  initWithI:i
            x:landmarks.landmark(i).x()
            y:landmarks.landmark(i).y()
            z:landmarks.landmark(i).z()];
            [landmarkObservations addObject:l];
        }
      
      [listOfLandmarks insertObject:landmarkObservations atIndex: handIndex];

//        if (handIndex == 0) {
//            observation.left = landmarkObservations;
//        } else if (handIndex == 1) {
//            observation.right = landmarkObservations;
//        } else {
//            NSLog(@"Fatal: unknown hand index %d", handIndex);
//            return;
//        }
    }
    NSLog(@"[TS:%lld] List of landmarks: %lu", packet.Timestamp().Value(), [listOfLandmarks count]);

//    [_delegate handTracker: self  didOutputHandsObservation:observation];
  }
  
  if(streamName == kHandednessOutputStream){
    if (packet.IsEmpty()) {
      NSLog(@"[TS:%lld] No handedness", packet.Timestamp().Value());
      return;
    }
    const auto& multiHandedness = packet.Get<std::vector<::mediapipe::ClassificationList>>();
    
    NSLog(@"[TS:%lld] Number of hand instances with handedness: %lu", packet.Timestamp().Value(),
          multiHandedness.size());
    
    

    if (!multiHandedness.size() > 0) {
        return;
    }
    
    HandsObservation *observation = [[HandsObservation alloc]init];
    
    
    for (int handIndex = 0; handIndex < multiHandedness.size(); ++handIndex) {
      const auto& handedness = multiHandedness[handIndex];
//      NSLog(@"[TS:%lld] Which hand of index %lu: %lu", packet.Timestamp().Value(),handIndex,
//            handedness.classification_size());
      
      for (int i = 0; i < handedness.classification_size(); ++i) {
        const auto& displayName=handedness.classification(i).display_name().c_str();
        NSLog(@"[TS:%lld] Which hand of index %lu: %lu", packet.Timestamp().Value(),handIndex,
              displayName);
        
        if(displayName == "right"){
          observation.right = listOfLandmarks[handIndex];
        } else if(displayName == "left") {
          observation.right = listOfLandmarks[handIndex];
        } else {
                      NSLog(@"Fatal: unknown hand index %d", handIndex);
                      return;
                  }
      }
      
    }
    
    
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




