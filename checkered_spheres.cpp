#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"
#include "./rt_headers/texture.h"

int main(){
    hittable_list world;

    std::shared_ptr<texture> checker=std::make_shared<checker_texture>(0.32,color(0,0,0.5),color(0.9,0.9,0.9));

    std::shared_ptr<material> checker_mat=std::make_shared<lambertian>(checker);
    world.add(std::make_shared<sphere>(point3(0,-10,0),10,checker_mat));
    world.add(std::make_shared<sphere>(point3(0,10,0),10,checker_mat));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=100;
    cam.max_depth=50;

    cam.vfov=20;
    cam.lookfrom=point3(13,2,3);
    cam.lookat=point3(0,0,0);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=0;

    cam.render(world,"./renders/checker_spheres.ppm",12);
}