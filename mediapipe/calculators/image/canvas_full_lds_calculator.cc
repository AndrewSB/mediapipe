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
// #include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/hyper_full.pb.h"
#include "mediapipe/framework/port/logging.h"
#include <iostream>
#include <vector>

namespace mediapipe
{

    class CanvasFullLdsCalculator : public CalculatorBase
    {

    public:
        CanvasFullLdsCalculator() = default;
        ~CanvasFullLdsCalculator() override = default;
        static ::mediapipe::Status GetContract(CalculatorContract *cc);
        ::mediapipe::Status Open(CalculatorContext *cc) override;
        ::mediapipe::Status Process(CalculatorContext *cc) override;
    };

    REGISTER_CALCULATOR(::mediapipe::CanvasFullLdsCalculator);

    ::mediapipe::Status CanvasFullLdsCalculator::GetContract(CalculatorContract *cc)
    {
        cc->Inputs().Tag("POSE_LANDMARKS").Set<NormalizedLandmarkList>();
        cc->Inputs().Tag("LEFT_HAND_LANDMARKS").Set<NormalizedLandmarkList>();
        cc->Inputs().Tag("RIGHT_HAND_LANDMARKS").Set<NormalizedLandmarkList>();
        cc->Outputs().Tag("HYPER_OUT").Set<HyperFullLandmarks>();
        return ::mediapipe::OkStatus();
    }

    ::mediapipe::Status CanvasFullLdsCalculator::Open(CalculatorContext *cc)
    {
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status CanvasFullLdsCalculator::Process(CalculatorContext *cc)
    {

        HyperFullLandmarks *hyper_out_lds = new HyperFullLandmarks();
        hyper_out_lds->InitAsDefaultInstance();

        if (!cc->Inputs().Tag("POSE_LANDMARKS").IsEmpty())
        {
            auto poselandmarklist = cc->Inputs().Tag("POSE_LANDMARKS").Get<NormalizedLandmarkList>();

            for (int i = 0; i < poselandmarklist.landmark_size(); ++i)
            {
                const NormalizedLandmark &landmark = poselandmarklist.landmark(i);
                // LOG (WARNING)  << "-------------7-----------------------" << std::endl;

                hyper_out_lds->mutable_pose_lds()->add_landmark();
                int size = hyper_out_lds->mutable_pose_lds()->landmark_size() - 1;
                LOG (WARNING)  << "-------------POSE -----------------------" << size << " " << landmark.x() << " " << landmark.y() << " " << landmark.z() << std::endl;

                if (landmark.has_x())
                {
                    hyper_out_lds->mutable_pose_lds()->mutable_landmark(size)->set_x(landmark.x());
                    // LOG (WARNING)  << "-------------9-----------------------" << std::endl;
                }
                if (landmark.has_y())
                {
                    hyper_out_lds->mutable_pose_lds()->mutable_landmark(size)->set_y(landmark.y());
                    // LOG (WARNING)  << "-------------10-----------------------" << std::endl;
                }
                if (landmark.has_z())
                {
                    hyper_out_lds->mutable_pose_lds()->mutable_landmark(size)->set_z(landmark.z());
                    // LOG (WARNING)  << "-------------11-----------------------" << std::endl;
                }
                if (landmark.has_visibility())
                {
                    hyper_out_lds->mutable_pose_lds()->mutable_landmark(size)->set_visibility(landmark.visibility());
                    // LOG (WARNING)  << "-------------12-----------------------" << std::endl;
                }
                if (landmark.has_presence())
                {
                    hyper_out_lds->mutable_pose_lds()->mutable_landmark(size)->set_presence(landmark.presence());
                    // LOG (WARNING)  << "-------------13-----------------------" << std::endl;
                }
            }
        }

        if (!cc->Inputs().Tag("LEFT_HAND_LANDMARKS").IsEmpty())
        {
            auto leftlandmarklist = cc->Inputs().Tag("LEFT_HAND_LANDMARKS").Get<NormalizedLandmarkList>();
            for (int i = 0; i < leftlandmarklist.landmark_size(); ++i)
            {
                const NormalizedLandmark &landmark = leftlandmarklist.landmark(i);
                // LOG (WARNING)  << "-------------7-----------------------" << std::endl;

                hyper_out_lds->mutable_lhand_lds()->add_landmark();
                int size = hyper_out_lds->mutable_lhand_lds()->landmark_size() - 1;
                LOG (WARNING)  << "-------------LEFT -----------------------" << size << " " << landmark.x() << " " << landmark.y() << " " << landmark.z() << std::endl;

                if (landmark.has_x())
                {
                    hyper_out_lds->mutable_lhand_lds()->mutable_landmark(size)->set_x(landmark.x());
                    // LOG (WARNING)  << "-------------9-----------------------" << std::endl;
                }
                if (landmark.has_y())
                {
                    hyper_out_lds->mutable_lhand_lds()->mutable_landmark(size)->set_y(landmark.y());
                    // LOG (WARNING)  << "-------------10-----------------------" << std::endl;
                }
                if (landmark.has_z())
                {
                    hyper_out_lds->mutable_lhand_lds()->mutable_landmark(size)->set_z(landmark.z());
                    // LOG (WARNING)  << "-------------11-----------------------" << std::endl;
                }
                if (landmark.has_visibility())
                {
                    hyper_out_lds->mutable_lhand_lds()->mutable_landmark(size)->set_visibility(landmark.visibility());
                    // LOG (WARNING)  << "-------------12-----------------------" << std::endl;
                }
                if (landmark.has_presence())
                {
                    hyper_out_lds->mutable_lhand_lds()->mutable_landmark(size)->set_presence(landmark.presence());
                    // LOG (WARNING)  << "-------------13-----------------------" << std::endl;
                }
            }
        }

        if (!cc->Inputs().Tag("RIGHT_HAND_LANDMARKS").IsEmpty())
        {
            auto rightlandmarklist = cc->Inputs().Tag("RIGHT_HAND_LANDMARKS").Get<NormalizedLandmarkList>();
            for (int i = 0; i < rightlandmarklist.landmark_size(); ++i)
            {
                const NormalizedLandmark &landmark = rightlandmarklist.landmark(i);
                // LOG (WARNING)  << "-------------7-----------------------" << std::endl;

                hyper_out_lds->mutable_rhand_lds()->add_landmark();
                int size = hyper_out_lds->mutable_rhand_lds()->landmark_size() - 1;
                LOG (WARNING)  << "-------------RIGHT -----------------------" << size << " " << landmark.x() << " " << landmark.y() << " " << landmark.z() << std::endl;


                if (landmark.has_x())
                {
                    hyper_out_lds->mutable_rhand_lds()->mutable_landmark(size)->set_x(landmark.x());
                    // LOG (WARNING)  << "-------------9-----------------------" << std::endl;
                }
                if (landmark.has_y())
                {
                    hyper_out_lds->mutable_rhand_lds()->mutable_landmark(size)->set_y(landmark.y());
                    // LOG (WARNING)  << "-------------10-----------------------" << std::endl;
                }
                if (landmark.has_z())
                {
                    hyper_out_lds->mutable_rhand_lds()->mutable_landmark(size)->set_z(landmark.z());
                    // LOG (WARNING)  << "-------------11-----------------------" << std::endl;
                }
                if (landmark.has_visibility())
                {
                    hyper_out_lds->mutable_rhand_lds()->mutable_landmark(size)->set_visibility(landmark.visibility());
                    // LOG (WARNING)  << "-------------12-----------------------" << std::endl;
                }
                if (landmark.has_presence())
                {
                    hyper_out_lds->mutable_rhand_lds()->mutable_landmark(size)->set_presence(landmark.presence());
                    // LOG (WARNING)  << "-------------13-----------------------" << std::endl;
                }
            }
        }

        // for (int i = 0; i < poselandmarklist.landmark_size(); ++i) {
        //     const NormalizedLandmark& landmark = poselandmarklist.landmark(i);
        //     LOG (WARNING)  << "--------------POSE LANDMARK POINT----------------------" << std::endl;
        //     LOG (WARNING) << landmark.x() << landmark.y() << landmark.z()  << std::endl;
        // }

        // for (int i = 0; i < leftlandmarklist.landmark_size(); ++i) {
        //     const NormalizedLandmark& landmark = leftlandmarklist.landmark(i);
        //     LOG (WARNING)  << "--------------LEFT LANDMARK POINT----------------------" << std::endl;
        //     LOG (WARNING) << landmark.x() << landmark.y() << landmark.z()  << std::endl;
        // }

        // for (int i = 0; i < rightlandmarklist.landmark_size(); ++i) {
        //     const NormalizedLandmark& landmark = rightlandmarklist.landmark(i);
        //     LOG (WARNING)  << "--------------RIGHT LANDMARK POINT----------------------" << std::endl;
        //     LOG (WARNING) << landmark.x() << landmark.y() << landmark.z()  << std::endl;
        // }

        std::unique_ptr<mediapipe::HyperFullLandmarks> output_stream_collection = std::make_unique<mediapipe::HyperFullLandmarks>(*hyper_out_lds);
        LOG(WARNING) << "-------------21-----------------------" << std::endl;

        cc->Outputs().Tag("HYPER_OUT").Add(output_stream_collection.release(), cc->InputTimestamp());
        LOG(WARNING) << "-------------22-----------------------" << std::endl;

        return ::mediapipe::OkStatus();
        // after defining calculator class, we need to register it with a macro invocation
        // REGISTER_CALCULATOR(calculator_class_name).
    }

    ::mediapipe::Status Close(CalculatorContext *cc)
    {
        if (!cc->GraphStatus().ok())
        {
            return ::mediapipe::OkStatus();
        }
        // close(sockfd);
        return ::mediapipe::OkStatus();
    }
}
//end namespace