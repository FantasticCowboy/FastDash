#include "catch.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include "estimateKeypoints.hpp"


TEST_CASE("Test Keypoints from mp4"){
    cv::VideoCapture cap("testFiles/testKeypointsFromMP4/video.mp4");

    if (!cap.isOpened())
    {
        std::cerr << "Failed to open video file" << std::endl;
        return;
    }

    cv::Mat frame;
    
    
    op::ConfigureLog::setPriorityThreshold(op::Priority::High);
    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
    

    op::WrapperStructPose wrapperStructPose{};
    wrapperStructPose.modelFolder = "/openpose/models";
    wrapperStructPose.renderThreshold = .5;
    opWrapper.configure(wrapperStructPose);  
    opWrapper.start();

    std::cout << "Starting...\n";
    int frameCount = 0;
    while (cap.read(frame))
    {
        std::cout << "generating keypoint for frame " + std::to_string(frameCount) + "\r";
        std::cout.flush();
        auto datumsPtr = opWrapper.emplaceAndPop(OP_CV2OPMAT(frame));
        debugKeypointDataImage(datumsPtr->at(0), "./build/debug/mp4KeypointsTest_" + std::to_string(frameCount) + ".png");
        frameCount++;
    }    
    std::cout << "\n";

    opWrapper.stop();

}