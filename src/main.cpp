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
#include <opencv2/opencv.hpp>
#include "clustering.hpp"

using std::array;
using std::vector;


DEFINE_string(encodingFilesLocation, "./testFiles/testCameraConfigsAndFrames/", "Where the files are stored on disk");
DEFINE_double(farClipPlaneDistance, 10, "Farclip plane of the camera");
DEFINE_int32(numFrames, 1, "Number of frames to read from disk");
DEFINE_bool(writeFramesToDisk, false, "Whethere or not to write the frames to disk");
DEFINE_int32(frameHeight,-1, "Height of frames to be read");
DEFINE_int32(frameWidth,-1, "Width of frames to be read");
DEFINE_bool(transformFrames, false, "Whether or not to transform the frames into pointclouds");
DEFINE_bool(calculateKeypoints, false, "Whether or not to calculate the keypoints for each frame and store in to a point cloud");
DEFINE_double(keypointThreshold, 0.5, "Confidence threshold that has to be met in order for fast dash to accept a keypoint");

int main(int argc, char *argv[]){
    std::cout << "Starting program..." << std::endl;
    gflags::ParseCommandLineFlags(&argc, &argv, true);  

    // TODO: maybe a vector of vectors or a class to manage all of this complexity 
    vector<cameraConfig> configs = getCameraConfigs();     
    vector<videoReader> depthVideoReaders;
    vector<videoReaderMP4> rgbVideoReaders;

    vector<screenToWorldTransformation> transforms;

    std::cout << "Starting Openposee..." << "\n";
    initializeWrapper();


    // Set up classes for reading in frames and transforming pixels
    for(int i = 0; i < configs.size(); i++){
        depthVideoReaders.push_back(videoReader(configs[i].filePrefix, FLAGS_frameHeight, FLAGS_frameWidth, FLAGS_encodingFilesLocation ));
        transforms.push_back(screenToWorldTransformation(configs[i].inverseProjectionMatrix, FLAGS_farClipPlaneDistance, configs[i].localCoordinatestoWorldCoordinatesMatrix));
    }

    // TODO: Make this from configs this is UGLY
    rgbVideoReaders.push_back(videoReaderMP4("/workspaces/FastDash/testFiles/endToEndTestFiles/test200x200/MP4/RGB Camera 0.mp4"));
    rgbVideoReaders.push_back(videoReaderMP4("/workspaces/FastDash/testFiles/endToEndTestFiles/test200x200/MP4/RGB Camera 1.mp4"));
    rgbVideoReaders.push_back(videoReaderMP4("/workspaces/FastDash/testFiles/endToEndTestFiles/test200x200/MP4/RGB Camera 2.mp4"));
    rgbVideoReaders.push_back(videoReaderMP4("/workspaces/FastDash/testFiles/endToEndTestFiles/test200x200/MP4/RGB Camera 3.mp4"));



    // Initallize data structure for storing point cloud frames
    vector<vector<array<float,3>>> pointFrame;
    vector<vector<cv::Point3f>> keyPoints;
    vector<vector<cv::Point3f>> kMeansCenters;




    // Calculate the keypoints for each person posing and store in pointFrames
    for(int i = 0; i < FLAGS_numFrames; i++){
        pointFrame.emplace_back();
        keyPoints.emplace_back();

        // Iterate through each camera and either calculate the keypoints or transform every pixel. Then stitch them together in to a single frame
        for(int j = 0; j< depthVideoReaders.size(); j++){
            vector<vector<float>> depthFrame = depthVideoReaders[j].getNextFrame();
            cv::Mat rgbFrame = rgbVideoReaders[j].getNextFrame();

            if(FLAGS_calculateKeypoints){
                std::cout << "Calculating Keypoints..." << std::endl;
                auto data = estimateKeypoints(depthFrame);
                debugKeypointDataImage(data, "./build/out_" + std::to_string(j) + "_" + std::to_string(i) + ".png");
                debugKeypointData(data, "./build/out_" + std::to_string(j) + "_" + std::to_string(i) + ".json");

                // Iterate through each keypoint and add to point frame file
                std::cout << "Array dimensions " << data->poseKeypoints.printSize() << std::endl;
                for(int k = 0; k < data->poseKeypoints.getVolume() ; k+=3 ){
                    if(data->poseKeypoints.at(k+2) < FLAGS_keypointThreshold){
                        continue;
                    }
                    int xCoord = 199 - data->poseKeypoints.at(k);
                    int yCoord = 199 - data->poseKeypoints.at(k+1);
                    array<float,3> point =  transforms[j].transformPixel(xCoord, yCoord, depthFrame[yCoord][xCoord], 1);
                    keyPoints[i].push_back(cv::Point3f(point[0], point[1], point[2]));                    
                }
            }

            if(FLAGS_transformFrames){
                std::cout << "Transforming Frame..." << std::endl;
                transforms[j].transformFrame(depthFrame, pointFrame[i]);                                
            }
        }

        if(FLAGS_calculateKeypoints){
            kMeansCenters.push_back(clusterPoints(keyPoints[i]));
        }
    }

    // Write Point frames to disk
    if(FLAGS_writeFramesToDisk){
        std::cout << "Writing Frames..." << std::endl;
        for(int i = 0; i < pointFrame.size(); i++){
            writeFrame("./build/out_" + std::to_string(i) + ".pointCloud", pointFrame[i]);
            writeFrame("./build/out_" + std::to_string(i) + ".keyPoints", keyPoints[i]);
            writeFrame("./build/out_" + std::to_string(i) + ".kmeansClusters", kMeansCenters[i]);
        }
    }    


    std::cout << "Shutdown Openpose..." << "\n";
    shutdownWrapper();


    std::cout << "Exiting program..." << std::endl;
}

