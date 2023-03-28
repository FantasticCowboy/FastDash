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



TEST_CASE("Testing to make sure xPos, yPos, and Diff fall within their respective boundaries"){
    std::vector<DeltaEncodedPixel> pixles =readDepthEncoding("./test/testDeltaEncodedFiles/delta_encoding_raw_data/323546670_1");    
    for(auto &px : pixles){
        REQUIRE((px.xPos >= 0 && px.xPos < 200));
        REQUIRE((px.yPos >= 0 && px.yPos < 200));
        REQUIRE((px.diff > -1 && px.diff < 1));
    }

    std::vector<std::vector<float>> depthFrame = readDepthFrame(200, 200, "./test/testDeltaEncodedFiles/delta_encoding_raw_data/323546670_0" );

    std::vector<std::vector<float>> frame = reconstructFrame(depthFrame, pixles);


    writeFrameToDisk(frame, "./scratch/frame.py");
}

TEST_CASE("Test can decode many frames sequentially", "asd"){
    videoReader reader = videoReader("./test/testDeltaEncodedFiles/delta_encoding_raw_data/323546670", 200, 200);

    reader.getNextFrame();
    writeFrameToDisk(reader.getNextFrame(), "./scratch/frame1.py");
    for(int i = 0; i < 24; i++){
        reader.getNextFrame();
    }
    writeFrameToDisk(reader.getNextFrame(), "./scratch/frame8.py");    

}

// This test has some artifacts appearing in the outputted video
// I think that the artififacts are occuring because of loss of precision
// of the floating points in the writeVideoFrame function as the std::to_string
// method looses exactness
TEST_CASE("Write video to python file"){
    videoReader reader = videoReader("./test/testDeltaEncodedFiles/delta_encoding_raw_data/323546670", 200, 200);
    string data = startWriteVideo();

    for(int i = 0; i < 240; i++){
        writeVideoFrame(reader.getNextFrame(), data);
    }
    endWriteVideo(data);

    std::fstream file;
    file.open("./scratch/video.py", std::fstream::trunc | std::fstream::out);
    file << data;
}