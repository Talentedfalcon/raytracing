#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <fstream>
#include <string>

class camera{
    public:
        float aspect_ratio=1.0;
        int image_width=100;
        int samples_per_pixel=10;
        int max_depth=10;

        double vfov=90;
        point3 lookfrom=point3(0,0,0);
        point3 lookat=point3(0,0,-1);
        vec3 vup=vec3(0,1,0);

        double defocus_angle=0;
        double focus_dist=10;

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

        vec3 u,v,w;

        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void initialize(){
            image_height=int(image_width/aspect_ratio);
            image_height=(image_height<1)?1:image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            camera_center=lookfrom;

            double theta=degrees_to_radians(vfov);
            double h=std::tan(theta/2);
            float viewport_height=2*h*focus_dist;
            float viewport_width=viewport_height*(double(image_width)/image_height);

            w=unit_vector(lookfrom-lookat);
            u=unit_vector(cross(vup,w));
            v=cross(w,u);

            vec3 viewport_u=viewport_width*u;
            vec3 viewport_v=viewport_height*-v;

            pixel_delta_u=viewport_u/image_width;
            pixel_delta_v=viewport_v/image_height;

            vec3 viewport_upper_left=camera_center-(focus_dist*w)-viewport_u/2-viewport_v/2;
            pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);
            
            double defocus_radius=focus_dist*std::tan(degrees_to_radians(defocus_angle/2));
            defocus_disk_u=u*defocus_radius;
            defocus_disk_v=v*defocus_radius;
        }

        ray get_ray(int x,int y) const{
            vec3 offset=sample_square();

            vec3 pixel_sample=pixel00_loc
                            +((x+offset.x())*pixel_delta_u)
                            +((y+offset.y())*pixel_delta_v);
            
            vec3 ray_origin=(defocus_angle<=0)?camera_center:defocus_disk_sample();
            vec3 ray_direction=pixel_sample-ray_origin;
            
            return ray(camera_center,ray_direction);
        }

        vec3 sample_square() const{
            return vec3(random_double()-0.5,random_double()-0.5,0);
        }

        point3 defocus_disk_sample() const{
            vec3 p=random_in_unit_disk();
            return camera_center+(p[0]*defocus_disk_u)+(p[1]*defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world){
            if(depth<=0){
                return color();
            }

            hit_record rec;
            if(world.hit(r,interval(0.001,infinity),rec)){
                ray scattered;
                color attenuation;
                if(rec.mat->scatter(r,rec,attenuation,scattered)){
                    return attenuation*ray_color(scattered,depth-1,world);
                }
                return color();
            }
            vec3 unit_dir=unit_vector(r.direction());
            double a=0.5*(unit_dir.y()+1.0);
            return (1.0-a)*color(1,1,1)+a*color(0.5,0.7,1);
        }
};

#endif