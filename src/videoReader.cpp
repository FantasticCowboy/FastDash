#include "videoReader.hpp"
#include "readDepthFrame.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

using std::string;
using std::vector;
using std::string;

videoReader::videoReader(std::string prefix_in, int h, int w){
    prefix = prefix_in;
    cur_frame = 0;
    height = h;
    width = w;
}

string videoReader::getFileName(){
    return prefix + "_" + std::to_string(cur_frame);
}

bool videoReader::hasNextFrame(){
    return std::filesystem::exists(getFileName());
}

// TODO: All of the vector copying here does a deep copy
// This will likely cause things to be slower than they need to be
// Honestly it is probably making it 2x slow
vector<vector<float>> videoReader::getNextFrame(){
    vector<vector<float>> ret;
    if(!std::filesystem::exists(getFileName())){
        throw std::runtime_error("No next file exists!" + getFileName());
    }
    if(prevFrame.empty()){
        prevFrame = readDepthFrame(width, height, getFileName());
        // deep copy all good
        ret = prevFrame;
    }else{
        ret = reconstructFrame(prevFrame, readDepthEncoding(getFileName()));
        prevFrame = ret;
    }

    cur_frame+=1;

    std::reverse(ret.begin(), ret.end());

    return ret;
}