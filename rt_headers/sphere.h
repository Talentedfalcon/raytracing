#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere: public hittable{
    private:
        ray center;
        double radius;
        std::shared_ptr<material> mat;
        aabb bbox;

        static void get_sphere_uv(const point3& p, double& u, double& v){
            double theta=std::acos(-p.y());
            double phi=std::atan2(-p.z(),p.x())+pi;
            u=phi/(2*pi);
            v=theta/pi;
        }
    public:
        // Stationary Sphere
        sphere(const point3& center, double radius, std::shared_ptr<material> mat){
            this->center=ray(center,vec3(0,0,0));
            this->radius=std::fmax(0,radius);
            this->mat=mat;

            vec3 rvec=vec3(radius,radius,radius);
            bbox=aabb(center-rvec,center+rvec);
        }
        // Moving Sphere
        sphere(const point3& center1, const point3& center2, double radius, std::shared_ptr<material> mat){
            this->center=ray(center1,center2-center1);
            this->radius=std::fmax(0,radius);
            this->mat=mat;

            vec3 rvec=vec3(radius,radius,radius);
            aabb box1(this->center.at(0)-rvec,this->center.at(0)+rvec);
            aabb box2(this->center.at(1)-rvec,this->center.at(1)+rvec);
            bbox=aabb(box1,box2);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            point3 curr_center=center.at(r.time());
            vec3 oc=curr_center-r.origin();
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
            vec3 outward_normal=(rec.p-curr_center)/radius;
            rec.set_face_normal(r,outward_normal);
            get_sphere_uv(outward_normal,rec.u,rec.v);
            rec.mat=mat;

            return true;
        }

        aabb bounding_box() const override{
            return bbox;
        }
};

#endif