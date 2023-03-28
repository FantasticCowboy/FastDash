#pragma once


#include <array>

using std::array;

array<float, 4> multiplyAndHomgenize(array<float,4> vec, array<array<float,4>, 4> mat){
    array<float, 4> out;

    int i = 0;

    
    for(auto row : mat){
        out[i] = row[0] * vec[0] + row[1] * vec[1] +row[2] * vec[2] + row[3] * vec[3];
        i++;
    }

    out = array<float,4>{{out[0] / out[3], out[1] / out[3], out[2] / out[3], out[3] / out[3] }};

    return out;
}