#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtw_stb_image.h"
#include "perlin.h"
#include <vector>

class texture{
    public:
        virtual ~texture()=default;
        virtual color value(double u, double v, const point3& p) const=0;
};

class solid_color: public texture{
    private:
        color albedo;
    public:
        solid_color(const color& albedo){
            this->albedo=albedo;
        }
        solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)){}

        color value(double u, double v, const point3& p) const override{
            return albedo;
        }
};

class checker_texture: public texture{
    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
    public:
        checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd){
            inv_scale=1.0/scale;
            this->even=even;
            this->odd=odd;
        }
        checker_texture(double scale, const color& c1, const color& c2): checker_texture(scale,std::make_shared<solid_color>(c1),std::make_shared<solid_color>(c2)){}

        color value(double u, double v, const point3& p) const override{
            int x=int(std::floor(inv_scale*p.x()));
            int y=int(std::floor(inv_scale*p.y()));
            int z=int(std::floor(inv_scale*p.z()));

            bool isEven=(x+y+z)%2==0;
            return isEven?even->value(u,v,p):odd->value(u,v,p);
        }
};

class image_texture: public texture{
    private:
        rtw_image image;
    public:
        image_texture(const char* filename) : image(filename){}

        color value(double u, double v, const point3& p) const override{
            if(image.height()<=0){
                return color(0,1,1);
            }

            u=interval(0,1).clamp(u);
            v=1.0-interval(0,1).clamp(v);

            int i=int(u*image.width());
            int j=int(v*image.height());
            const unsigned char* pixel=image.pixel_data(i,j);

            double color_scale=1.0/255.0;
            return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }
};

class noise_texture: public texture{
    private:
        perlin noise;
        double scale;
        double turbulence;
        color color_a=color(1,1,1);
        color color_b=color(0,0,0);
    public:
        noise_texture(double scale, double turbulence=0){
            this->scale=scale;
            this->turbulence=turbulence;
        }
        noise_texture(double scale, const color& color_a, const color& color_b, double turbulence=0){
            this->scale=scale;
            this->turbulence=turbulence;
            this->color_a=color_a;
            this->color_b=color_b;
        }
        
        color value(double u, double v, const point3& p) const override{
            double multiplier=0.0;
            if(turbulence){
                multiplier=(1+std::sin(scale*p.z()+10*noise.turb(p,turbulence)));
            }
            else{
                multiplier=0.5*(1.0+noise.noise(scale*p));
            }
            return color_a*multiplier+color_b*(1.0-multiplier);
        }
};

class linear_gradient_texture: public texture{
    private:
        std::vector<color> colors;
        double angle;
    public:
        linear_gradient_texture(const color& color_a, const color& color_b, double angle=0){
            this->colors.push_back(color_a);
            this->colors.push_back(color_b);
            this->angle=angle*pi/180;
        }
        linear_gradient_texture(const std::vector<color>& colors, double angle=0){
            this->colors=colors;
            this->angle=angle*pi/180;
        }

        color value(double u, double v, const point3& p) const override{
            v-=0.5;
            u-=0.5;
            double scale=abs(cos(angle))+abs(sin(angle));
            double angled=(v*cos(angle)+u*sin(angle))/scale;
            angled+=0.5;

            double spacing=1.0/(colors.size()-1);
            double start_idx=0;
            double end_idx=spacing;
            for(int i=0;i<colors.size()-1;i++){
                if(angled>=start_idx && angled<=end_idx){
                    angled=(angled-start_idx)/(end_idx-start_idx);
                    return colors[i+1]*angled+(1-angled)*colors[i];
                }
                start_idx=end_idx;
                end_idx+=spacing;
            }
            
            return color();
        }
};

#endif