#include "readDepthFrame.hpp"
#include <iostream>
#include <fstream>

using std::cout;


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
    cout << "Reading Depth Encoding...\n";    
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

std::vector<std::vector<float>> readDepthFrame(int width, int height, std::string fileName){

}