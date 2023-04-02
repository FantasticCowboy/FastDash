#include <vector>
#include <iostream>
#include "readDepthFrame.hpp"
#include "catch.hpp"
#include <fstream>
#include "videoReader.hpp"
#include <array>
#include "screenToWorldTransformation.hpp"
#include <iostream>  
#include <iomanip>      
#include <sstream>
#include "matrix.hpp"
#include "writePng.hpp"

using std::cout;
using std::vector;
using std::array;


void writeFrameToDisk(vector<vector<float>> frame, string fileName){
    std::fstream file;
    cout << "writing frame to disk\n";
    file.open(fileName, std::fstream::out | std::fstream::trunc );
    REQUIRE(file.good());

    file << "frame = [";
    bool first = true;
    for(auto &row : frame){
        if(first){
            file << "[";
            first = false;
        }else{
            file << ",[";
        }
        for(auto depth : row){
            file << depth << ",";
        }
        file << "0 ]";
    }
    file << "]";
}


void writePoints(vector<array<float,3>> points){
    string data = "points = [";
    std::ostringstream foo(data);        
    for(auto point : points){
        foo << "[" + std::to_string(point[0]) + "," + std::to_string(point[1]) + ","  << std::setprecision(9) << point[2] <<  "],";
    }
    data += foo.str();    
    data.pop_back();
    data += "]";
    std::fstream f;
    f.open("./scratch/pointCloud.py", std::fstream::trunc | std::fstream::out);
    f << data;    
}


string startWriteVideo(){
    return "video = [";
}
void endWriteVideo(string &data){
    data.pop_back();
    data.push_back(']');
}

void writeVideoFrame(vector<vector<float>> frame, string &data){
    data += "[";
    for(auto &row : frame){
        data += "[";
        for(auto num : row){
            data += std::to_string(num) + ",";
        }        
        data.pop_back();
        data += "],";
    }
    data.pop_back();
    data += "],";
}


TEST_CASE("Write to png"){
    vector<vector<float>> tmp = readDepthFrame(200,200, "./testFiles/largeTestFiles/2_0");
    writePng(tmp, "./out.png");
}