// This implements a simple calculator which takes a video stream as input and 
// output it without applyiing any operations on its frames.
#include "absl/strings/str_cat.h"
#include "mediapipe/framework/calculator_framework.h" 
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_imgcodecs_inc.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/hyper_out.pb.h"
#include "mediapipe/framework/port/logging.h"
#include <iostream>
#include <vector>

namespace mediapipe {

class CanvasCalculator : public CalculatorBase {

public:
    CanvasCalculator() = default;
    ~CanvasCalculator() override = default; 
  static ::mediapipe::Status GetContract(CalculatorContract* cc);
  ::mediapipe::Status Open(CalculatorContext* cc) override;
  ::mediapipe::Status Process(CalculatorContext* cc) override;
  };

  REGISTER_CALCULATOR(CanvasCalculator);

::mediapipe::Status CanvasCalculator::GetContract (CalculatorContract *cc){
    cc->Inputs().Tag("MY_LANDMARKS").Set<std::vector<NormalizedLandmarkList>>();
    cc->Inputs().Tag("HANDEDNESS").Set<std::vector<ClassificationList>>();
    cc->Outputs().Tag("HYPER_OUT").Set<HyperOutList>();
    return ::mediapipe::OkStatus();
}

::mediapipe::Status CanvasCalculator::Open(CalculatorContext* cc) {
    return ::mediapipe::OkStatus();
}
::mediapipe::Status CanvasCalculator::Process(CalculatorContext* cc) {

    auto landmarklist = cc->Inputs().Tag("MY_LANDMARKS").Get<std::vector<NormalizedLandmarkList>>();
    auto handednesslist = cc->Inputs().Tag("HANDEDNESS").Get<std::vector<ClassificationList>>();

    for (int i = 0; i < landmarklist.size(); ++i) {
        const NormalizedLandmarkList& landmarks = landmarklist[i];
    
        for (int i = 0; i < landmarks.landmark_size(); ++i) {
            const NormalizedLandmark& landmark = landmarks.landmark(i);
            LOG (WARNING)  << "--------------LANDMARK POINT----------------------" << std::endl;
            LOG (WARNING) << landmark.x() << landmark.y() << landmark.z()  << std::endl; 
        }
    }

    for (int i = 0; i < handednesslist.size(); ++i) {
        const ClassificationList& handedness_clist = handednesslist[i];

        for (int i = 0; i < handedness_clist.classification_size(); ++i) {
            const Classification& handedness = handedness_clist.classification(i);
            LOG (WARNING)  << "-------------HANDEDNESS-----------------------" << std::endl;
            LOG (WARNING) << handedness.label() << std::endl; 
        } 
    }

    HyperOutList* output_vector = new HyperOutList();
    // output_vector->InitAsDefaultInstance();
    Packet data;
    // output_vector->add_normalised_landmark_list(landmarklist);
    // output_vector->add_handedness_classification_list(handednesslist);

    cc->Outputs().Tag("HYPER_OUT").AddPacket(data);
    return ::mediapipe::OkStatus();
// after defining calculator class, we need to register it with a macro invocation 
// REGISTER_CALCULATOR(calculator_class_name).
REGISTER_CALCULATOR(::mediapipe::CanvasCalculator);
}
}
 //end namespace