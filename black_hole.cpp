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

    world.add(black_hole(point3(0,0,0),1,5));

    std::shared_ptr<material> light_mat=std::make_shared<diffuse_light>(color(10,2,1));
    world.add(std::make_shared<quad>(point3(-0.5,-0.5,5),vec3(1,0,0),vec3(0,1,0),light_mat));

    world=hittable_list(std::make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=500;
    cam.max_depth=50;

    cam.vfov=45;
    cam.lookfrom=point3(1,1,-14);
    cam.lookat=point3(0,0,0);
    cam.vup=vec3(0,1,0);

    cam.background_a=color(0,0,0);
    cam.bg_is_gradient=false;

    cam.defocus_angle=10.0;
    cam.focus_dist=(cam.lookfrom-cam.lookat).length();

    cam.render(world,"./renders/black_hole.ppm",16);

    return 0;
}