#include "catch.hpp"
#include "cameraConfigs.hpp"
#include <vector>
#include <gflags/gflags.h>
#include <array>
#include <iostream>

using std::cout;
using std::vector;

// Used in cameraConfigs.cpp
DECLARE_string(cameraConfigsPath);

TEST_CASE(){
    FLAGS_cameraConfigsPath = "src/test/testCameraConfigs/config.json";
    const vector<cameraConfig>  tmp = getCameraConfigs();    
    string correctFilePrefix = "0";
    vector<vector<float>> correctInverseProjectionMat = {
        {0.363970249891281,0,0,0},
        {0,0.363970249891281,0,0},
        {0,0,0,-1},
        {0,0,-1.61666655540466,1.71666657924652},
    };
    vector<vector<float>> correctLocalToWorldMat = {
        {0.363970249891281,0,0,0},
        {0,0.363970249891281,0,0},
        {0,0,0,-1},
        {0,0,-1.61666655540466,1.71666657924652},
    };





    REQUIRE(transformMatrix4f(correctInverseProjectionMat) == tmp[0].inverseProjectionMatrix);
    REQUIRE(transformMatrix4f(correctLocalToWorldMat) == tmp[0].localCoordinatestoWorldCoordinatesMatrix);
    REQUIRE(correctFilePrefix == tmp[0].filePrefix);   
}