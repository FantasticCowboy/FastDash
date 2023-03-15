#ifndef VIDEOREADER
#define VIDEOREADER

#include <string>
#include <vector> 
using std::vector;
using std::string;




// DESCRIPTION: used to read frames from disk
class videoReader{
    private:
        vector<vector<float>> prevFrame;
        string prefix;
        int cur_frame = 0;     
        int height; 
        int width;
        string getFileName();
    public:
        videoReader(std::string prefix, int height, int width);

        // DESCRIPTION: returns the next frame in the sequence if it exists
        // REQUIRES: next frame to be read exists
        // EFFECTS: returned value is the next frame in the sequence
        // THROWS: std::runtime_error if next file cannot be exists or does not exist
        vector<vector<float>> getNextFrame();

        // DESCRIPTION: checks to see if the next frame can be read
        bool hasNextFrame();
};

#endif