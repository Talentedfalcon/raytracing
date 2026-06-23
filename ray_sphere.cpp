#include "raytrace.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <fstream>

color ray_color(const ray& r, const hittable& world){
    hit_record rec;
    if(world.hit(r,0,infinity,rec)){
        return 0.5*(rec.normal+color(1,1,1));
    }
    vec3 unit_dir=unit_vector(r.direction());
    double a=0.5*(unit_dir.y()+1.0);
    return (1.0-a)*color(1,1,1)+a*color(0,0,1);
}

int main(){
    float aspect_ratio=16.0/9.0;
    int image_width=860;
    int image_height=int(image_width/aspect_ratio);
    image_height=(image_height<1)?1:image_height;

    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0,0,-1),0.5));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1),100));

    float focal_length=1.0;
    float viewport_height=2.0;
    float viewport_width=viewport_height*(double(image_width)/image_height);
    point3 camera_center(0,0,0);

    vec3 viewport_u(viewport_width,0,0);
    vec3 viewport_v(0,-viewport_height,0);

    vec3 pixel_delta_u=viewport_u/image_width;
    vec3 pixel_delta_v=viewport_v/image_height;

    vec3 viewport_upper_left=camera_center-vec3(0,0,focal_length)-viewport_u/2-viewport_v/2;
    vec3 pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);

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
                vec3 pixel_center=pixel00_loc+(x*pixel_delta_u)+(y*pixel_delta_v);
                vec3 ray_direction=pixel_center-camera_center;
                ray r(camera_center,ray_direction);

                color pixel_color=ray_color(r,world);
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