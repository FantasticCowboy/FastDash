#!/bin/bash
g++ -std=c++17 -o ./build/FastDash ./src/main.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp ./src/screenToWorldTransform.cpp -I ./hdr -I ./thirdParty -I /usr/include -Wall -Werror