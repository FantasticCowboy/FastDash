#!/bin/bash
g++ -std=c++17 -o ./build/FastDash ./src/main.cpp ./src/readDepthFrame.cpp -I ./hdr -Wall -Werror