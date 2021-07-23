
#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

@interface HandLandmark : NSObject 
- (instancetype)init;
@property uint32_t i;
@property float x;
@property float y;
@property float z;
@end

@interface HandsObservation : NSObject
- (instancetype)init;
@property NSArray<HandLandmark *> *left;
@property NSArray<HandLandmark *> *right;
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
