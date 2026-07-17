#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/quad.h"
#include "./rt_headers/bvh.h"

int main(){
    hittable_list world;

    std::shared_ptr<texture> image_tex=std::make_shared<image_texture>("./images/colorsplotch.jpg");
    std::shared_ptr<material> image_mat=std::make_shared<lambertian>(image_tex);
    world.add(std::make_shared<sphere>(point3(0,-1000,0),1000, image_mat));

    std::shared_ptr<material> glass_mat=std::make_shared<dielectric>(1.42);
    world.add(std::make_shared<sphere>(point3(0,2,0),2,glass_mat));

    std::shared_ptr<material> light_mat=std::make_shared<diffuse_light>(color(4,4,4));
    world.add(std::make_shared<sphere>(point3(0,7,0),2,light_mat));
    world.add(std::make_shared<quad>(point3(3,1,-2),vec3(2,0,0),vec3(0,2,0),light_mat));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=500;
    cam.max_depth=50;

    cam.background_a=color(0,0,0);
    cam.bg_is_gradient=false;

    cam.vfov=20;
    cam.lookfrom=point3(26,3,6);
    cam.lookat=point3(0,2,0);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=0;

    cam.render(world,"./renders/lights.ppm",16);
}