#include "writePointCloud.hpp"
#include <fstream>
#include <iostream>


using std::cout;

// DESCRIPTION: Opens file and returns the correspond fstream. 
// makes sure that the file can be opened
// TODO: better error handling - nuking the whole program seems like bad practice in this case
// since the error should be handleable
std::fstream openFileWrite(std::string fileName){
    std::fstream fs;
    fs.open(fileName, std::fstream::out | std::fstream::binary | std::fstream::trunc);
    if(!fs.good()){
        cout << "Failed to open " + fileName + "\n";
        exit(1);
    }
    return fs;
}


void writeFrame(string filePath, vector<array<float,3>> pointFrame){
    std::fstream fs = openFileWrite(filePath);
    for(auto point : pointFrame){
        char* start = reinterpret_cast<char*>(point.data());
        fs.write(start, sizeof(float) * 3);
    }
}