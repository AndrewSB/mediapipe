#import "ObjcppLib.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPLayerRenderer.h"

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/hyper_full.pb.h"

static NSString* const kGraphName = @"holistic_tracking_gpu";
static const char* kInputStream = "input_video";
static const char* kOutputStream = "output_video";


static const char* kHyperLandmarksOutputStream = "all_landmarks";


@interface HyperHolisticTracking() <MPPGraphDelegate>
@property(nonatomic) MPPGraph* mediapipeGraph;
@end

@interface Landmarks()
- (instancetype)initWithI:(uint32_t)i x:(float)x y:(float)y z:(float)z;
@end

@implementation Landmarks

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

@interface HolisticObservation()
@property NSArray<Landmarks *> *left;
@property NSArray<Landmarks *> *right;
@property NSArray<Landmarks *> *body;
@end


@implementation HolisticObservation

HolisticObservation *selfReference;

@end

@implementation HyperHolisticTracking {}

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
  [newGraph addFrameOutputStream:kOutputStream outputPacketType:MPPPacketTypePixelBuffer];
  [newGraph addFrameOutputStream:kHyperLandmarksOutputStream outputPacketType:MPPPacketTypeRaw];
  
  
  
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
          [_delegate holisticTracker: self didOutputPixelBuffer:pixelBuffer];
      }
}

- (void)mediapipeGraph:(MPPGraph*)graph
     didOutputPacket:(const ::mediapipe::Packet&)packet
          fromStream:(const std::string&)streamName {

if(streamName == kHyperLandmarksOutputStream){
  if (packet.IsEmpty()) {
      // NSLog(@"[TS:%lld] No hyper landmarks", packet.Timestamp().Value());
      return;
    }

    const auto& hyperLandmarks = packet.Get<::mediapipe::HyperFullLandmarks>();
    // NSLog(@"[TS:%lld] Hands Detected : %lu", packet.Timestamp().Value(),
    //       hyperLandmarks.pose_lds().landmark_size());

    HolisticObservation *observation = [[HolisticObservation alloc]init]; 
    if(hyperLandmarks.has_pose_lds()){
        const auto& bodyLandmarks = hyperLandmarks.pose_lds();

        if(bodyLandmarks.landmark_size() >0){
            NSLog(@"[TS:%lld] Body Points count : %lu", packet.Timestamp().Value(),
            bodyLandmarks.landmark_size());
            NSMutableArray<Landmarks *> *landmarkObservations = [NSMutableArray array];
            for (int i = 0; i < bodyLandmarks.landmark_size(); ++i){
                auto* l= [[Landmarks alloc]  initWithI:i
                    x:bodyLandmarks.landmark(i).x()
                    y:bodyLandmarks.landmark(i).y()
                    z:bodyLandmarks.landmark(i).z()];
                    [landmarkObservations addObject:l];
            }

            observation.body = landmarkObservations;
        }
    }

    if(hyperLandmarks.has_lhand_lds()){
        const auto& leftHandLandmarks = hyperLandmarks.lhand_lds();

        if(leftHandLandmarks.landmark_size() >0){
            NSLog(@"[TS:%lld] Left Hand Points count : %lu", packet.Timestamp().Value(),
            leftHandLandmarks.landmark_size());
            NSMutableArray<Landmarks *> *landmarkObservations = [NSMutableArray array];
            for (int i = 0; i < leftHandLandmarks.landmark_size(); ++i){
                auto* l= [[Landmarks alloc]  initWithI:i
                    x:leftHandLandmarks.landmark(i).x()
                    y:leftHandLandmarks.landmark(i).y()
                    z:leftHandLandmarks.landmark(i).z()];
                    [landmarkObservations addObject:l];
            }

            observation.left = landmarkObservations;
        }

    }
    
    if(hyperLandmarks.has_rhand_lds()){
        const auto& rightHandLandmarks = hyperLandmarks.rhand_lds();

        if(rightHandLandmarks.landmark_size() >0){
            NSLog(@"[TS:%lld] Right Hand Points count : %lu", packet.Timestamp().Value(),
            rightHandLandmarks.landmark_size());
            NSMutableArray<Landmarks *> *landmarkObservations = [NSMutableArray array];
            for (int i = 0; i < rightHandLandmarks.landmark_size(); ++i){
                auto* l= [[Landmarks alloc]  initWithI:i
                    x:rightHandLandmarks.landmark(i).x()
                    y:rightHandLandmarks.landmark(i).y()
                    z:rightHandLandmarks.landmark(i).z()];
                    [landmarkObservations addObject:l];
            }

            observation.right = landmarkObservations;
        }
    }
    
    
    
    [_delegate holisticTracker: self  didOutputHolisticObservation:observation];
    

    

    
    
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