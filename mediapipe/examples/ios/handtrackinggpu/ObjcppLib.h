
#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

@class HandsObservation;
@class HyperHandTracking;
@class HandLandmark;

@protocol HyperHandTrackingDelegate <NSObject>
- (void)handTracker: (HyperHandTracking*)handTracker didOutputHandsObservation: (HandsObservation *)handsObservation;
- (void)handTracker: (HyperHandTracking*)handTracker didOutputPixelBuffer: (CVPixelBufferRef)pixelBuffer;
//@optional
//- (void)didReceive:(HandsObservation *)handObservation;
//- (void)recievePixelBuffer:(CVPixelBufferRef)pixelBuffer;
@end

@interface HandLandmark : NSObject 
@property uint32_t i;
@property float x;
@property float y;
@property float z;
@end

@interface Handedness : NSObject
@property uint32_t i;
@property  float score;
@property char label;
@property char display_name;
@end


@interface HandsObservation : NSObject
@property(nonatomic, readonly) NSArray<HandLandmark *> *left;
@property(nonatomic, readonly) NSArray<HandLandmark *> *right;
@end

@interface HyperHandTracking : NSObject
- (instancetype)init;
- (void)startGraph;
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer;
@property(weak, nonatomic) id<HyperHandTrackingDelegate> delegate;
@end





