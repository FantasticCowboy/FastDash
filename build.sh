#!/bin/bash
g++ -std=c++17 -o ./build/FastDash ./src/main.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp ./src/screenToWorldTransform.cpp -I ./hdr -I ./thirdParty/eigen-3.4.0/Eigen -Wall -Werror