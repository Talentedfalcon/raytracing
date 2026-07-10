#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"

int main(){
    hittable_list world;

    std::shared_ptr<material> metal_mat=std::make_shared<metal>(color(0.6,0.6,0.8),0.005);
    std::shared_ptr<sphere> mirror=std::make_shared<sphere>(point3(0,-101,-3),100,metal_mat);
    world.add(mirror);

    std::shared_ptr<texture> earth_texture=std::make_shared<image_texture>("./images/earthmap.jpg");
    std::shared_ptr<material> earth_mat=std::make_shared<lambertian>(earth_texture);
    std::shared_ptr<sphere> globe=std::make_shared<sphere>(point3(0,1,-3),2,earth_mat);
    world.add(globe);

    std::shared_ptr<material> glass_mat=std::make_shared<dielectric>(1.33);
    std::shared_ptr<sphere> marble=std::make_shared<sphere>(point3(-1,1,0),2,glass_mat);
    world.add(marble);

    camera cam;
    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=100;
    cam.max_depth=50;

    cam.vfov=20;
    cam.lookfrom=point3(7,7,12);
    cam.lookat=point3(0,0,-3);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=0;

    world=hittable_list(std::make_shared<bvh_node>(world));

    cam.render(world,"./renders/earth.ppm",16);
}