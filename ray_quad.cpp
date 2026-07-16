#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/quad.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"

int main(){
    hittable_list world;

    std::shared_ptr<material> left_red=std::make_shared<metal>(color(1.0, 0.2, 0.2),0);
    std::shared_ptr<material> back_green=std::make_shared<metal>(color(0.2, 1.0, 0.2),0);
    std::shared_ptr<material> right_blue=std::make_shared<metal>(color(0.2, 0.2, 1.0),0);
    std::shared_ptr<material> upper_orange=std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
    std::shared_ptr<material> lower_teal=std::make_shared<lambertian>(color(0.2, 0.8, 0.8));
    
    world.add(std::make_shared<quad>(point3(-2.5,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(std::make_shared<quad>(point3(-2,-2, 1), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(std::make_shared<quad>(point3( 2.5,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<quad>(point3(-2, 2.5, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<quad>(point3(-2,-2.5, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));
    
    std::shared_ptr<material> glass_ball=std::make_shared<dielectric>(1.42);
    std::shared_ptr<material> metal_ball=std::make_shared<metal>(color(0.7,0.6,0.5),0);

    world.add(std::make_shared<sphere>(point3(0,-1,3),1,glass_ball));
    world.add(std::make_shared<sphere>(point3(0,1,2),1,metal_ball));

    camera cam;

    cam.aspect_ratio      = 16.0/9.0;
    cam.image_width       = 1920;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(1,0,7);
    cam.lookat   = point3(-1,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world,"./renders/quads.ppm",16);
}