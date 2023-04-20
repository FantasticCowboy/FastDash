#pragma once

#include <opencv2/opencv.hpp>
// Command-line user interface
// OpenPose dependencies
#include <openpose/headers.hpp>
#define OPENPOSE_FLAGS_DISABLE_PRODUCER
#define OPENPOSE_FLAGS_DISABLE_DISPLAY
#include <openpose/flags.hpp>

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
using std::cout;


class wrapperGenerators{
    private:
        std::vector<op::PoseModel> models = {op::PoseModel::COCO_18, op::PoseModel::BODY_25, op::PoseModel::MPI_15};
        std::vector<op::Point<int>> cnnResolutions = { op::Point(32,16), op::Point(320,160), op::Point(-1,368)};
        std::vector<int> scalesNumbers = {1,4,8};
        std::vector<float> scalesGaps = {.1,.5,.75};

        std::vector<int> currentlyInUse = {-1,-1,-1,-1};
        int curIndex = 0;
        int choices = 3;
        int numGenerated = 0;
        int timeToRun = 5;
        op::String pathToModels = "/openpose/models";
        std::string imagePath = "/workspaces/FastDash/testFiles/testKeypoints/0_0.png";            
    public:
        op::WrapperStructPose getCurrentWrapperSturctPose(){
            op::WrapperStructPose wrapperStructPose{};
            wrapperStructPose.modelFolder = pathToModels;
            wrapperStructPose.numberPeopleMax = 1;

            wrapperStructPose.poseModel = models[currentlyInUse[0]];
            wrapperStructPose.netInputSize = cnnResolutions[currentlyInUse[1]];
            wrapperStructPose.scalesNumber = scalesNumbers[currentlyInUse[2]];
            wrapperStructPose.scaleGap = scalesGaps[currentlyInUse[3]];
        }

        void runTest(){
            op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
            opWrapper.configure(getCurrentWrapperSturctPose());            
            const auto input_image = op::loadImage(imagePath, cv::IMREAD_COLOR);    
            for(int i = 0; i < timeToRun ; i++){
                opWrapper.emplaceAndPop(input_image);
            }
            opWrapper.stop();
        }

        void printCurrentlyInUse(){
            cout << "[ ";
            for(auto num : currentlyInUse){
                cout  << num << ", ";
            }
            cout << "]\n";
            numGenerated ++;
        }

        void exploreCombinations(){   
            for(int i = 0; i < choices; i++){
                currentlyInUse[curIndex]++;
                if(curIndex != currentlyInUse.size() -1){
                    curIndex++;
                    exploreCombinations();                
                    curIndex--;            
                }else{
                    runTest();
                }
            }            
            currentlyInUse[curIndex] = 0;
            cout << "Generated " << numGenerated << "\n";
        }


};

int testWraperGenerators(){
    wrapperGenerators gen;
    gen.exploreCombinations();
}

int generateKeypoins(){
    cout << "generating keypoints...\n";
    // Set the logging level
    // op::ConfigureLog::setPriorityThreshold(op::Priority::High);

    // Create an OpenPose wrapper
    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
    cout << "Configuring model...\n";

    
    

    // Configure pose model
    op::WrapperStructPose wrapperStructPose{};
    wrapperStructPose.poseModel = op::PoseModel::COCO_18;
    wrapperStructPose.modelFolder = "/openpose/models";
    wrapperStructPose.numberPeopleMax = 1;
    wrapperStructPose.netInputSize = op::Point(32,16);
    // wrapperStructPose.caffeModelPath = 
    wrapperStructPose.scalesNumber = 1;
    wrapperStructPose.scaleGap = 0.25;
    opWrapper.configure(wrapperStructPose);  
    

    // wrapperStructPose.netInputSize = op::Point(16,16);

    cout << "Starting...\n";

    opWrapper.start();
    // Read the image
    cout << "Reading in the image...\n";
    const auto image_path = "/workspaces/FastDash/testFiles/testKeypoints/0_0.png";
    const auto input_image = op::loadImage(image_path, cv::IMREAD_COLOR);
    
    
    // Process the image and get keypoints
    cout << "Process the image and get keypoints...\n";
    auto start = std::chrono::system_clock::now();
    int times_run = 1000;
     
    std::string loading_bar = "";
    for(int i = 0; i < times_run ; i++){
        cout << "Loading...";
        if(i % 100 == 0){
            loading_bar.push_back('#');
        }
        cout << loading_bar << "\r";
        cout.flush();
        opWrapper.emplaceAndPop(input_image);
    }
    cout << "\n";

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start
        );

    std::cout << "time to process " << delta.count() << "ms ...\n";
    std::cout << "avg time to process " << delta.count() / (1.0 * times_run) << "ms ...\n";


    return 0;
}