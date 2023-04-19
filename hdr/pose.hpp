#pragma once

#include <opencv2/opencv.hpp>
// Command-line user interface
// OpenPose dependencies
#include <openpose/headers.hpp>
#define OPENPOSE_FLAGS_DISABLE_PRODUCER
#define OPENPOSE_FLAGS_DISABLE_DISPLAY
#include <openpose/flags.hpp>

#include <iostream>

using std::cout;

int generateKeypoins(){
    cout << "generating keypoints...\n";


    // Set the logging level
    op::ConfigureLog::setPriorityThreshold(op::Priority::High);

    // Create an OpenPose wrapper
    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};

    cout << "Configuring model...\n";

    // Configure pose model
    op::WrapperStructPose wrapperStructPose{};
    wrapperStructPose.poseModel = op::PoseModel::BODY_25;
    opWrapper.configure(wrapperStructPose);  

    cout << "Starting...\n";

    opWrapper.start();
    // Read the image
    cout << "Reading in the image...\n";
    const auto image_path = "/workspaces/FastDash/testFiles/testKeypoints/0_0.png";
    const auto input_image = op::loadImage(image_path);
    
    // Process the image and get keypoints
    cout << "Process the image and get keypoints...\n";
    const auto pose_keypoints = opWrapper.emplaceAndPop(input_image);


    return 0;
}