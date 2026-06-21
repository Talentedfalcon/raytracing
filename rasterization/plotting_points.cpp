#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef float Point[3];

struct Pixel{
    unsigned char r,g,b;
};

int render(vector<Pixel> image,int image_width,int image_height){
    ofstream outFile("output.ppm",ios::binary);
    
    try{
        if(!outFile){
            cerr << "Error: Could not open file for writing" << endl;
            return 1;
        }
        outFile << "P6\n" << image_width << " " << image_height << "\n255\n";
    
        for(int y=0;y<image_height;y++){
            std::clog << "\rScanline remaining:" << (image_height-y) << " " << std::flush;
            for(int x=0;x<image_width;x++){
                int index=y*image_width+x;
                outFile.put(image[index].r);
                outFile.put(image[index].g);
                outFile.put(image[index].b);
            }
        }
        outFile.close();
        std::clog << "\rDone.                   \n";
    }
    catch(...){
        cout << "Caught an unhandled exception\n";
    }
    return 0;
}

int main(){
    Point corners[8]={
        {1, -1, -5},
        {1, -1, -3},
        {1, 1, -5},
        {1, 1, -3},
        {-1, -1, -5},
        {-1, -1, -3},
        {-1, 1, -5},
        {-1, 1, -3}
    };

    const unsigned int image_width=256,image_height=256;
    float aspect_ratio=image_width/image_height;

    vector<Pixel> image(image_width*image_height);

    for(int y=0;y<image_height;y++){
        for(int x=0;x<image_width;x++){
            int index=y*image_width+x;
            image[index].r=0;
            image[index].b=0;
            image[index].g=0;
        }
    }

    //Calculating the perspective divide of the points from 
    //a canvas that is 1 unit away from eye/camera 
    for (int i=0;i<8;i++){
        float x_proj=corners[i][0]/-corners[i][2];
        float y_proj=corners[i][1]/-corners[i][2]*aspect_ratio;
        printf("Projected corner %d: x:%f, y:%f\n",i,x_proj,y_proj);

        float x_proj_remap=(x_proj+1)/2;
        float y_proj_remap=(y_proj+1)/2;
        printf("\t\tx_remap:%f, y_remap:%f\n",x_proj_remap,y_proj_remap);

        //Raster Space
        int x_proj_pix=x_proj_remap*image_width;
        int y_proj_pix=y_proj_remap*image_height;
        printf("\t\tx_pix:%d, y_pix:%d\n",x_proj_pix,y_proj_pix);

        int proj_index=y_proj_pix*image_width+x_proj_pix;
        image[proj_index].r=255;
        image[proj_index].b=255;
    }

    render(image,image_width,image_height);

    return 0;
}