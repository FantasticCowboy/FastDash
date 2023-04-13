#include "getKeypoints.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;


std::vector<std::array<float,2>> keypoints(json &j){
    std::vector<std::array<float,2>> keypoints = j[0]["keypoints"].get<std::vector<std::array<float,2>>>();
    return keypoints;
}


std::vector<std::vector<std::array<float,2>>> getKeypoints(std::vector<std::string> filePaths){
    std::vector<std::vector<std::array<float,2>>> ret;
    for(auto path : filePaths){
        std::ifstream f(path);
        json data = json::parse(f);
        if(data.is_discarded()){
            throw std::runtime_error("Json parsing error");
        }
        ret.push_back(keypoints(data));
    }
    return ret;
}