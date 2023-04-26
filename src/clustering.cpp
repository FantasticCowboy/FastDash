#include "clustering.hpp"
#include <iostream>
std::vector<cv::Point3f> clusterPoints(std::vector<cv::Point3f> &points){
    std::vector<cv::Point3f> centers;
    cv::Mat labels;
    //cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 10, 1.0), // TODO: understand what this line means

    std::cout << "points:" <<points.size() << "\n";
    int k = points.size() >= 25 ? 25 : points.size();
    if(k <= 0){
        return centers;
    }



    cv::kmeans(
        points,
        k,
        labels,
        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), // TODO: understand what this line means
        10,
        cv::KMEANS_PP_CENTERS,
        centers
    );
    std::cout << "labels:" << labels.size() << "\n";
    std::cout << "center:" << centers.size() << "\n";



    return centers;
}