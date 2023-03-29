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


static int height = 200;
static int width = 200;
static int numFrames = 2;
DEFINE_string(encodingFilesLocation, "./testFiles/", "Where the files are stored on disk");
DEFINE_double(farClipPlaneDistance, 10, "Farclip plane of the camera");
DEFINE_int32(numFrames, 24, "Number of frames to read from disk");


int main(int argc, char *argv[]){
    std::cout << "Starting program..." << std::endl;
    gflags::ParseCommandLineFlags(&argc, &argv, true);   
    vector<cameraConfig> configs = getCameraConfigs();     
    vector<videoReader> videoReaders;
    vector<screenToWorldTransformation> transforms;

    for(int i = 0; i < configs.size(); i++){
        videoReaders.push_back(videoReader(FLAGS_encodingFilesLocation + configs[i].filePrefix, height, width));
        transforms.push_back(screenToWorldTransformation(configs[i].inverseProjectionMatrix, FLAGS_farClipPlaneDistance, configs[i].localCoordinatestoWorldCoordinatesMatrix));
    }

    vector<vector<array<float,3>>> pointsFrames;
    std::cout << "Transforming Frames..." << std::endl;
    for(int i = 0; i < numFrames; i++){
        pointsFrames.emplace_back();
        for(int i = 0; i < videoReaders.size(); i++){
            transforms[i].transformFrame(videoReaders[i].getNextFrame(), pointsFrames[0]);
        }
    }

    writeFrame("./out", pointsFrames[0]);

    std::cout << "Exiting program..." << std::endl;
}