#pragma once

#include <vector>
#include <array>
#include "transformMatrix.hpp"
using std::vector;
using std::array;


// DESCRIPTION: This is the transformation from a point on the screen
// in a camera to a point in the world. 
// TODO: Parallelize the computation of the transform on the GPU or CPU
// TODO: maybe update members to be constants in order to get better optimizations
// TODO: Honestly in all of the implementations where I return a vector, I can preallocate the space - currently I am just calling push back 
class screenToWorldTransformation{
private:
    transformMatrix4f transformMatrix;
    transformMatrix4f localToWorld;
    float farClipPlaneDistance;
public:
    screenToWorldTransformation(transformMatrix4f &transformMatrixIn, float farClipPlaneIn, transformMatrix4f &localToWorldIn);

    // DESCRIPTION: converts the x,y,z screen coordinate into world coordinates
    // REQUIRES: z is a float from 0-1 such that 0 means it is on the near clip plane and 1 means it is on the far clip plane
    array<float,3> transformPixel(float x, float y, float z, float w) const;

    // Description: converts all of the coordinates into world coordinates and returns a new frame containing the data
    vector<array<float,3>> transformFrame(const vector<vector<float>> &frame) const;

    // Description: converts all of the coordinates into world coordinates. Appends values to the coords vector
    void transformFrame(const vector<vector<float>> &frame, vector<array<float,3>> coords) const;


    // Decscription: converts all of the pixels in the frames into world coordiantes 
    vector<array<float,3>> transformFrames(const vector<vector<vector<float>>> &frames) const;

};