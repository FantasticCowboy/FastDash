#include "transformMatrix.hpp"
#include <Eigen/Dense>
#include <iostream>


using std::cout;
using Eigen::Vector4f;

transformMatrix4f::transformMatrix4f(array<array<float,4>,4> &mat_in){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            mat(i,j) = mat_in[i][j];
        }
    }
}

transformMatrix4f::transformMatrix4f(){

}


transformMatrix4f::transformMatrix4f(vector<vector<float>> &mat_in){
    if(mat_in.empty() || mat_in.size() != 4 || mat_in[0].size() != 4){
        throw std::runtime_error("Needs to be a 4x4 vector");
    } 

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            mat(i,j) = mat_in[i][j];
        }
    }
}

bool transformMatrix4f::operator==(const transformMatrix4f &lhs) const{
    return mat.isApprox(lhs.mat);
}

void transformMatrix4f::operator=(const transformMatrix4f &lhs){
    mat = lhs.mat;
}



void transformMatrix4f::multiplyVec4f(array<float,4> &vec) const{
    Vector4f v;
    v << vec[0],vec[1],vec[2],vec[3];
    v = mat * v;
    vec[0] = v[0];
    vec[1] = v[1];    
    vec[2] = v[2];    
    vec[3] = v[3];
}
