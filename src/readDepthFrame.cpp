#include "readDepthFrame.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
using std::cout;
using std::vector;


// DESCRIPTION: Opens file and returns the correspond fstream. 
// makes sure that the file can be opened
// TODO: better error handling - nuking the whole program seems like bad practice in this case
// since the error should be handleable
std::fstream openFile(std::string fileName){
    std::fstream fs;
    fs.open(fileName, std::fstream::in | std::fstream::binary);
    if(!fs.good()){
        cout << "Failed to open " + fileName + "\n";
        exit(1);
    }
    return fs;
}

// TODO: need to look into preallocating the vector memory as resizing the vector dynamically
// may lead to performance issues
std::vector<DeltaEncodedPixel> readDepthEncoding(std::string fileName){
    std::vector<DeltaEncodedPixel> pixels;
    
    std::fstream fs = openFile(fileName);

    while(!fs.eof()){
        DeltaEncodedPixel px;
        char* ptr = reinterpret_cast<char*>(&px);
        fs.read(ptr, sizeof(DeltaEncodedPixel));
        pixels.push_back(px);
    }
    
    return pixels;
}



// Used for reading in pixels below
struct RGBAPixel{
    float r;
    float g;
    float b;
    float a;
};
// TODO: determine the ordering of the depth frame on disk
// The image is upside down - when read sequentially you get
// bottom left to top right going across the rows
// May just want to leave as is and update later as reading the depth encoding
// will likely be in the same coordinate system
std::vector<std::vector<float>> readDepthFrame(int width, int height, std::string fileName){
    std::vector<std::vector<float>> frame(width, std::vector<float>(height));
    std::fstream fs = openFile(fileName);
    for(int j = 0; j < height ; j++){
        for(int i = 0; i < width; i++){
            RGBAPixel depth;
            char* ptr = reinterpret_cast<char*>(&depth);
            fs.read(ptr, sizeof(depth));
            frame[j][i] = depth.r;
        }
    }
    return frame;
}

// TODO: may not need to make deep copies here+
std::vector<std::vector<float>> reconstructFrame(
    std::vector<std::vector<float>> prevFrame, std::vector<DeltaEncodedPixel> diff){
    std::vector<std::vector<float>> nextFrame = prevFrame;

    for(auto px : diff){
        nextFrame[px.yPos][px.xPos] += px.diff;
    }
    return nextFrame;
}