//
//  ViewController.swift
//  HyperHandTracker
//
//  Created by Jaykumar Patel on 27/07/21.
//

import AVFoundation
import SceneKit
import UIKit

#if canImport(HyperHandTracking)
  // Either import standalone iOS framework...
  import HyperHandTracking

  func hyperHandTrackinghBundle() -> Bundle {
    return Bundle(for: HyperHandTracking.self)
  }
#elseif canImport(mediapipe_examples_ios_handtrackinggpu_HyperHandTracking)
  // ...or import the ObjcppLib target linked using Bazel.
  import mediapipe_examples_ios_handtrackinggpu_HyperHandTracking

  func facialSearchBundle() -> Bundle {
    return Bundle.main
  }
#endif

class ViewController: UIViewController,
  AVCaptureVideoDataOutputSampleBufferDelegate, HyperHandTrackingDelegate
{
  @IBOutlet weak var imageView: UIImageView!
      @IBOutlet weak var toggleView: UISwitch!
      var previewLayer: AVCaptureVideoPreviewLayer!
      @IBOutlet weak var xyLabel:UILabel!
      @IBOutlet weak var featurePoint: UIView!
      let camera = Camera()
      let tracker: HyperHandTracking = HyperHandTracking()!
      
      
      override func viewDidLoad() {
          super.viewDidLoad()
          camera.setSampleBufferDelegate(self)
          camera.start()
          tracker.startGraph()
          tracker.delegate = self
          
  //        previewLayer = AVCaptureVideoPreviewLayer(session: camera.session) as AVCaptureVideoPreviewLayer
  //        previewLayer.frame = view.bounds
  //        view.layer.addSublayer(previewLayer)
  //        view.bringSubviewToFront(xyLabel)
  //        view.bringSubviewToFront(featurePoint)
      }
      
      func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
          let pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer)
          tracker.processVideoFrame(pixelBuffer)

      }
  
  func handTracker(_ handTracker: HyperHandTracking!, didOutputHandsObservation handsObservation: HandsObservation!) {
    print("Right hand:",handsObservation.right ?? "not found")
    print("Left hand:",handsObservation.left ?? "not found" )
    
  }
  
  func handTracker(_ handTracker: HyperHandTracking!, didOutputPixelBuffer pixelBuffer: CVPixelBuffer!) {
    DispatchQueue.main.async {

                    self.imageView.image = UIImage(ciImage: CIImage(cvPixelBuffer: pixelBuffer))

            }
        }
  }
  

