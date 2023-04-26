#include <vector>
#include <opencv2/opencv.hpp>


// Takes in a list of poitns and returns their centers
std::vector<cv::Point3f> clusterPoints(std::vector<cv::Point3f> &points);