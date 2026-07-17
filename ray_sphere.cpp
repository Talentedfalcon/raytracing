#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"

int main(){
    hittable_list world;

    std::shared_ptr<lambertian> mat_ground=std::make_shared<lambertian>(color(0.5,0.8,0.3));
    std::shared_ptr<lambertian> mat_diffuse=std::make_shared<lambertian>(color(0.5,0.1,0.8));
    std::shared_ptr<metal> mat_steel=std::make_shared<metal>(color(0.8,0.8,0.8),0.2);
    std::shared_ptr<metal> mat_gold=std::make_shared<metal>(color(0.8,0.6,0.2),0);
    std::shared_ptr<dielectric> mat_glass=std::make_shared<dielectric>(1.5); //(refractive index of air)/(refractive index of water)
    std::shared_ptr<dielectric> mat_bubble=std::make_shared<dielectric>(1/1.5); //(refractive index of air)/(refractive index of water)

    world.add(std::make_shared<sphere>(point3(0,-100.5,-1),100,mat_ground));
    world.add(std::make_shared<sphere>(point3(0,0,-2.0),0.5,mat_diffuse));
    world.add(std::make_shared<sphere>(point3(1,0,-1.5),0.5,mat_steel));
    world.add(std::make_shared<sphere>(point3(-1,0,-1.5),0.5,mat_gold));
    world.add(std::make_shared<sphere>(point3(0.25,0,-1.0),0.4,mat_glass));
    world.add(std::make_shared<sphere>(point3(0.25,0,-1.0),0.3,mat_bubble));

    world=hittable_list(std::make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=100;
    cam.max_depth=50;

    cam.vfov=45;
    cam.lookfrom=point3(1.5,1.5,1);
    cam.lookat=point3(0,0,-1.5);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=10.0;
    cam.focus_dist=(cam.lookfrom-cam.lookat).length();

    cam.render(world,"./renders/output.ppm",8);

    return 0;
}