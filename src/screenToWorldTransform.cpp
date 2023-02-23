#include "screenToWorldTransformation.hpp"


using std::copy;
using std::begin;
using std::end;


screenToWorldTransformation::screenToWorldTransformation(
    array<array<float,4>, 4 > transformMatrixIn, float farClipPlaneIn, array<float,3> cameraPosIn) : farClipPlaneDistance(farClipPlaneIn){
    copy(begin(transformMatrixIn), end(transformMatrixIn), begin(transformMatrix));
    copy(begin(cameraPosIn), end(cameraPosIn), begin(cameraPos));
}


// TODO: This can likely be parallelized on the GPU
array<float,3> screenToWorldTransformation::transform(float x, float y, float z, float w){

    // TODO: Get rod of the hardcoding yuck!
    array<float,4> out = {x, y, z, w};
    int i = 0;
    for(auto row : transformMatrix){
        out[i] = x * row[0] +  y * row[1] + z * row[2] + w * row[3];
        i++;
    }

    w = out[3];

    array<float, 3> res = {out[0] * w,out[1] * w ,out[2] * farClipPlaneDistance * w };
    return res;
}