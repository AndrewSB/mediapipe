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

    // for (int i = 0; i < landmarklist.size(); ++i) {
    //     const NormalizedLandmarkList& output_landmarks = landmarklist[i];
    
    //     for (int i = 0; i < output_landmarks.landmark_size(); ++i) {
    //         const NormalizedLandmark& landmark = output_landmarks.landmark(i);
    //         LOG (WARNING)  << "--------------LANDMARK POINT----------------------" << std::endl;
    //         LOG (WARNING) << landmark.x() << landmark.y() << landmark.z()  << std::endl; 
    //     }
    // }

    // for (int i = 0; i < handednesslist.size(); ++i) {
    //     const ClassificationList& handedness_clist = handednesslist[i];

    //     for (int i = 0; i < handedness_clist.classification_size(); ++i) {
    //         const Classification& handedness = handedness_clist.classification(i);
    //         LOG (WARNING)  << "-------------HANDEDNESS-----------------------" << std::endl;
    //         LOG (WARNING) << handedness.label() << std::endl; 
    //     } 
    // }

    HyperOutList *output_vector = new HyperOutList();

    NormalizedLandmarkList output_norm_landmarks;
    for (int i = 0; i < landmarklist.size(); ++i)
    {
        const NormalizedLandmarkList &output_landmarks = landmarklist[i];
        for (int i = 0; i < output_landmarks.landmark_size(); ++i)
        {
            const NormalizedLandmark &landmark = output_landmarks.landmark(i);
            NormalizedLandmark *norm_landmark = output_norm_landmarks.add_landmark();
            norm_landmark->set_x(landmark.x());
            norm_landmark->set_y(landmark.y());
            // Scale Z coordinate as X + allow additional uniform normalization.
            norm_landmark->set_z(landmark.z());
            if (landmark.has_visibility())
            { // Set only if supported in the model.
                norm_landmark->set_visibility(landmark.visibility());
            }
            if (landmark.has_presence())
            { // Set only if supported in the model.
                norm_landmark->set_presence(landmark.presence());
            }
        }
    }

    ClassificationList output_handedness_list;
    for (int i = 0; i < handednesslist.size(); ++i)
    {
        const ClassificationList& handedness_clist = handednesslist[i];
        for (int i = 0; i < handedness_clist.classification_size(); ++i) {
            
            const Classification& handedness = handedness_clist.classification(i);
            Classification *handedness_out = output_handedness_list.add_classification();
            if (handedness.has_label()){
                handedness_out->set_label(handedness.label());
            }
            if (handedness.has_index()){
                handedness_out->set_index(handedness.index());
            }
            if (handedness.has_score()){
                handedness_out->set_score(handedness.score());
            }
            if (handedness.has_display_name()){
                handedness_out->set_display_name(handedness.display_name());
            }
        }
    }

    HyperOutList hyper_out_lds;
    // output_vector->InitAsDefaultInstance();
    // Packet data;
    hyper_out_lds.set_allocated_normalised_landmark_list(&output_norm_landmarks);
    hyper_out_lds.set_allocated_handedness_classification_list(&output_handedness_list);

    // auto hyper_out_lds = absl::make_unique<HyperOutList>();
    // HyperOut* hyper_out = hyper_out_lds->add_normalised_landmark_list();

    // LOG (WARNING)  << "-------------HANDEDNESS-----------------------" << std::endl;
    // LOG (WARNING) << handedness.label() << std::endl; 

    cc->Outputs().Tag("HYPER_OUT").AddPacket(
        MakePacket<HyperOutList>(hyper_out_lds).At(
            cc->InputTimestamp()));

    return ::mediapipe::OkStatus();
// after defining calculator class, we need to register it with a macro invocation 
// REGISTER_CALCULATOR(calculator_class_name).
REGISTER_CALCULATOR(::mediapipe::CanvasCalculator);
}
}
 //end namespace