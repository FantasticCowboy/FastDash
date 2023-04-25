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
#include "estimateKeypoints.hpp"

using std::array;
using std::vector;


DEFINE_string(encodingFilesLocation, "./testFiles/testCameraConfigsAndFrames/", "Where the files are stored on disk");
DEFINE_double(farClipPlaneDistance, 10, "Farclip plane of the camera");
DEFINE_int32(numFrames, 1, "Number of frames to read from disk");
DEFINE_bool(writeFramesToDisk, false, "Whethere or not to write the frames to disk");
DEFINE_int32(frameHeight,-1, "Height of frames to be read");
DEFINE_int32(frameWidth,-1, "Width of frames to be read");
DEFINE_bool(transformFrames, false, "Whether or not to transform the frames into pointclouds");

int main(int argc, char *argv[]){
    std::cout << "Starting program..." << std::endl;
    gflags::ParseCommandLineFlags(&argc, &argv, true);   
    vector<cameraConfig> configs = getCameraConfigs();     
    vector<videoReader> videoReaders;
    vector<screenToWorldTransformation> transforms;

    // Set up classes for reading in frames and transforming pixels
    for(int i = 0; i < configs.size(); i++){
        videoReaders.push_back(videoReader(configs[i].filePrefix, FLAGS_frameHeight, FLAGS_frameWidth, FLAGS_encodingFilesLocation ));
        transforms.push_back(screenToWorldTransformation(configs[i].inverseProjectionMatrix, FLAGS_farClipPlaneDistance, configs[i].localCoordinatestoWorldCoordinatesMatrix));
    }

    // Initallize data structure for storing point cloud frames
    vector<vector<array<float,3>>> pointsFrame;

    // If flag is enabled, iterate through each depth frame, convert to point cloud and store in pointFrames
    if(FLAGS_transformFrames){
        std::cout << "Transforming Frames..." << std::endl;
        for(int i = 0; i < FLAGS_numFrames; i++){
            pointsFrame.emplace_back();

            // Convert all of the pixels to 3d space
            for(int j = 0; j< videoReaders.size(); j++){
                transforms[j].transformFrame(videoReaders[j].getNextFrame(), pointsFrame[i]);
            }
        }
    }

    // Calculate the keypoints for each person posing and store in pointFrames
    std::cout << "Calculating Keypoints..." << std::endl;
    for(int i = 0; i < FLAGS_numFrames; i++){
        pointsFrame.emplace_back();
        for(int j = 0; j< videoReaders.size(); j++){
            vector<vector<float>> frame = videoReaders[j].getNextFrame();
            auto data = estimateKeypoints(frame);

            // Iterate through each keypoint and add to point frame file
            for(int k = 0; k < data->poseKeypoints.getSize(0); k+=3 ){
                int xCoord = data->poseKeypoints.at(k);
                int yCoord = data->poseKeypoints.at(k+1);
                array<float,3> point =  transforms[j].transformPixel(xCoord, yCoord, frame[xCoord][yCoord], 1);
                pointsFrame[i].push_back(point);
            }
        }
    }

    // Write Point frames to disk
    if(FLAGS_writeFramesToDisk){
        std::cout << "Writing Frames..." << std::endl;
        for(int i = 0; i < pointsFrame.size(); i++){
            writeFrame("./build/out_" + std::to_string(i) + ".pointCloud", pointsFrame[i]);
        }
    }    

    std::cout << "Exiting program..." << std::endl;
}

