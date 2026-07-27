#include "./rt_headers/raytrace.h"
#include "./rt_headers/camera.h"

#include "./rt_headers/hittable.h"
#include "./rt_headers/hittable_list.h"
#include "./rt_headers/material.h"
#include "./rt_headers/constant_medium.h"
#include "./rt_headers/quad.h"
#include "./rt_headers/sphere.h"
#include "./rt_headers/bvh.h"

int main(){
    std::srand(static_cast<unsigned int>(time(nullptr)));

    hittable_list boxes1;

    std::shared_ptr<material> ground=std::make_shared<lambertian>(color(0.48,0.83,0.53));

    int boxes_per_side=20;

    //Floor Grid Tiling
    for(int i=0;i<boxes_per_side;i++){
        for(int j=0;j<boxes_per_side;j++){
            double w=100;
            double x0=-1000+i*w;
            double z0=-1000+j*w;
            double y0=0;
            double x1=x0+w;
            double y1=random_double(1,101);
            double z1=z0+w;

            boxes1.add(box(point3(x0,y0,z0),point3(x1,y1,z1),ground));
        }
    }

    hittable_list world;
    world.add(std::make_shared<bvh_node>(boxes1));

    //Light
    std::shared_ptr<material> light=std::make_shared<diffuse_light>(color(7,7,7));
    world.add(std::make_shared<quad>(point3(123,554,147),vec3(300,0,0),vec3(0,0,265),light));

    //Motion blur sphere
    point3 center1=point3(400,400,200);
    point3 center2=center1+vec3(-50,50,0);
    std::shared_ptr<material> sphere_mat=std::make_shared<lambertian>(color(0.7,0.3,0.1));
    std::shared_ptr<sphere> mb_sphere=std::make_shared<sphere>(center1,center2,50,sphere_mat);
    world.add(mb_sphere);

    //Glass sphere
    std::shared_ptr<material> glass_mat=std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(260,150,45),50,glass_mat));

    //Fuzzy Metal sphere
    std::shared_ptr<material> metal_mat=std::make_shared<metal>(color(0.8,0.8,0.9),0.5);
    world.add(std::make_shared<sphere>(point3(0,150,145),50,metal_mat));

    //Glossy Sphere
    std::shared_ptr<sphere> boundary=std::make_shared<sphere>(point3(360,150,145),70,glass_mat);
    world.add(boundary);
    world.add(std::make_shared<constant_medium>(boundary,0.2,color(0.2,0.4,0.9)));

    //Foggy Background
    boundary=std::make_shared<sphere>(point3(0,0,0),5000,glass_mat);
    world.add(std::make_shared<constant_medium>(boundary,0.0001,color(1,1,1)));

    //Earth Sphere
    std::shared_ptr<texture> earth_tex=std::make_shared<image_texture>("./images/earthmap.jpg");
    std::shared_ptr<material> earth_mat=std::make_shared<lambertian>(earth_tex);
    world.add(std::make_shared<translate>(
        std::make_shared<rotate>(
            std::make_shared<sphere>(point3(0,0,0),100,earth_mat),
            0,0,23.5
        ),
        vec3(400,200,400)
    ));
    
    //Perlin Noise Sphere
    std::shared_ptr<texture> noise_tex=std::make_shared<noise_texture>(0.2,4);
    std::shared_ptr<material> noise_mat=std::make_shared<lambertian>(noise_tex);
    world.add(std::make_shared<sphere>(point3(220,280,300),80,noise_mat));

    //Bunch of white spheres
    hittable_list boxes2;
    std::shared_ptr<material> white=std::make_shared<lambertian>(color(0.73,0.73,0.73));
    int total_spheres=1000;
    for(int i=0;i<total_spheres;i++){
        boxes2.add(std::make_shared<sphere>(point3::random(0,165),10,white));
    }

    world.add(std::make_shared<translate>(
        std::make_shared<rotate>(
            std::make_shared<bvh_node>(boxes2),
            15,15,15
        ),
        vec3(-100,270,395)
    ));

    camera cam;

    cam.aspect_ratio=1.0;
    cam.image_width=1080;
    cam.samples_per_pixel=5000;
    cam.max_depth=50;

    cam.background_a=color(0,0,0);
    cam.bg_is_gradient=false;

    cam.vfov=40;
    cam.lookfrom=point3(478,278,-600);
    cam.lookat=point3(278,278,0);
    cam.vup=vec3(0,1,0);

    cam.defocus_angle=0;

    world=hittable_list(std::make_shared<bvh_node>(world));

    cam.render(world,"./renders/all_in.ppm",16);
}