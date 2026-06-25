#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material{
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const{
            return false;
        }
};

class lambertian: public material{
    private:
        color albedo;
        
    public:
        lambertian(const color& albedo){
            this->albedo=albedo;
        }

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            vec3 scatter_direction=rec.normal+random_unit_vector();
            if(scatter_direction.near_zero()){
                scatter_direction=rec.normal;
            }
            
            scattered=ray(rec.p,scatter_direction);
            attenuation=albedo;
            return true;
        }
};

class metal: public material{
    private:
        color albedo;
        double fuzz;
        
    public:
        metal(const color& albedo, double fuzz){
            this->albedo=albedo;
            this->fuzz=fuzz<1?fuzz:1;
        }

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            vec3 reflected=reflect(r_in.direction(),rec.normal);
            reflected=unit_vector(reflected)+(fuzz*random_unit_vector());
            scattered=ray(rec.p,reflected);
            attenuation=albedo;
            return (dot(scattered.direction(),rec.normal)>0);
        }
};

#endif