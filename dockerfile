# LTS version of ubuntu that will stop recieving support in 2032
# changed from 18.04 to this because 18.04 had the wrong version of 
# c++ installed
FROM nvidia/cuda:11.4.1-cudnn8-devel-ubuntu20.04


ENV DEBIAN_FRONTEND=noninteractive



RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
        wget apt-utils lsb-core cmake git \
        libopencv-dev \
        protobuf-compiler \
        libprotobuf-dev \
        libgoogle-glog-dev \
        libboost-all-dev \
        hdf5-tools \
        libhdf5-dev \
        libatlas-base-dev\
        git

RUN git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git

WORKDIR /openpose

# RUN git checkout 2034f9c07526b885c16ba94eec4a4bd6cc8787c3

WORKDIR /scripts/ubuntu

RUN sed -i 's/\<sudo -H\>//g' install_deps.sh; \
    sed -i 's/\<sudo\>//g' install_deps.sh; \
    sync; sleep 1;

WORKDIR /openpose/build




# Install g++ - likely do not need this later on, should just learn how to use cmake
# RUN apt-get install g++




# required for openpose c++ api and FastDash
RUN apt-get install libgflags-dev -y


# install curl
RUN apt install curl -y

# install json parsing library
RUN mkdir /usr/local/include/nlohmann
RUN cd /usr/local/include/nlohmann && curl https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp > ./json.hpp

# install eigen matrix library
RUN apt-get install libeigen3-dev -y

# install libraries for python visualization scripts
RUN apt install pip -y
RUN pip3 install open3d
RUN pip3 install numpy

WORKDIR /openpose

RUN git submodule update --init --recursive --remote

# Downloads all available models. You can reduce image size by being more selective.
# TODO: I need to edit this list such that the cmake flags are updated to the new
# version of open pose
RUN ldconfig /usr/local/cuda/lib64
RUN ldconfig
RUN cmake -DDOWNLOAD_BODY_25_MODEL=ON . -S . -B ./build/ 
RUN ldconfig
RUN cd ./build && make -j$(nproc)


# allows you to run openpose anywhere similar to ls or grep
RUN mv /openpose/build/examples/openpose/openpose.bin /usr/bin/openposes

# moves the header files to /usr/include in order to make sure that
# g++ can locate the files
RUN mv /openpose/include/* /usr/local/include
RUN mv /openpose/build/src/openpose/libopenpose.so /openpose/build/src/openpose/libopenpose.so.1.7.0 /usr/local/lib

# moves the open cv files as all the libs reference it using <opencv2/HEADER_FILE_NAME>
RUN mv /usr/include/opencv4/opencv2 /usr/include
RUN ldconfig

# TODO: need a command to restart the container so that nvidia works