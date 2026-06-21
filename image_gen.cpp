#include "vec3.h"
#include "color.h"

#include <iostream>
#include <fstream>

int main(){
    int image_width=256;
    int image_height=256;

    std::ofstream outFile("temp.ppm",std::ios::binary);

    try{
        if(!outFile){
            std::cerr << "Error: Could not open file for writing" << std::endl;
            return 1;
        }
        outFile << "P6\n" << image_width << " " << image_height << "\n255\n";

        for(int y=0;y<image_height;y++){
            std::clog << "\rScanline remaining:" << (image_height-y) << " " << std::flush;
            for(int x=0;x<image_width;x++){
                color pixel_color(
                    double(x)/(image_width-1),
                    double(y)/(image_height-1),
                    0
                );
                write_color(outFile,pixel_color);
            }
        }
        outFile.close();
        std::clog << "\rDone.                   \n";
    }
    catch(...){
        std::cout << "Caught an unhandled exception\n";
    }

    return 0;
}