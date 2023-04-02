#include "cameraConfigs.hpp"
#include <gflags/gflags.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include <iostream>


DEFINE_string(cameraConfigsPath, "./testFiles/largeTestFiles/cameraConfigs.json", "Json file that contains camera projection information");

using json = nlohmann::json;
using std::cout;


// Description: parses json for camera config
// Throws: runtime error if parsing failed
cameraConfig parseConfig(json &config){

    array<array<float,4>,4> inverseMat = config["inverseProjectionMatrix"].get< array<array<float,4>,4> >();
    array<array<float,4>,4> localToWorldMat = config["localCoordinatestoWorldCoordinatesMatrix"].get< array<array<float,4>,4> >();

    return cameraConfig{
        config["filePrefix"], 
        transformMatrix4f(inverseMat),
        transformMatrix4f(localToWorldMat)
    };
}

static bool readConfigs = false;
static vector<cameraConfig> configs = vector<cameraConfig> ();

const vector<cameraConfig> getCameraConfigs(){
    if(!readConfigs){
        std::ifstream f(FLAGS_cameraConfigsPath);
        json data = json::parse(f);
        if(data.is_discarded()){
            throw std::runtime_error("Json parsing error");
        }

        for(auto &json : data){
            configs.push_back(parseConfig(json));
        }
        readConfigs = true;
    }
    return configs;
}