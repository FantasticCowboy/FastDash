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

TEST_CASE("Transformation Matrix Sanity Check 3"){
    array<array<float,4>,4> mat_in = {{
        {{2,4.23,8,123.23}},
        {{-234,43.2312344,323.214,-2314}},
        {{123,700,0,1}},
        {{0.0213,0.421,213,43.123}}
    }};

    transformMatrix4f mat(mat_in);

    array<float,4> arr = {{.5,.2133,123,1}};

    mat.multiplyVec4f(arr);

    // within .01 of zero
    Approx target = Approx(0).margin(.01);
    REQUIRE(arr[0] - 1109.132259 == target);
    REQUIRE(arr[1] - 37333.54322 == target);
    REQUIRE(arr[2] - 211.81 == target);
    REQUIRE(arr[3] - 26242.22344     == target);
}

TEST_CASE("Transformation Matrix Copy Check"){
    array<array<float,4>,4> mat_in = {{
        {{2,4.23,8,123.23}},
        {{-234,43.2312344,323.214,-2314}},
        {{123,700,0,1}},
        {{0.0213,0.421,213,43.123}}
    }};

    transformMatrix4f mat(mat_in);

    array<float,4> arr = {{.5,.2133,123,1}};

    transformMatrix4f matCopy = mat;

    matCopy.multiplyVec4f(arr);

    // within .01 of zero
    Approx target = Approx(0).margin(.01);
    REQUIRE(arr[0] - 1109.132259 == target);
    REQUIRE(arr[1] - 37333.54322 == target);
    REQUIRE(arr[2] - 211.81 == target);
    REQUIRE(arr[3] - 26242.22344     == target);
}