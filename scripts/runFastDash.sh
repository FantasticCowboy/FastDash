#!/bin/bash

# make FastDash && ./build/FastDash --frameHeight=200 --frameWidth=200 --calculateKeypoints=true --writeFramesToDisk=true  --transformFrames=true --numFrames=240 --encodingFilesLocation=./testFiles/bigTest/ --cameraConfigsPath=./testFiles/bigTest/cameraConfigs.json


make FastDash && ./build/FastDash --Debug_read_in_frames=true --frameHeight=200 --frameWidth=200 --calculateKeypoints=true --writeFramesToDisk=true  --transformFrames=true --numFrames=24 --encodingFilesLocation=./testFiles/endToEndTestFiles/test200x200/DeltaEncodings/ --cameraConfigsPath=./testFiles/endToEndTestFiles/test200x200/DeltaEncodings/cameraConfigs.json