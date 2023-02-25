#include "screenToWorldTransformation.hpp"


using std::copy;
using std::begin;
using std::end;


screenToWorldTransformation::screenToWorldTransformation(
    array<array<float,4> ,4 > transformMatrixIn, float farClipPlaneIn, array<array< float,4> , 4 >  localToWorldIn) : farClipPlaneDistance(farClipPlaneIn){
    copy(begin(transformMatrixIn), end(transformMatrixIn), begin(transformMatrix));
    copy(begin(localToWorldIn), end(localToWorldIn), begin(localToWorld));
}


array<float, 4> matrixMultiply(array<float,4> vec, array<array<float,4>, 4> mat){
    array<float, 4> out;
    int i = 0;
    for(auto row : mat){
        out[i] = row[0] * vec[0] + row[1] * vec[1] +row[2] * vec[2] + row[3] * vec[3];
        i++;
    }
    return out;
}

// TODO: This can likely be parallelized on the GPU
array<float,3> screenToWorldTransformation::transform(float x, float y, float z, float w){
    float distance = z * farClipPlaneDistance; 
    x /= 200;
    y /= 200;
    x = 2 * x - 1;
    y = 2 * y - 1;
    x *= distance;
    y *= distance;
    // TODO: Get rod of the hardcoding yuck!
    array<float, 4> out = {x,y, distance, distance}; 
    out = matrixMultiply(out, transformMatrix);
    matrixMultiply(out, localToWorld);

    array<float, 3> res = {{out[0],out[1],out[2]}};

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