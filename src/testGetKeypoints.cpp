#include "getKeypoints.hpp"
#include "catch.hpp"
#include <vector>
#include <string> 
#include <array>
#include <iostream>
#include "cameraConfigs.hpp"
#include "screenToWorldTransformation.hpp"
#include "writePointCloud.hpp"
#include "videoReader.hpp"


TEST_CASE("Test Get Keypoints"){
    std::vector<std::string> filepaths = {
        "testFiles/testKeypoints/0_0.json",
        "testFiles/testKeypoints/1_0.json",
        "testFiles/testKeypoints/2_0.json",
        "testFiles/testKeypoints/3_0.json"
    };
    std::vector<std::vector<std::array<float,2>>> keypoints = getKeypoints(filepaths);
    std::vector<cameraConfig> configs = getCameraConfigs();
    vector<videoReader> videoReaders;
    vector<screenToWorldTransformation> transforms;

    for(int i = 0; i < configs.size(); i++){
        videoReaders.push_back(videoReader(configs[i].filePrefix,1080 , 1920, "./testFiles/testKeypoints/" ));
        transforms.push_back(screenToWorldTransformation(configs[i].inverseProjectionMatrix, 10, configs[i].localCoordinatestoWorldCoordinatesMatrix));
    }


    vector<array<float,3>> pointFrame;
    int angle = 0;
    for(auto cameraAngle : keypoints){
        std::vector<std::vector<float>> distances = videoReaders[angle].getNextFrame();
        for(auto keypoint : cameraAngle){

            int xTruncated = int(keypoint[1]) ;
            int yTruncated = int(keypoint[0]) ;

            std::cout << 1080 - keypoint[1] << " " << 1920 - keypoint[0] << "\n";

            std::array<float, 3UL> tmp = transforms[angle].transformPixel(keypoint[0], 1920 - keypoint[1], distances[xTruncated][1920-yTruncated], 1);
            
            float z = tmp[2];

            tmp[2] = tmp[0];
            tmp[0] = z;

            pointFrame.push_back(tmp);
        }
        angle++;
    }

    writeFrame("./build/out_test_get_keypoints.pointCloud", pointFrame);    
}