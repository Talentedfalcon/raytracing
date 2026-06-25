#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color=vec3;

inline double linear_to_gamma(double linear_component){
    if(linear_component>0){
        return std::sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream& out, const color& pixel_color){
    double r=pixel_color.x();
    double g=pixel_color.y();
    double b=pixel_color.z();

    r=linear_to_gamma(r);
    g=linear_to_gamma(g);
    b=linear_to_gamma(b);

    static const interval intensity(0,1);
    int ir=int(255*intensity.clamp(r));
    int ig=int(255*intensity.clamp(g));
    int ib=int(255*intensity.clamp(b));

    out.put(ir);
    out.put(ig);
    out.put(ib);
}

#endif