#include <iostream>
#include "videoReader.hpp"
#include "screenToWorldTransformation.hpp"
#include <string>
#include <array>
#include <vector>
#include <gflags/gflags.h>
#include <gflags/gflags_completions.h>
#include "cameraConfigs.hpp"


using std::array;
using std::vector;


static float farClipPlaneDistance = 10;
static int height = 1080;
static int width = 1920;
static int numFrames = 24;
string encodingFilesLocation = "./test/testDeltaEncodedFiles/delta_encoding_raw_data/";
string uid0 = "0";
string uid1 = "1";
string uid2 = "2";
string uid3 = "3";

int main(int argc, char *argv[]){
    std::cout << "Starting program..." << std::endl;
    gflags::ParseCommandLineFlags(&argc, &argv, true);   

    vector<cameraConfig> configs = getCameraConfigs();     
    vector<videoReader> videoReaders;
    vector<screenToWorldTransformation> transforms;


    for(auto config: configs){
        videoReaders.emplace_back(encodingFilesLocation + config.filePrefix, height, width);
        transforms.emplace_back(config.inverseProjectionMatrix, farClipPlaneDistance, config.localCoordinatestoWorldCoordinatesMatrix);
    }


    vector<vector<array<float,3>>> pointsFrames;
    
    for(int i = 0; i < numFrames; i++){
        vector<array<float,3>> points;
        for(int i = 0; i < videoReaders.size(); i++){
            transforms[i].transformFrame(videoReaders[i].getNextFrame(),points);
        }
        pointsFrames.push_back(points);        
    }  

    std::cout << "Exiting program..." << std::endl;
}