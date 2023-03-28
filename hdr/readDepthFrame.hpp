#pragma once

#include <cstdint>
#include <vector>
#include <string>




typedef struct{
    float diff;
    uint32_t xPos;
    uint32_t yPos;

    
}DeltaEncodedPixel;


// DESCRIPTION: Reads in the depth frame as a 2d array of floats. The depth frame should have the format:
// [{float depth, float depth, float depth, float depth}, ...]. Each depth
// REQUIRES: width and height specify the corret dimensions of the frame.
// EFFECTS: Return vector is a 2d frame of floats that represent depths on a 0-1 scale. vector can be accessed with
// vec[xPos][yPos] to retrieve a pixel at xPos, yPos.
// TODO: need to specify how the float is stored on disk - big endia, small endian etc..
// TODO: need to sepcify what the order of the pixels are in the file i.e what coordinate system is the file using
std::vector<std::vector<float>> readDepthFrame(int width, int height, std::string fileName);
 
// DESCRIPTION: Reads pixels from the specified file. The specified file should have this format: 
// [{float diff, uint16 xpos, uint16 ypos}, ... , {float diff, uint16 xpos, uint16 ypos}]
// REQUIRES: filename is a valid path to a delta encoded file
// EFFECTS: Returned value is a vector of delta encoded pixels
// TODO: need to specify how the float, uint16, will be represented in the file - big endian, little endian.
std::vector<DeltaEncodedPixel> readDepthEncoding(std::string fileName);

// DESCRIPTION: Constructs the orignal depth frame from the pervious depth frame and the diff
// REQUIRES: prevFrame is not empty
// EFFECTS: returned vector is encoded depth frame
std::vector<std::vector<float>> reconstructFrame(std::vector<std::vector<float>> &prevFrame, std::vector<DeltaEncodedPixel> &diff);
