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

REGISTER_CALCULATOR(::mediapipe::CanvasCalculator);

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

    LOG (WARNING)  << "-------------1-----------------------" << std::endl;

    auto landmarklist = cc->Inputs().Tag("MY_LANDMARKS").Get<std::vector<NormalizedLandmarkList>>();
    LOG (WARNING)  << "-------------2-----------------------" << std::endl;

    auto handednesslist = cc->Inputs().Tag("HANDEDNESS").Get<std::vector<ClassificationList>>();
    LOG (WARNING)  << "-------------3-----------------------" << std::endl;

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

    // HyperOutList *output_vector = new HyperOutList();

    // HyperOutList hyper_out_ldslist;
    HyperOutList* hyper_out_ldslist = new HyperOutList();
    LOG (WARNING)  << "-------------4-----------------------" << std::endl;

    hyper_out_ldslist->InitAsDefaultInstance();
    LOG (WARNING)  << "-------------5-----------------------" << std::endl;


    // NormalizedLandmarkList output_norm_landmarks;
    // ClassificationList output_handedness_list;

    for (int i = 0; i < landmarklist.size(); ++i)
    {
        // HyperOut *hyper_out_ld = hyper_out_ldslist->add_hyper_out();

        HyperOut* hyperOut = hyper_out_ldslist->add_hyper_out();
        LOG (WARNING)  << "-------------6-----------------------" << std::endl;

        const NormalizedLandmarkList &output_landmarks = landmarklist[i];
        for (int i = 0; i < output_landmarks.landmark_size(); ++i)
        {
            const NormalizedLandmark &landmark = output_landmarks.landmark(i);
            LOG (WARNING)  << "-------------7-----------------------" << std::endl;

            // NormalizedLandmark *norm_landmark = output_norm_landmarks.add_landmark();
            // hyper_out_ldslist->mutable_hyper_out()->normalised_landmark_list()->add_landmark();
            int size =  hyperOut->mutable_normalised_landmark_list()->landmark_size()-1;
            LOG (WARNING)  << "-------------8-----------------------" << size << " " << landmark.x() << landmark.y() << landmark.z() << std::endl;

            // if (landmark.has_x())
            // { 
                hyperOut->mutable_normalised_landmark_list()->mutable_landmark(size)->set_x(landmark.x());
                LOG (WARNING)  << "-------------9-----------------------" << std::endl;

            // }
            if (landmark.has_y())
            { 
                hyperOut->mutable_normalised_landmark_list()->mutable_landmark(size)->set_y(landmark.y());
                LOG (WARNING)  << "-------------10-----------------------" << std::endl;

            }
            if (landmark.has_z())
            { 
                hyperOut->mutable_normalised_landmark_list()->mutable_landmark(size)->set_z(landmark.z());
                LOG (WARNING)  << "-------------11-----------------------" << std::endl;

            }
            if (landmark.has_visibility())
            {   
                hyperOut->mutable_normalised_landmark_list()->mutable_landmark(size)->set_visibility(landmark.visibility());
                LOG (WARNING)  << "-------------12-----------------------" << std::endl;

            }
            if (landmark.has_presence())
            {
                hyperOut->mutable_normalised_landmark_list()->mutable_landmark(size)->set_presence(landmark.presence());
                LOG (WARNING)  << "-------------13-----------------------" << std::endl;

            }
        }

        const ClassificationList& handedness_clist = handednesslist[i];
        LOG (WARNING)  << "-------------14-----------------------" << std::endl;

        for (int i = 0; i < handedness_clist.classification_size(); ++i) {
            
            const Classification& handedness = handedness_clist.classification(i);
            LOG (WARNING)  << "-------------15-----------------------" << std::endl;

            int size =  hyperOut->mutable_handedness_classification_list()->classification_size()-1;
            LOG (WARNING)  << "-------------16-----------------------" << std::endl;


            // Classification *handedness_out = output_handedness_list.add_classification();
            
            if (handedness.has_label()){
                hyperOut->mutable_handedness_classification_list()->mutable_classification(size)->set_label(handedness.label());
                LOG (WARNING)  << "-------------17-----------------------" << std::endl;

            }
            if (handedness.has_index()){
                hyperOut->mutable_handedness_classification_list()->mutable_classification(size)->set_index(handedness.index());
                LOG (WARNING)  << "-------------18-----------------------" << std::endl;

            }
            if (handedness.has_score()){
                // handedness_out->set_score(handedness.score());
                hyperOut->mutable_handedness_classification_list()->mutable_classification(size)->set_score(handedness.score());
                LOG (WARNING)  << "-------------19-----------------------" << std::endl;

            }
            if (handedness.has_display_name()){
                // handedness_out->set_display_name(handedness.display_name());
                hyperOut->mutable_handedness_classification_list()->mutable_classification(size)->set_display_name(handedness.display_name());
                LOG (WARNING)  << "-------------20-----------------------" << std::endl;


            }
        }
    }

    // ClassificationList output_handedness_list;
    // for (int i = 0; i < handednesslist.size(); ++i)
    // {
    //     const ClassificationList& handedness_clist = handednesslist[i];
    //     for (int i = 0; i < handedness_clist.classification_size(); ++i) {
            
    //         const Classification& handedness = handedness_clist.classification(i);
    //         Classification *handedness_out = output_handedness_list.add_classification();
            
    //         if (handedness.has_label()){
    //             handedness_out->set_label(handedness.label());
    //         }
    //         if (handedness.has_index()){
    //             handedness_out->set_index(handedness.index());
    //         }
    //         if (handedness.has_score()){
    //             handedness_out->set_score(handedness.score());
    //         }
    //         if (handedness.has_display_name()){
    //             handedness_out->set_display_name(handedness.display_name());
    //         }
    //     }
    // }

    // auto hyper_out_lds = std::make_unique<HyperOutList>();
    // var hyper_out_lds HyperOutList;
    // hyper_out_lds->InitAsDefaultInstance();
    // Packet data;
    
    // hyper_out_lds.set_allocated_normalised_landmark_list(&output_norm_landmarks);
    // hyper_out_lds.set_allocated_handedness_classification_list(&output_handedness_list);

    // auto hyper_out_lds = absl::make_unique<HyperOutList>();
    // HyperOut* hyper_out = hyper_out_lds->add_normalised_landmark_list();

    // LOG (WARNING)  << "-------------HANDEDNESS-----------------------" << std::endl;
    // LOG (WARNING) << handedness.label() << std::endl; 


    std::unique_ptr<mediapipe::HyperOutList> output_stream_collection = std::make_unique<mediapipe::HyperOutList>(*hyper_out_ldslist); 
    LOG (WARNING)  << "-------------21-----------------------" << std::endl;

    cc->Outputs().Tag("HYPER_OUT").Add(output_stream_collection.release(), cc->InputTimestamp());

    LOG (WARNING)  << "-------------22-----------------------" << std::endl;


    // cc->Outputs().Tag("HYPER_OUT").AddPacket(
    //     MakePacket<HyperOutList>(hyper_out_ldslist).At(
    //         cc->InputTimestamp()));

    // cc->Outputs().Tag("HYPER_OUT").Add(hyper_out_lds,cc->InputTimestamp()));

    return ::mediapipe::OkStatus();
// after defining calculator class, we need to register it with a macro invocation 
// REGISTER_CALCULATOR(calculator_class_name).
}

::mediapipe::Status Close(CalculatorContext* cc) {
    if (!cc->GraphStatus().ok()) {
      return ::mediapipe::OkStatus();
    }
    // close(sockfd);
    return ::mediapipe::OkStatus();
  }
}
 //end namespace