#ifndef SPHERE_H

#define SPHERE_H
#include "Math.h"
#include "Ray.h"
#include "Object.h"

class Sphere : public Object
{
public:
    double radius, radius2;
    Sphere(const double &, const vec3d &, const vec3d &, const vec3d &, const Reflection_t &);
    double intersect(const Ray &) const;
};

#endif