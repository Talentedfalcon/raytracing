#include "raytrace.h"
#include "camera.h"

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main(){
    hittable_list world;

    std::shared_ptr<lambertian> mat_ground=std::make_shared<lambertian>(color(0.5,0.8,0.3));
    std::shared_ptr<lambertian> mat_diffuse=std::make_shared<lambertian>(color(0.8,0.8,0.8));

    world.add(std::make_shared<sphere>(point3(0,0,-1),0.5,mat_diffuse));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1),100,mat_ground));

    camera cam;

    cam.aspect_ratio=16.0/9.0;
    cam.image_width=1920;
    cam.samples_per_pixel=100;
    cam.max_depth=50;

    cam.render(world,"temp.ppm");

    return 0;
}