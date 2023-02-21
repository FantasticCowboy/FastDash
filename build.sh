#!/bin/bash
g++ -std=c++17 -o ./build/FastDash ./src/main.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp -I ./hdr -Wall -Werror