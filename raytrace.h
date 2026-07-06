#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

const double infinity=std::numeric_limits<double>::infinity();
const double pi=3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees*pi/180.0;
}

inline double random_double(){
    return std::rand()/(RAND_MAX+1.0);
}

inline double random_double(double min, double max){
    return min+(max-min)*random_double();
}

inline int get_num_digits(int n){
    int digits=0;
    while(n){
        digits+=1;
        n/=10;
    }
    return digits;
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif