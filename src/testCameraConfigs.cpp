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

TEST_CASE("Test Read Camera Configs"){
    FLAGS_cameraConfigsPath = "./testFiles/testCameraConfigs/cameraConfigs.json";
    const vector<cameraConfig>  configs = getCameraConfigs();    
    string correctFilePrefix = "0";
    vector<vector<float>> correctInverseProjectionMat = {
        {0.363970249891281,0,0,0},
        {0,0.363970249891281,0,0},
        {0,0,0,-1},
        {0,0,-1.61666655540466,1.71666657924652},
    };
    vector<vector<float>> correctLocalToWorldMat = {
        {1,0,0,0},
        {0,1,0,2},
        {0,0,1,-5},
        {0,0,0,1},
    };


    REQUIRE(transformMatrix4f(correctInverseProjectionMat) == configs[0].inverseProjectionMatrix);
    REQUIRE(transformMatrix4f(correctLocalToWorldMat) == configs[0].localCoordinatestoWorldCoordinatesMatrix);
    REQUIRE(correctFilePrefix == configs[0].filePrefix);   


    correctFilePrefix = "2";
    correctInverseProjectionMat = {
        {0.363970249891281,0,0,0},
        {0,0.363970249891281,0,0},
        {0,0,0,-1},
        {0,0,-1.61666655540466,1.71666657924652},
    };
    correctLocalToWorldMat = {
        {-1.19209289550781E-07,0,1.00000011920929,-5},
        {0,1,0,2},
        {-1.00000011920929,0,-1.19209289550781E-07,0},
        {0,0,0,1},
    };

    REQUIRE(correctFilePrefix == configs[2].filePrefix);       
    REQUIRE(transformMatrix4f(correctInverseProjectionMat) == configs[2].inverseProjectionMatrix);
    REQUIRE(transformMatrix4f(correctLocalToWorldMat) == configs[2].localCoordinatestoWorldCoordinatesMatrix);

}