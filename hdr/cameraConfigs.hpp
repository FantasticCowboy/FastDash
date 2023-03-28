#include <vector>
#include <string>
#include "transformMatrix.hpp"


using std::vector;
using std::string;

struct cameraConfig{
    string filePrefix;
    transformMatrix4f inverseProjectionMatrix;
    transformMatrix4f localCoordinatestoWorldCoordinatesMatrix;
};

// DESCRIPTION: returns all of the camera configurations. 
const vector<cameraConfig> getCameraConfigs();

