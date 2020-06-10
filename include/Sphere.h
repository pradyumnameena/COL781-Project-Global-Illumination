#ifndef SPHERE_H

#define SPHERE_H
#include "Math.h"
#include "Ray.h"
#include "Object.h"

class Sphere
{
public:
    double radius, radius2;
    vec3d position, emission, color;
    Reflection_t type;
    Sphere();
    Sphere(const double &, const vec3d &, const vec3d &, const vec3d &, const Reflection_t &);
    double intersect(const Ray &) const;
};

#endif