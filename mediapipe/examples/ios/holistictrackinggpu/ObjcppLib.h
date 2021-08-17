#import <CoreVideo/CoreVideo.h>
#import <AVFoundation/AVFoundation.h>

#import <UIKit/UIKit.h>

@class HolisticObservation;
@class HyperHolisticTracking;
@class Landmarks;

@protocol HyperHolisticTrackingDelegate <NSObject>
- (void)holisticTracker: (HyperHolisticTracking*)holisticTracker didOutputHolisticObservation: (HolisticObservation *)HolisticObservation;
- (void)holisticTracker: (HyperHolisticTracking*)holisticTracker didOutputPixelBuffer: (CVPixelBufferRef)pixelBuffer;

@end



@interface Landmarks : NSObject 
@property uint32_t i;
@property float x;
@property float y;
@property float z;
@property float visibility;
@property float presence;
@end

@interface HolisticObservation : NSObject
@property(nonatomic, readonly) NSArray<Landmarks *> *left;
@property(nonatomic, readonly) NSArray<Landmarks *> *right;
@property(nonatomic, readonly) NSArray<Landmarks *> *body;
@end

@interface HyperHolisticTracking : NSObject
- (instancetype)init;
- (void)startGraph;
// - (void)processVideoFrame:(CVPixelBufferRef)imageBuffer;
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer timestamp:(CMTime)timestamp;
@property(weak, nonatomic) id<HyperHolisticTrackingDelegate> delegate;

@end