

#include <Eigen/Dense>
#include <array>
#include <vector>
using Eigen::Matrix4f;
using std::array;
using std::vector;

// Class to represent a transform matrix
// Transform matrices are used to convert a point from one coordinate space to another
// Values stored in this matrix are floats
// This class is necessary because including the Eigen library takes way too long
// consequently, instead of including the eigen library everytime, we only have to build this object file once
class transformMatrix4f
{
private:
    Matrix4f mat;
public:
    transformMatrix4f(array<array<float,4>,4> &mat_in);
    transformMatrix4f(vector<vector<float>> &mat_in);
    transformMatrix4f();
    bool operator==( const transformMatrix4f &rhs) const;
    void operator=( const transformMatrix4f &rhs);
    // multiplies in place
    void multiplyVec4f(array<float,4> &vec) const;
};

