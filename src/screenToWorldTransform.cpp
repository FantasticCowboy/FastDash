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

    x /= 200;
    y /= 200;
    x = 2 * x - 1;
    y = 2 * y - 1;
    x *= z * farClipPlaneDistance;
    y *= z * farClipPlaneDistance;
    float distance = z * farClipPlaneDistance; 
    // TODO: Get rod of the hardcoding yuck!
    array<float,4> out;
    int i = 0;
    for(auto row : transformMatrix){
        out[i] = x * row[0] +  y * row[1] + distance * row[2] + distance * row[3];
        i++;
    }
    w = out[3];
    array<float, 3> res = {out[1] ,out[0] ,out[2]};
    return res;
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