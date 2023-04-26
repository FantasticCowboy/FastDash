#pragma once

#include <array>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using std::array;
using std::vector;
using std::string;


void writeFrame(string filePath, vector<array<float,3>> pointFrame);

void writeFrame(string filePath, vector<cv::Point3f> pointFrame);