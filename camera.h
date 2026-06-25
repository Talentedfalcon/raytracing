#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include <fstream>
#include <string>

class camera{
    public:
        float aspect_ratio=1.0;
        int image_width=100;
        int samples_per_pixel=10;
        int max_depth=10;

        int render(const hittable& world,const std::string& file_name){
            initialize();
            std::ofstream outFile(file_name,std::ios::binary);

            try{
                if(!outFile){
                    std::cerr << "Error: Could not open file for writing" << std::endl;
                    return 1;
                }
                outFile << "P6\n" << image_width << " " << image_height << "\n255\n";

                for(int y=0;y<image_height;y++){
                    std::clog << "\rScanline remaining:" << (image_height-y) << " " << std::flush;
                    for(int x=0;x<image_width;x++){
                        color pixel_color(0,0,0);
                        for(int sample=0;sample<samples_per_pixel;sample++){
                            ray r=get_ray(x,y);
                            pixel_color+=ray_color(r,max_depth,world);
                        }
                        write_color(outFile,pixel_samples_scale*pixel_color);
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
        
    private:
        int image_height;
        double pixel_samples_scale;
        point3 camera_center;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 pixel00_loc;

        void initialize(){
            image_height=int(image_width/aspect_ratio);
            image_height=(image_height<1)?1:image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            float focal_length=1.0;
            float viewport_height=2.0;
            float viewport_width=viewport_height*(double(image_width)/image_height);
            camera_center=point3(0,0,0);

            vec3 viewport_u(viewport_width,0,0);
            vec3 viewport_v(0,-viewport_height,0);

            pixel_delta_u=viewport_u/image_width;
            pixel_delta_v=viewport_v/image_height;

            vec3 viewport_upper_left=camera_center-vec3(0,0,focal_length)-viewport_u/2-viewport_v/2;
            pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);
        }

        ray get_ray(int x,int y) const{
            vec3 offset=sample_square();

            vec3 pixel_sample=pixel00_loc
                            +((x+offset.x())*pixel_delta_u)
                            +((y+offset.y())*pixel_delta_v);
            
            vec3 ray_direction=pixel_sample-camera_center;
            
            return ray(camera_center,ray_direction);
        }

        vec3 sample_square() const{
            return vec3(random_double()-0.5,random_double()-0.5,0);
        }

        color ray_color(const ray& r, int depth, const hittable& world){
            if(depth<=0){
                return color();
            }

            hit_record rec;
            if(world.hit(r,interval(0.001,infinity),rec)){
                vec3 direction=rec.normal+random_unit_vector();
                return 0.5*ray_color(ray(rec.p,direction),depth-1,world);
            }
            vec3 unit_dir=unit_vector(r.direction());
            double a=0.5*(unit_dir.y()+1.0);
            return (1.0-a)*color(1,1,1)+a*color(0.5,0.7,1);
        }
};

#endif