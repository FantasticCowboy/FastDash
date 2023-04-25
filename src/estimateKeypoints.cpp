#include "estimateKeypoints.hpp"
#include <opencv2/opencv.hpp>



// NOTE: I determined that in order to get the unity coordinate system in line with the openpose coordinate system, you need to 
// Reflect over the Y and X axis. To determine this I compared the images saved to disk by unity to what unity was actually showing.
// What unity actually shows is normal but what  is saved to the y and x axis is incorrect since it is reflected. Thius tells me that
// when interpreting the data raw without any internal unity transformation the y anx x axis need to be reflected over the center of the image


// Reflects vector over x and y axis and mputs that into a cv matrix
// See above note for an explanation
cv::Mat convertVectorToMat(const std::vector<std::vector<float>> &frame){
    // cv::CV_32F
    int type = 5;
    // op::Matrix newFrame(frame.size(), frame[0].size(), type);
    cv::Mat f(frame.size(), frame[0].size(), type);

    int rowIndex = 199;
    for(auto row: frame){
        int colIdex = 199;
        for(auto val : row){
            f.at<float>(rowIndex,colIdex) = val;
            colIdex--;
        }
        rowIndex--;
    }

    cv::Mat output0;

    normalize(f, output0, 0, 255, cv::NORM_MINMAX, CV_8U);

    cv::Mat output;
    cv::cvtColor(output0, output, cv::COLOR_GRAY2BGR );
    return output;
}

std::shared_ptr<op::Datum> estimateKeypoints(const std::vector<std::vector<float>> &frame){
    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
    op::WrapperStructPose wrapperStructPose{};
    wrapperStructPose.modelFolder = "/openpose/models";
    opWrapper.configure(wrapperStructPose);  
    opWrapper.start();
    cv::Mat tmp = (convertVectorToMat(frame));
    auto ret = opWrapper.emplaceAndPop(OP_CV2OPMAT(tmp));
    opWrapper.stop();

    return ret->at(0);
}

void debugKeypointData(std::shared_ptr<op::Datum>  &data, const std::string &filepath){
    op::savePeopleJson(
        data->poseKeypoints,
        data->poseCandidates,
        "poseKeypoints",
        filepath,
        true
    );
}


void debugKeypointDataImage(std::shared_ptr<op::Datum>  &data, const std::string &filepath){
    op::saveImage(data->cvOutputData, filepath);
}