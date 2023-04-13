#include <iostream>
#include "videoReader.hpp"
#include "screenToWorldTransformation.hpp"
#include <string>
#include <array>
#include <vector>
#include <gflags/gflags.h>
#include <gflags/gflags_completions.h>
#include "cameraConfigs.hpp"
#include "writePointCloud.hpp"

using std::array;
using std::vector;


DEFINE_string(encodingFilesLocation, "./testFiles/largeTestFiles/", "Where the files are stored on disk");
DEFINE_double(farClipPlaneDistance, 10, "Farclip plane of the camera");
DEFINE_int32(numFrames, 1, "Number of frames to read from disk");
DEFINE_bool(writeFramesToDisk, false, "Whethere or not to write the frames to disk");
DEFINE_int32(frameHeight,-1, "Height of frames to be read");
DEFINE_int32(frameWidth,-1, "Width of frames to be read");

int main(int argc, char *argv[]){
    std::cout << "Starting program..." << std::endl;
gflags::ParseCommandLineFlags(&argc, &argv, true);   
    vector<cameraConfig> configs = getCameraConfigs();     
    vector<videoReader> videoReaders;
    vector<screenToWorldTransformation> transforms;

    for(int i = 0; i < configs.size(); i++){
        videoReaders.push_back(videoReader(configs[i].filePrefix, FLAGS_frameHeight, FLAGS_frameWidth, FLAGS_encodingFilesLocation ));
        transforms.push_back(screenToWorldTransformation(configs[i].inverseProjectionMatrix, FLAGS_farClipPlaneDistance, configs[i].localCoordinatestoWorldCoordinatesMatrix));
    }

    vector<vector<array<float,3>>> pointsFrames;
    std::cout << "Transforming Frames..." << std::endl;
    for(int i = 0; i < FLAGS_numFrames; i++){
        pointsFrames.emplace_back();
        for(int j = 0; j< videoReaders.size(); j++){
            transforms[j].transformFrame(videoReaders[j].getNextFrame(), pointsFrames[i]);
        }
    }


    if(FLAGS_writeFramesToDisk){
        std::cout << "Writing Frames..." << std::endl;
        for(int i = 0; i < pointsFrames.size(); i++){
            writeFrame("./build/out_" + std::to_string(i) + ".pointCloud", pointsFrames[i]);
        }
    }
    std::cout << "Exiting program..." << std::endl;
}