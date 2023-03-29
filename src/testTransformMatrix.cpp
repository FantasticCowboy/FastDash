#include "catch.hpp"
#include "transformMatrix.hpp"
#include <array>
#include <vector>

using std::array;

TEST_CASE("Transformation Matrix Sanity Check"){
    array<array<float,4>,4> mat_in = {{
        {{1,0,0,0}},
        {{0,1,0,0}},
        {{0,0,1,0}},
        {{0,0,0,1}}
    }};

    transformMatrix4f mat(mat_in);

    array<float,4> arr = {{1,1,1,1}};

    mat.multiplyVec4f(arr);

    bool val = ((arr[0] == 1 && arr[1] == 1 && arr[2] == 1 && arr[3] == 1));
    REQUIRE(val);
}

TEST_CASE("Transformation Matrix Sanity Check 2"){
    array<array<float,4>,4> mat_in = {{
        {{1,0,0,0}},
        {{0,1,0,0}},
        {{0,0,0,0}},
        {{0,0,0,0}}
    }};

    transformMatrix4f mat(mat_in);

    array<float,4> arr = {{1,1,1,1}};

    mat.multiplyVec4f(arr);

    bool val = ((arr[0] == 1 && arr[1] == 1 && arr[2] == 0 && arr[3] == 0));
    REQUIRE(val);
}