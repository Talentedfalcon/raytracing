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

    std::shared_ptr<material> red=std::make_shared<lambertian>(color(0.65,0.05,0.05));
    std::shared_ptr<material> white=std::make_shared<lambertian>(color(0.73,0.73,0.73));
    std::shared_ptr<material> green=std::make_shared<lambertian>(color(0.12,0.45,0.15));
    std::shared_ptr<material> light=std::make_shared<diffuse_light>(color(15,15,15));

    world.add(std::make_shared<quad>(point3(555,0,0),vec3(0,555,0),vec3(0,0,555),green));
    world.add(std::make_shared<quad>(point3(0,0,0),vec3(0,555,0),vec3(0,0,555),red));
    world.add(std::make_shared<quad>(point3(343,554,332),vec3(-130,0,0),vec3(0,0,-105),light));
    world.add(std::make_shared<quad>(point3(0,0,0),vec3(555,0,0),vec3(0,0,555),white));
    world.add(std::make_shared<quad>(point3(555,555,555),vec3(-555,0,0),vec3(0,0,-555),white));
    world.add(std::make_shared<quad>(point3(0,0,555),vec3(555,0,0),vec3(0,555,0),white));

    std::shared_ptr<material> metal_mat=std::make_shared<metal>(color(0.8,0.8,0.8),0.2);
    std::shared_ptr<hittable> box1=box(point3(0,0,0),point3(165,330,165),metal_mat);
    box1=std::make_shared<rotate>(box1,0,30,0);
    box1=std::make_shared<translate>(box1,vec3(265,0,295));
    world.add(box1);
    
    std::shared_ptr<material> glass=std::make_shared<dielectric>(1.54);
    std::shared_ptr<hittable> box2=box(point3(0,0,0),point3(165,165,165),glass);
    box2=std::make_shared<rotate>(box2,0,-18,0);
    box2=std::make_shared<translate>(box2,vec3(130,0,65));
    world.add(box2);
    
    camera cam;

    cam.aspect_ratio=1.0;
    cam.image_width=1080;
    cam.samples_per_pixel=10000;
    cam.max_depth=50;

    cam.background_a=color(0,0,0);
    cam.bg_is_gradient=false;

    cam.vfov=40;
    cam.lookfrom=point3(278,278,-800);
    cam.lookat=point3(278,278,0);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=0;

    cam.render(world,"./renders/cornell_box.ppm",16);
}