# LTS version of ubuntu that will stop recieving support in 2032
# changed from 18.04 to this because 18.04 had the wrong version of 
# c++ installed
FROM ubuntu:22.04


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
        libatlas-base-dev

RUN git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git

WORKDIR /openpose

RUN git checkout 2034f9c07526b885c16ba94eec4a4bd6cc8787c3

WORKDIR /scripts/ubuntu

RUN sed -i 's/\<sudo -H\>//g' install_deps.sh; \
    sed -i 's/\<sudo\>//g' install_deps.sh; \
    sync; sleep 1;

WORKDIR /openpose/build



# Downloads all available models. You can reduce image size by being more selective.
# TODO: I need to edit this list such that the cmake flags are updated to the new
# version of open pose
RUN cmake -DGPU_MODE:String=CPU_ONLY \
          -DOWNLOAD_BODY_25_MODEL:Bool=ON \
          -DDOWNLOAD_BODY_MPI_MODEL:Bool=ON \
          -DDOWNLOAD_BODY_COCO_MODEL:Bool=ON \
          -DDOWNLOAD_FACE_MODEL:Bool=ON \
          -DDOWNLOAD_HAND_MODEL:Bool=ON \
          -DUSE_MKL:Bool=OFF \
          ..

# Install g++ - likely do not need this later on, should just learn how to use cmake
RUN apt-get install g++=4:11.2.0-1ubuntu1

# you may find that you need to adjust this.
RUN make -j$((`nproc`+1))

RUN apt-get remove wget unzip cmake git build-essential -y && apt-get autoremove -y

# moves the header files to /usr/include in order to make sure that
# g++ can locate the files
RUN mv /openpose/include/* /usr/local/include
RUN mv /openpose/build/src/openpose/libopenpose.so /openpose/build/src/openpose/libopenpose.so.1.7.0 /usr/local/lib

# moves the open cv files as all the libs reference it using <opencv2/HEADER_FILE_NAME>
RUN mv /usr/include/opencv4/opencv2 /usr/include


# required for openpose c++ api
RUN apt-get install libgflags-dev

# allows you to run openpose anywhere similar to ls or grep
RUN mv /openpose/build/examples/openpose/openpose.bin /usr/bin/
RUN mv /usr/bin/openpose.bin /usr/bin/openpose

# install curl
RUN apt install curl

# install json parsing library
RUN mkdir /usr/local/include/nlohmann
RUN cd /usr/local/include/ && curl https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp > ./json.hpp

# install eigen matrix library
RUN apt-get install libeigen3-dev

WORKDIR /openpose