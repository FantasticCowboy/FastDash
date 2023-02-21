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
        ~videoReader();        
        // DESCRIPTION: returns the next frame in the sequence if it exists
        // else it returns null
        vector<vector<float>> getNextFrame();
};

#endif