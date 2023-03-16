#include <vector>
#include <string>
#include <Eigen/Dense>



using std::vector;
using std::string;

struct cameraConfig{
    string filePrefix;

};

// DESCRIPTION: returns all of the camera configurations. 
vector<cameraConfig> getCameraConfigs();