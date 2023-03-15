#!/bin/bash

echo running ${1}.cpp ...
# Need to understand why this makes things work
# Export is a built-in command of the Bash shell. It is used to mark variables and functions to be passed to child processes.
# need to understand why -L does not cause the loader - ld - to know where to look for files. It seems like a design flaw.
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
g++ -### -std=c++17 -o ./build/test ./test/${1}.cpp ./src/readDepthFrame.cpp ./src/videoReader.cpp ./src/screenToWorldTransform.cpp -I /usr/include -I ./hdr -I ./thirdParty -I ./thirdParty/Eigen-3.4.0 -Wall -Werror -lopenpose -lgflags
./build/test

rm ./build/test

