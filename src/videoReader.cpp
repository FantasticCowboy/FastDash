#include "videoReader.hpp"
#include "readDepthFrame.hpp"
#include <string>
#include <vector>

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

vector<vector<float>> videoReader::getNextFrame(){
    vector<vector<float>> ret;
    if(prevFrame.empty()){
        prevFrame = readDepthFrame(width, height, getFileName());
        // deep copy all good
        ret = prevFrame;
    }else{
        
    }

    return vector<vector<float>>();
}