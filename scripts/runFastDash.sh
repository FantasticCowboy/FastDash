#!/bin/bash

make FastDash && ./build/FastDash --frameHeight=200 --frameWidth=200 --calculateKeypoints=true --writeFramesToDisk=true  --transformFrames=true --numFrames=240 --encodingFilesLocation=./testFiles/bigTest/ --cameraConfigsPath=./testFiles/bigTest/cameraConfigs.json