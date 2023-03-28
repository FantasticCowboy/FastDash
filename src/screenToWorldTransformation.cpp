#include "screenToWorldTransformation.hpp"


using std::copy;
using std::begin;
using std::end;


screenToWorldTransformation::screenToWorldTransformation(
    transformMatrix4f &transformMatrixIn, float farClipPlaneIn, transformMatrix4f  &localToWorldIn) {
    farClipPlaneDistance = farClipPlaneIn;
    transformMatrix = transformMatrixIn;
    localToWorld = localToWorldIn;
}

// TODO: This can likely be parallelized on the GPU
// TOOD: I need to understand what is going on and why this transformation works correctly
// TODO: this function will likely be the most time consuming
array<float,3> screenToWorldTransformation::transformPixel(float x, float y, float z, float w) const{
    float distance = z * farClipPlaneDistance; 
    x /= 200;
    y /= 200;
    x = 2 * x - 1;
    y = 2 * y - 1;
    x *= distance;
    y *= distance;
    array<float, 4> out = {x,y, distance, distance}; 
    transformMatrix.multiplyVec4f(out);
    out = {out[1], out[0], out[2], 1};
    localToWorld.multiplyVec4f(out);
    array<float, 3> res = {{out[0],out[1], out[2]}};
    return res;
}

vector<array<float,3>> screenToWorldTransformation::transformFrame(const vector<vector<float>> &frame) const{
    
    vector<array<float,3>> points;
    uint x = 0;
    for(auto &row : frame){
        uint y = 0;
        for(auto z : row){
            points.push_back(transformPixel(x, y, z, 1));
            y++;
        }
        y++;
    }
    return points;
}

void screenToWorldTransformation::transformFrame(const vector<vector<float>> &frame, vector<array<float,3>> coords) const{
    uint x = 0;
    for(auto &row : frame){
        uint y = 0;
        for(auto z : row){
            coords.push_back(transformPixel(x, y, z, 1));
            y++;
        }
        y++;
    }
}

vector<array<float,3>> screenToWorldTransformation::transformFrames(const vector<vector<vector<float>>> &frames) const{
    vector<array<float,3>> points;
    for(auto &frame : frames){
        transformFrame(frame, points);
    }
    return points;
}


/*
    // TODO: need to make sense of the answer below
    https://answers.unity.com/questions/1014337/calculation-be$$anonymous$$nd-cameraworldtoscreenpoint.html

   public static Vector3 ManualScreenToWorldPoint(Vector2 screenPoint, float distance, Matrix4x4 cameraToWorldMatrix, Matrix4x4 projectionMatrixInverse) {
     // here we are converting screen point in screen space to camera space point placed on a plane "distance" away from the camera
     // screen point is in range [(0,0) - (Screen.Width, Screen.Height)]
     Vector2 pointViewportSpace = screenPoint / new Vector2(Screen.width, Screen.height); // convert space [(0,0) - (Screen.Width, Screen.Height)] to [(0,0) - (1,1)]
     Vector2 pointCameraSpaceNormalized = (pointViewportSpace * 2.0f) - Vector2.one; // convert space [(0,0) - (1,1)] to [(-1,-1) - (1,1)]
     Vector2 pointCameraSpace = pointCameraSpaceNormalized * distance; // convert space [(-1,-1) - (1,1)] to [(-dist,-dist) - (dist, dist)]
     Vector4 planePoint = new Vector4(pointCameraSpace.x, pointCameraSpace.y, distance, distance); // define the point (don't know why z and w components need to be set to distance)
 
     // calculate convertion matrix from camera space to world space
     Matrix4x4 matrix = cameraToWorldMatrix * projectionMatrixInverse;
     // multiply world point by VP matrix
     Vector4 worldPoint = matrix * planePoint;
 
    returnn worldPoint
*/