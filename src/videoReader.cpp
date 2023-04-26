#include "videoReader.hpp"
#include "readDepthFrame.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <gflags/gflags.h>
#include <writePng.hpp>
using std::string;
using std::vector;
using std::string;


DEFINE_bool(Debug_read_in_frames, false, "Whether or not to write the read in frames to disk as a png. Used to verfiy that they are being read in correctly.");

videoReader::videoReader(std::string prefix_in, int h, int w, string fileDirectory_in){
    prefix = prefix_in;
    fileDirectory = fileDirectory_in;
    cur_frame = 0;
    height = h;
    width = w;
}

string videoReader::getFileName(){
    return prefix + "_" + std::to_string(cur_frame);    
}

string videoReader::getFilePath(){
    return fileDirectory + prefix + "_" + std::to_string(cur_frame);
}

bool videoReader::hasNextFrame(){
    return std::filesystem::exists(getFilePath());
}

// TODO: All of the vector copying here does a deep copy
// This will likely cause things to be slower than they need to be
// Honestly it is probably making it 2x slow
vector<vector<float>> videoReader::getNextFrame(){
    vector<vector<float>> ret;
    if(!std::filesystem::exists(getFilePath())){
        throw std::runtime_error("No next file exists!" + getFilePath());
    }
    if(prevFrame.empty()){
        prevFrame = readDepthFrame(width, height, getFilePath());
        // deep copy all good
        ret = prevFrame;
    }else{
        std::vector<DeltaEncodedPixel> diff = readDepthEncoding(getFilePath());
        prevFrame = reconstructFrame(prevFrame, diff);
        ret = prevFrame;
    }

    if(FLAGS_Debug_read_in_frames){
        writePng(ret, "./build/debug/" + getFileName() + ".png");
    }

    cur_frame+=1;
    return ret;
}

cv::Mat videoReaderMP4::getNextFrame(){
    cv::Mat frame;
    cap.read(frame);    
    return frame;
}


videoReaderMP4::videoReaderMP4(std::string path){
    cap = cv::VideoCapture(path);

}

// TODO: What the hell is this code - fix it ?????!?!?!@?@#!?!@#?
bool videoReaderMP4::hasNextFrame(){
    return true;
    
}