#include <iostream>
#include "videoReader.hpp"
#include "screenToWorldTransformation.hpp"
#include <string>
#include <array>
#include <vector>

string prefix = "./test/testDeltaEncodedFiles/delta_encoding_raw_data/";
string uid0 = "0";
string uid1 = "1";
string uid2 = "2";
string uid3 = "3";

using std::array;
using std::vector;


array<array< float,4> , 4 > transformMatrix0 = { {
    {{0.36397	,0.00000,	0.00000,	0.00000}},
    {{0.00000	,0.36397,	0.00000,	0.00000}},
    {{0.00000	,0.00000,	0.00000,	-1.00000}},
    {{0.00000	,0.00000,	-1.61667,	1.71667}}
}};
array<array< float,4> , 4 > transformMatrix1 = { {
    {{0.36397	,0.00000	,0.00000	,0.00000}},
    {{0.00000	,0.36397	,0.00000	,0.00000}},
    {{0.00000	,0.00000	,0.00000	,-1.00000}},
    {{0.00000	,0.00000	,-1.61667	,1.71667}}
}};
array<array< float,4> , 4 > transformMatrix2 = { {
    {{0.36397	,0.00000,	0.00000,	0.00000}},
    {{0.00000	,0.36397,	0.00000,	0.00000}},
    {{0.00000	,0.00000,	0.00000,	-1.00000}},
    {{0.00000	,0.00000,	-1.61667,	1.71667}}
}};
array<array< float,4> , 4 > transformMatrix3 = { {
    {{0.36397	,0.00000,	0.00000,	0.00000}},
    {{0.00000	,0.36397,	0.00000,	0.00000}},
    {{0.00000	,0.00000,	0.00000,	-1.00000}},
    {{0.00000	,0.00000,	-1.61667,	1.71667}}
}};   

array<array< float,4> , 4 > localToGlobal0 = { {
    {{0.00000	,0.00000	,1.00000	,5.00000}},
    {{0.00000	,1.00000	,0.00000	,2.00000}},
    {{1.00000	,0.00000	,0.00000	,0.00000}},
    {{0.00000	,0.00000	,0.00000	,1.00000}}
}};
array<array< float,4> , 4 > localToGlobal1 = { {
    {{0.00000	    ,0.00000	,-1.00000	,-5.00000}},
    {{0.00000	    ,1.00000	,0.00000	,2.00000}},
    {{-1.00000	,0.00000	,0.00000	,0.00000}},
    {{0.00000	    ,0.00000	,0.00000	,1.00000}}
}};
array<array< float,4> , 4 > localToGlobal2 = { {
    {{-1.00000	,0.00000	,0.00000	,0.00000}},
    {{0.00000	,1.00000	,0.00000	,2.00000}},
    {{0.00000	,0.00000	,1.00000	,5.00000}},
    {{0.00000	,0.00000	,0.00000	,1.00000}}
}};
array<array< float,4> , 4 > localToGlobal3 = { {
    {{1.00000	,0.00000,	0.00000,	0.00000}},
    {{0.00000	,1.00000,	0.00000,	2.00000}},
    {{0.00000	,0.00000,	-1.00000,	-5.00000}},
    {{0.00000	,0.00000,	0.00000,	1.00000}}
}};      

static float farClipPlaneDistance = 10;
static screenToWorldTransformation screenToWorldtransformation0(transformMatrix0, farClipPlaneDistance, localToGlobal0);
static screenToWorldTransformation screenToWorldtransformation1(transformMatrix1, farClipPlaneDistance, localToGlobal1);
static screenToWorldTransformation screenToWorldtransformation2(transformMatrix2, farClipPlaneDistance, localToGlobal2);
static screenToWorldTransformation screenToWorldtransformation3(transformMatrix3, farClipPlaneDistance, localToGlobal3);


static int height = 1080;
static int width = 1920;
static int numFrames = 24;
int main(){
    std::cout << "Starting program..." << std::endl;
    videoReader reader0(prefix + uid0, 1080, 1920);
    videoReader reader1(prefix + uid1, 1080, 1920);
    videoReader reader2(prefix + uid2, 1080, 1920);
    videoReader reader3(prefix + uid3, 1080, 1920);

    for(int i = 0; i < numFrames; i++){
        vector<vector<float>> frame0 = reader0.getNextFrame();
        vector<vector<float>> frame1 = reader1.getNextFrame();
        vector<vector<float>> frame2 = reader2.getNextFrame();
        vector<vector<float>> frame3 = reader3.getNextFrame();
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                screenToWorldtransformation0.transform(x, y,frame0[x][y], 1);
                screenToWorldtransformation1.transform(x, y,frame1[x][y], 1);
                screenToWorldtransformation2.transform(x, y,frame2[x][y], 1);
                screenToWorldtransformation3.transform(x, y,frame3[x][y], 1);
            }
        }
    }  
    std::cout << "Exiting program..." << std::endl;
}