#ifndef SPHERE_H

#define SPHERE_H
#include "Math.h"
#include "Ray.h"
#include "Object.h"

class Sphere : public Object
{
public:
    double radius, radius2;
    Sphere(const double &rad, const vec3f &p, const vec3f &e, const vec3f &c) : Object(), radius(rad), radius2(rad * rad)
    {
        position = p;
        emission = e;
        color = c;
    }
};

#endif