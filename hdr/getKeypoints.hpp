#pragma once
#include <vector>
#include <string>
#include <array>
#include <gflags/gflags.h>



std::vector<std::vector<std::array<float,2>>> getKeypoints(std::vector<std::string> filePaths);

