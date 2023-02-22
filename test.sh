#!/bin/bash
g++ -std=c++17 -o ./build/TestReadDepthEncoding ./test/testReadDepthEncoding.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp -I ./hdr -I ./thirdParty -Wall -Werror

./build/TestReadDepthEncoding

rm ./build/TestReadDepthEncoding