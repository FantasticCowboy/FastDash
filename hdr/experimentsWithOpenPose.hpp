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
#include <fstream>

using std::cout;


class wrapperGenerators{
    private:
        std::vector<op::PoseModel> models = {op::PoseModel::COCO_18, op::PoseModel::BODY_25, op::PoseModel::MPI_15};
        std::vector<op::Point<int>> cnnResolutions = { op::Point(16,16), op::Point(32, 16), op::Point(32,32)};
        std::vector<int> scalesNumbers = {1,2,2};
        std::vector<float> scalesGaps = {0.25,0.5,0.75};
        std::vector<int> currentlyInUse = {-1,-1,-1,-1};
        int curIndex = 0;
        int choices = 3;
        int numGenerated = 0;
        int timeToRun = 15;
        op::String pathToModels = "/openpose/models";
        std::string imagePath = "/workspaces/FastDash/testFiles/testOpenPose/input/2_0.png";       

        std::string outputFilePath = "/workspaces/FastDash/testFiles/testOpenPose/results/results.txt";     
        std::fstream outStream ;
            op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};


        op::WrapperStructPose getCurrentWrapperSturctPose(){
            op::WrapperStructPose wrapperStructPose{};
            wrapperStructPose.modelFolder = pathToModels;
            wrapperStructPose.numberPeopleMax = 1;

            wrapperStructPose.poseModel = models[currentlyInUse[0]];
            wrapperStructPose.netInputSize = cnnResolutions[currentlyInUse[1]];
            wrapperStructPose.scalesNumber = scalesNumbers[currentlyInUse[2]];
            wrapperStructPose.scaleGap = scalesGaps[currentlyInUse[3]];



            return wrapperStructPose;
        }

        void runTest(){

            try{
                opWrapper.configure(getCurrentWrapperSturctPose()); 
                opWrapper.start();           
                const auto input_image = op::loadImage(imagePath, cv::IMREAD_COLOR); 
                cout << "Completed " + std::to_string(numGenerated) + "/" + std::to_string(81) + " tests, running " << getCurrentlyInUse() << "\r";
                cout.flush();

                auto start = std::chrono::system_clock::now();


                for(int i = 0; i < timeToRun ; i++){
                    opWrapper.emplaceAndPop(input_image);
                    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));

                }

                auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() - start
                );
                std::string result = std::to_string(
                        delta.count() / (1.0 * timeToRun)) + ", " + getCurrentlyInUse() + "\n"; 
                cout << "Wrote " << result << "\n";
                outStream.write(result.c_str(), result.size());
                outStream.flush();

                opWrapper.stop();
            }catch(std::exception e){
                cout << e.what() << "\n";
                if(opWrapper.isRunning()){
                    opWrapper.stop();
                }
            }

            numGenerated ++;
            
        }

        std::string getCurrentlyInUse(){
            std::string result = "";            
            for(auto num : currentlyInUse){
                result += std::to_string(num) + " ";
            }
            return result;
        }

        void exploreCombinations(){   
            for(int i = 0; i < choices; i++){
                //cout << "Incrementing Index: " << curIndex << "\n";
                currentlyInUse[curIndex]++;
                if(curIndex != currentlyInUse.size() -1){
                    curIndex++;
                    exploreCombinations();                
                    curIndex--;            
                }else{
                    //cout << "Running test for " << getCurrentlyInUse() << "\n";
                    cout.flush();
                    runTest();
                }
            }            
            //cout << "Erasing Index: " << curIndex << "\n";
            currentlyInUse[curIndex] = -1;
        }

    public:  
        void testDifferentOpenPoseSettings(){
            op::ConfigureLog::setPriorityThreshold(op::Priority::NoOutput);
            outStream = std::fstream(outputFilePath,  std::fstream::out | std::fstream::trunc);

            cout << "Beginning to test different OpenPose Settings...\n";
            exploreCombinations();
            cout << "Finished testing different OpenPose Settings...\n";

            outStream.close();
        }


};

void testWraperGenerators(){
    wrapperGenerators gen;
    gen.testDifferentOpenPoseSettings();
}

int generateKeypoins(){
    cout << "generating keypoints...\n";
    // Set the logging level
    op::ConfigureLog::setPriorityThreshold(op::Priority::High);

    // Create an OpenPose wrapper
    op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};

    
    

    // cout << "Configuring model...\n";
    // Configure pose model
    op::WrapperStructPose wrapperStructPose{};
    // wrapperStructPose.poseModel = op::PoseModel::BODY_25;
    wrapperStructPose.modelFolder = "/openpose/models";
    // wrapperStructPose.numberPeopleMax = 1;
    // wrapperStructPose.netInputSize = op::Point(32,16);
    // // wrapperStructPose.caffeModelPath = 
    // wrapperStructPose.scalesNumber = 1;
    // wrapperStructPose.scaleGap = 0.25;
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
    op::Datum tmp;
    cout << input_image.size(0) << "\n";
    auto datumsPtr = opWrapper.emplaceAndPop(input_image);
    

    op::opLog("Body keypoints: " + datumsPtr->at(0)->poseKeypoints.toString(), op::Priority::High);

    op::saveImage( 
        datumsPtr->at(0)->cvOutputData, 
        "/workspaces/FastDash/testFiles/testOpenPose/output/test.png"
    );
    op::savePeopleJson(
        datumsPtr->at(0)->poseKeypoints,
        datumsPtr->at(0)->poseCandidates ,
        "BodyPose",
        "/workspaces/FastDash/testFiles/testOpenPose/output/test.json",
        true
    );


    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start
        );

    std::cout << "time to process " << delta.count() << "ms ...\n";
    std::cout << "avg time to process " << delta.count() / (1.0 * times_run) << "ms ...\n";


    return 0;
}