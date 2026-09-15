#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/quad.h"
#include "./rt_headers/bvh.h"
#include "./rt_headers/constant_medium.h"

int main(){
    hittable_list world;
    
    std::vector<color> grad_colors={color(1,0,0),color(0,0,1),color(0,1,0)}; 
    
    double angles[]={0,30,45,60,90,120};
    
    for(int i=0;i<6;i++){
        std::shared_ptr<texture> linear_grad=std::make_shared<linear_gradient_texture>(grad_colors,angles[i]);
        world.add(std::make_shared<quad>(vec3(35*(i-3),20,0),vec3(30,0,0),vec3(0,30,0),std::make_shared<diffuse_light>(linear_grad)));
        world.add(std::make_shared<sphere>(point3(35*(i-3)+15,-20,0),15,std::make_shared<diffuse_light>(linear_grad)));
    }

    world=hittable_list(std::make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=600;
    cam.samples_per_pixel=100;
    cam.max_depth=50;

    cam.vfov=45;
    cam.lookfrom=point3(0,0,-160);
    cam.lookat=point3(0,0,0);
    cam.vup=vec3(0,1,0);

    cam.background_a=color(0,0,0);
    cam.bg_is_gradient=false;

    cam.render(world,"./renders/gradient_test.ppm",16);

    return 0;
}