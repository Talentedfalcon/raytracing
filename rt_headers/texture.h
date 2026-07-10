#ifndef TEXTURE_H
#define TEXTURE_H

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

#endif