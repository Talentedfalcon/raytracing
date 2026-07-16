#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class quad: public hittable{
    private:
        point3 Q;
        vec3 u, v;
        vec3 w;
        std::shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        double D;
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat){
            this->Q=Q;
            this->u=u;
            this->v=v;
            this->mat=mat;
            
            vec3 n=cross(u,v);
            normal=unit_vector(n);
            D=dot(normal,Q);
            w=n/dot(n,n);
            
            set_bounding_box();
        }

        virtual void set_bounding_box(){
            aabb bbox_diagonal1=aabb(Q,Q+u+v);
            aabb bbox_diagonal2=aabb(Q+u,Q+v);
            bbox=aabb(bbox_diagonal1,bbox_diagonal2);
        }

        aabb bounding_box() const override{
            return bbox;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            //t=(D-n.P)/(n.d)
            double denominator=dot(normal,r.direction());

            if(std::fabs(denominator)<1e-8){
                return false;
            }

            double t=(D-dot(normal,r.origin()))/denominator;
            if(!ray_t.contains(t)){
                return false;
            }

            point3 intersection=r.at(t);
            vec3 planar_hitpt_vector=intersection-Q;
            double alpha=dot(w,cross(planar_hitpt_vector,v));
            double beta=dot(w,cross(u,planar_hitpt_vector));
            
            if(!is_interior(alpha,beta,rec)){
                return false;
            }

            rec.t=t;
            rec.p=intersection;
            rec.mat=mat;
            rec.set_face_normal(r,normal);
            
            return true;
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const{
            interval unit_interval=interval(0,1);
            
            if(!unit_interval.contains(a) || !unit_interval.contains(b)){
                return false;
            }

            rec.u=a;
            rec.v=b;
            return true;
        }
};

#endif