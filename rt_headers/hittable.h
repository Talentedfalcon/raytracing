#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

class material;

class hit_record{
    public:
        point3 p;
        vec3 normal;
        std::shared_ptr<material> mat;
        double t;
        double u;
        double v;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal){
            front_face=dot(r.direction(),outward_normal)<0;
            normal=front_face?outward_normal:-outward_normal;
        }
};

class hittable{
    public:
        virtual ~hittable() = default;
        
        virtual bool hit(const ray& r,interval ray_t,hit_record& rec) const = 0;

        virtual aabb bounding_box() const = 0;
};

class translate: public hittable{
    private:
        std::shared_ptr<hittable> object;
        vec3 offset;
        aabb bbox;
    public:
        translate(std::shared_ptr<hittable> object, const vec3& offset){
            this->object=object;
            this->offset=offset;
            bbox=object->bounding_box()+offset;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            ray offset_r(r.origin()-offset, r.direction(), r.time());

            if(!object->hit(offset_r,ray_t,rec)){
                return false;
            }

            rec.p+=offset;

            return true;
        }

        aabb bounding_box() const override{
            return bbox;
        }
};

class rotate: public hittable{
    private:
        std::shared_ptr<hittable> object;
        double rad_x;
        double rad_y;
        double rad_z;
        aabb bbox;

        point3 rotate_x(const point3& p, double angle) const{
            double cos_theta=std::cos(angle);
            double sin_theta=std::sin(angle);
            
            double newx=p.x();
            double newy=p.y()*cos_theta-p.z()*sin_theta;
            double newz=p.y()*sin_theta+p.z()*cos_theta;

            return point3(newx,newy,newz);
        }

        point3 rotate_y(const point3& p, double angle) const{
            double cos_theta=std::cos(angle);
            double sin_theta=std::sin(angle);
            
            double newx=p.x()*cos_theta+p.z()*sin_theta;
            double newy=p.y();
            double newz=-p.x()*sin_theta+p.z()*cos_theta;

            return point3(newx,newy,newz);
        }

        point3 rotate_z(const point3& p, double angle) const{
            double cos_theta=std::cos(angle);
            double sin_theta=std::sin(angle);
            
            double newx=p.x()*cos_theta-p.y()*sin_theta;
            double newy=p.x()*sin_theta+p.y()*cos_theta;
            double newz=p.z();

            return point3(newx,newy,newz);
        }
    public:
        rotate(std::shared_ptr<hittable> object, double angle_x=0, double angle_y=0, double angle_z=0){
            this->object=object;

            rad_x=degrees_to_radians(-angle_x);
            rad_y=degrees_to_radians(-angle_y);
            rad_z=degrees_to_radians(-angle_z);

            bbox=object->bounding_box();

            point3 min(infinity,infinity,infinity);
            point3 max(-infinity,-infinity,-infinity);

            for(int i=0;i<2;i++){
                for(int j=0;j<2;j++){
                    for(int k=0;k<2;k++){
                        double x=i*bbox.x.max+(1-i)*bbox.x.min;
                        double y=j*bbox.y.max+(1-j)*bbox.y.min;
                        double z=k*bbox.z.max+(1-k)*bbox.z.min;

                        point3 p=point3(x,y,z);
                        p=rotate_x(p,rad_x);
                        p=rotate_y(p,rad_y);
                        p=rotate_z(p,rad_z);

                        vec3 tester=vec3(p.x(),p.y(),p.z());

                        for(int c=0;c<3;c++){
                            min[c]=std::fmin(min[c],tester[c]);
                            max[c]=std::fmax(max[c],tester[c]);
                        }
                    }
                }
            }
            bbox=aabb(min,max);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            //Transform the ray from world space to object space
            point3 origin=rotate_x(r.origin(),rad_x);
            origin=rotate_y(origin,rad_y);
            origin=rotate_z(origin,rad_z);

            vec3 direction=rotate_x(r.direction(),rad_x);
            direction=rotate_y(direction,rad_y);
            direction=rotate_z(direction,rad_z);

            ray rotated_r(origin,direction,r.time());

            if(!object->hit(rotated_r,ray_t,rec)){
                return false;
            }

            //Transform the intersection from object space to world space
            rec.p=rotate_z(rec.p,-rad_z);
            rec.p=rotate_y(rec.p,-rad_y);
            rec.p=rotate_x(rec.p,-rad_x);

            rec.normal=rotate_z(rec.normal,-rad_z);
            rec.normal=rotate_y(rec.normal,-rad_y);
            rec.normal=rotate_x(rec.normal,-rad_x);

            return true;
        }
        
        aabb bounding_box() const override{
            return bbox;
        }
};

#endif