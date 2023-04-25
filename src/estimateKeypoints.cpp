#include "estimateKeypoints.hpp"
#include <opencv2/opencv.hpp>

cv::Mat convertVectorToMat(const std::vector<std::vector<float>> &frame){
    // cv::CV_32F
    int type = 5;
    // op::Matrix newFrame(frame.size(), frame[0].size(), type);
    cv::Mat f(frame.size(), frame[0].size(), type);

    int rowIndex = 0;
    for(auto row: frame){
        int colIdex = 0;
        for(auto val : row){
            f.at<float>(rowIndex, colIdex) = val;
            colIdex++;
        }
        rowIndex++;
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