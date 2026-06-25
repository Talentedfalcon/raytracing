#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere: public hittable{
    private:
        point3 center;
        double radius;
        std::shared_ptr<material> mat;
    public:
        sphere(const point3& center, double radius, std::shared_ptr<material> mat){
            this->center=center;
            this->radius=std::fmax(0,radius);
            this->mat=mat;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            vec3 oc=center-r.origin();
            double a=r.direction().length_squared();
            double b=-2.0*dot(r.direction(),oc);
            double c=oc.length_squared()-radius*radius;
            double discriminant=b*b-4*a*c;

            if(discriminant<0){
                return false;
            }
            double sqrtd=std::sqrt(discriminant);

            double root=(-b-sqrtd)/(2.0*a);
            if(!ray_t.surrounds(root)){
                root=(-b+sqrtd)/(2.0*a);
                if(!ray_t.surrounds(root)){
                    return false;
                }
            }
            rec.t=root;
            rec.p=r.at(rec.t);
            vec3 outward_normal=(rec.p-center)/radius;
            rec.set_face_normal(r,outward_normal);
            rec.mat=mat;

            return true;
        }
};

#endif