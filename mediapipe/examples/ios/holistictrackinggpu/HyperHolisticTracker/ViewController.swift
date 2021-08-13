//
//  ViewController.swift
//  HyperHandTracker
//
//  Created by Jaykumar Patel on 27/07/21.
//

import AVFoundation
import SceneKit
import UIKit

#if canImport(HyperHolisticTracking)
  // Either import standalone iOS framework...
  import HyperHolisticTracking

  func hyperHolisticTrackinghBundle() -> Bundle {
    return Bundle(for: HyperHolisticTracking.self)
  }
#elseif canImport(mediapipe_examples_ios_holistictrackinggpu_HyperHolisticTracking)
  // ...or import the ObjcppLib target linked using Bazel.
  import mediapipe_examples_ios_holistictrackinggpu_HyperHolisticTracking

  func facialSearchBundle() -> Bundle {
    return Bundle.main
  }
#endif

class ViewController: UIViewController,
  AVCaptureVideoDataOutputSampleBufferDelegate, HyperHolisticTrackingDelegate
{
  @IBOutlet weak var imageView: UIImageView!
      @IBOutlet weak var toggleView: UISwitch!
      var previewLayer: AVCaptureVideoPreviewLayer!
      @IBOutlet weak var xyLabel:UILabel!
      @IBOutlet weak var featurePoint: UIView!
      let camera = Camera()
      let tracker: HyperHolisticTracking = HyperHolisticTracking()!
      
      
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
  
  func holisticTracker(_ holisticTracker: HyperHolisticTracking!, didOutputHolisticObservation holisticObservation: HolisticObservation!) {
    print("Right hand:",holisticObservation.right ?? "not found")
    print("Left hand:",holisticObservation.left ?? "not found" )
    print("Body points:",holisticObservation.body ?? "not found" )
    
  }
  
  func holisticTracker(_ holisticTracker: HyperHolisticTracking!, didOutputPixelBuffer pixelBuffer: CVPixelBuffer!) {
    DispatchQueue.main.async {
      
//      print("pixel buffer : ",pixelBuffer ?? "not found")

                    self.imageView.image = UIImage(ciImage: CIImage(cvPixelBuffer: pixelBuffer))

            }
        }
  }
  

