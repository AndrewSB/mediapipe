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
#include "mediapipe/framework/port/logging.h"
#include <iostream>
#include <vector>
//calculator CanvasCalculator
// cv::Mat contouring(cv::Mat frame_ip, int width, cv::Mat thresh, int mid, int right, cv::Mat pattern_img, int radius);

bool first_frame = true;
float prev_x = -1;
float prev_y = -1;
float curr_x = -1;
float curr_y = -1;
int radius = 20;
cv::Mat prev_frame;

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
    cc->Inputs().Tag("INPUT_VIDEO").Set<ImageFrame>();
    cc->Inputs().Tag("MY_LANDMARKS").Set<std::vector<mediapipe::NormalizedLandmarkList, std::allocator<mediapipe::NormalizedLandmarkList>>>();
    cc->Outputs().Tag("OUTPUT_VIDEO").Set<ImageFrame>();
    return ::mediapipe::OkStatus();
}

::mediapipe::Status CanvasCalculator::Open(CalculatorContext* cc) {
    return ::mediapipe::OkStatus();
}
::mediapipe::Status CanvasCalculator::Process(CalculatorContext* cc) {

    // read input frane and make input mat
    const auto& input_frame = cc->Inputs().Tag("INPUT_VIDEO").Get<ImageFrame>();
    cv::Mat input_mat = formats::MatView(&input_frame); 
    cv::Mat frame_final ;

    // Declare empty frames needed
    // cv::Mat frame, gray, pattern_img, frame_final;

    // std::unique_ptr<ImageFrame> output_frame(
    //     new ImageFrame(input_frame.Format(), input_frame.Width(),input_frame.Height()));
    // cv::Mat output_mat = formats::MatView(output_frame.get());
    
    // set width height for landmarks un normalization
    int image_width = input_mat.size().width;
    int image_height = input_mat.size().height;
    auto landmarks = cc->Inputs().Tag("MY_LANDMARKS").Get<std::vector<mediapipe::NormalizedLandmarkList, std::allocator<mediapipe::NormalizedLandmarkList> >>();
    
    LOG (WARNING)  << "------------------------------------" << std::endl;
    LOG (WARNING) << landmarks.size()  << std::endl; 
    // for (int i = 0; i < landmarks.landmark_size(); ++i) {
    //     // const NormalizedLandmark& landmark = landmarks.landmark(i);
    //     LOG (WARNING)  << "------------------------------------" << std::endl;
    //     LOG (WARNING) << landmarks[i].x() << landmarks[i].y() << landmark.z()  << std::endl; 
    // }

    // curr_x = landmarks.landmark(8).x() * image_width;
    // curr_y = landmarks.landmark(8).y() * image_height;

    // cv::Point a(prev_x, prev_y);
    // cv::Point b(curr_x, curr_y);
    // double res = cv::norm(a-b);//Euclidian distance

    // if(curr_x < 1 || res < 50 ) 
    // {
    //     curr_x = prev_x;
    //     curr_y = prev_y;
    // }

    // std::unique_ptr<ImageFrame> output_frame(
    //     new ImageFrame(input_frame.Format(), input_frame.Width(),input_frame.Height()));

    // cv::Mat output_mat = formats::MatView(output_frame.get());
    

    // if(first_frame == true) {
    //     prev_x = curr_x;
    //     prev_y = curr_y;
    //     frame_final = cv::Mat(image_height, image_width, CV_8UC3, cv::Scalar(255,255,255));
    //     prev_frame = frame_final;
    //     first_frame = false;
    // }   

    
    // LOG (WARNING) << "Distance " << res <<std::endl;
    // if(res<50)
    //     cv::circle(frame_final, cv::Point(prev_x, prev_y) , radius, cv::Scalar(0, 255, 0));
    //  else
    //     // cv::circle(frame_final, cv::Point(curr_x, curr_y) , radius, cv::Scalar(0, 255, 0));
    //     cv::line(prev_frame, cv::Point(prev_x, prev_y), cv::Point(curr_x, curr_y), cv::Scalar(0, 255, 0));

    // // std::cout << "image width " << image_width << "image height" << image_height <<  prev_x << "  " <<  prev_y << "  " << curr_x << "  " << curr_y <<  "  " <<  std::endl;
    // // cv::circle(frame_final, cv::Point(curr_x, curr_y) , radius, cv::Scalar(0, 255, 0));
    // // cv::line(prev_frame, cv::Point(prev_x, prev_y), cv::Point(curr_x, curr_y), cv::Scalar(0, 255, 0));

    // prev_x = curr_x;
    // prev_y = curr_y;

    // // cv::Mat temp_frame = prev_frame;
    // prev_frame.copyTo(output_mat);

    std::unique_ptr<ImageFrame> output_frame(
        new ImageFrame(input_frame.Format(), input_frame.Width(),input_frame.Height()));

    cc->Outputs().Tag("OUTPUT_VIDEO").Add(output_frame.release() , cc->InputTimestamp());
    return ::mediapipe::OkStatus();
// after defining calculator class, we need to register it with a macro invocation 
// REGISTER_CALCULATOR(calculator_class_name).
REGISTER_CALCULATOR(::mediapipe::CanvasCalculator);
}
}
 //end namespace