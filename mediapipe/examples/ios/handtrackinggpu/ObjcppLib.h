
#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

@class HandsObservation;
@class HyperHandTracking;
@class HandLandmark;

@interface HandLandmark : NSObject 
@property uint32_t i;
@property float x;
@property float y;
@property float z;
@end



@interface HandsObservation : NSObject
@property(nonatomic, readonly) NSArray<HandLandmark *> *left;
@property(nonatomic, readonly) NSArray<HandLandmark *> *right;
@end

@protocol HyperHandTrackingDelegate <NSObject>
@optional
- (void)didReceive:(HandsObservation *)handObservation;
- (void)recievePixelBuffer:(CVPixelBufferRef)pixelBuffer;
@end

@interface HyperHandTracking : NSObject
- (instancetype)init;
- (void)startGraph;
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer;
@property(weak, nonatomic) id<HyperHandTrackingDelegate> delegate;
@end
