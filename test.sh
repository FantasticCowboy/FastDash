#!/bin/bash
g++ -std=c++17 -o ./build/TestReadDepthEncoding ./test/testReadDepthEncoding.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp ./src/screenToWorldTransform.cpp -I ./hdr -I ./thirdParty -I ./thirdParty/Eigen-3.4.0 -Wall -Werror

./build/TestReadDepthEncoding

rm ./build/TestReadDepthEncoding