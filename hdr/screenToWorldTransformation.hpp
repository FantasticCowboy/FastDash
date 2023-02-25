#include <vector>
#include <array>
using std::vector;
using std::array;

// DESCRIPTION: This is the transformation from a point on the screen
// in a camera to a point in the world. 
// TODO: Parallelize the computation of the transform on the GPU or CPU
// TODO: maybe update members to be constants in order to get better optimizations
class screenToWorldTransformation{
private:
    array<array< float,4> , 4 > transformMatrix;
    array<array< float,4> , 4 > localToWorld;
    float farClipPlaneDistance;
public:
    screenToWorldTransformation(array<array<float,4> ,4 > transformMatrixIn, float farClipPlaneIn, array<array< float,4> , 4 >  localToWorldIn);

    // DESCRIPTION: converts the x,y,z screen coordinate into world coordinates
    // REQUIRES: z is a float from 0-1 such that 0 means it is on the near clip plane and 1 means it is on the far clip plane
    array<float,3> transform(float x, float y, float z, float w);
};