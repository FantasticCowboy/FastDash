#pragma once
#include <string>
#include <vector> 
#include <opencv2/opencv.hpp>


using std::vector;
using std::string;


// TODO: make this an abstract reader class 

// DESCRIPTION: used to read frames from disk
class videoReader{
    private:
        vector<vector<float>> prevFrame;
        string prefix;
        string fileDirectory;
        int cur_frame = 0;     
        int height; 
        int width;
        string getFilePath();
        string getFileName();
    public:
        videoReader(std::string prefix, int height, int width, string fileDirectory_in);

        // DESCRIPTION: returns the next frame in the sequence if it exists
        // REQUIRES: next frame to be read exists
        // EFFECTS: returned value is the next frame in the sequence
        // THROWS: std::runtime_error if next file cannot be exists or does not exist
        vector<vector<float>> getNextFrame();

        // DESCRIPTION: checks to see if the next frame can be read
        bool hasNextFrame();
};


// DESCRIPTION: used to read frames from disk
class videoReaderMP4{
    private:
        cv::VideoCapture cap;
    public:
        videoReaderMP4(std::string path);

        // DESCRIPTION: returns the next frame in the sequence if it exists
        // REQUIRES: next frame to be read exists
        // EFFECTS: returned value is the next frame in the sequence
        // THROWS: std::runtime_error if next file cannot be exists or does not exist
        cv::Mat getNextFrame();

        // DESCRIPTION: checks to see if the next frame can be read
        bool hasNextFrame();
};