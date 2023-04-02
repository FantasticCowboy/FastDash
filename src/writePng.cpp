#include "writePng.hpp"
#include <string.h>
#include <libpng16/png.h>
#include <stdexcept>
#include <iostream>

// TODO: need to enforce that the sizes are equal
void writePng(vector<vector<float>> frame, string filePath){

    if(frame.empty() || frame.front().empty()){
        throw std::runtime_error("2d Vector and it's elements cannot be empty!");
    }


    FILE *fp = fopen(filePath.c_str(), "wb");
    
    if (!fp) return;   

    png_image img;
    memset(&img, 0, sizeof(png_image));
    img.version = PNG_IMAGE_VERSION;
    img.opaque = NULL;
    img.width = frame[0].size();
    img.height = frame.size();
    img.format = PNG_FORMAT_GRAY;
    img.flags = 0;
    
    png_bytep* buffer;

    buffer = reinterpret_cast<png_bytep*>(malloc(PNG_IMAGE_SIZE(img)));
    char* ptr = reinterpret_cast<char*>(buffer);

    int bufferPos = 0;
    for(auto row : frame){
        for(auto val : row){
            memset(ptr + bufferPos, val * 255, 1);
            ptr++;
        }
    }
    png_image_write_to_file(&img, filePath.c_str(),0, buffer, PNG_IMAGE_ROW_STRIDE(img),  NULL);

}