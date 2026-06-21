#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color=vec3;

void write_color(std::ostream& out, const color& pixel_color){
    double r=pixel_color.x();
    double g=pixel_color.y();
    double b=pixel_color.z();

    int ir=int(255*r);
    int ig=int(255*g);
    int ib=int(255*b);

    out.put(ir);
    out.put(ig);
    out.put(ib);
}

#endif