#!/bin/bash


#./testFiles/payload/out_0_%d.png
# build/debug/mp4KeypointsTest_%d.png
# ./scripts/pngToMp4.sh  build/debug/mp4KeypointsTest_%d.png ./test_color.mp4
ffmpeg -framerate 24 -i $1 $2 -y