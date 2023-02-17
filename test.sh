#!/bin/bash
g++ -std=c++17 -o ./build/TestReadDepthEncoding ./test/testReadDepthEncoding.cpp ./src/readDepthFrame.cpp -I ./hdr -I ./thirdParty -Wall -Werror

./build/TestReadDepthEncoding

rm ./build/TestReadDepthEncoding