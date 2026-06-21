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
                double r=double(x)/(image_width-1);
                double g=0.0;
                double b=double(y)/(image_height-1);

                int ir=int(255*r);
                int ig=int(255*g);
                int ib=int(255*b);

                outFile.put(ir);
                outFile.put(ig);
                outFile.put(ib);
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