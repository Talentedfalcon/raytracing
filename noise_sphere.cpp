#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"

int main(){
    std::srand(static_cast<unsigned int>(time(nullptr)));
    hittable_list world;

    std::shared_ptr<texture> noise_tex=std::make_shared<noise_texture>(4);
    std::shared_ptr<material> noise_mat=std::make_shared<lambertian>(noise_tex);
    world.add(std::make_shared<sphere>(point3(0,-1000,0),1000,noise_mat));
    world.add(std::make_shared<sphere>(point3(0,2,0),2,noise_mat));

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

    world=hittable_list(std::make_shared<bvh_node>(world));

    cam.render(world,"./renders/noise.ppm",12);
}