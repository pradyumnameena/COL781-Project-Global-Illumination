#ifndef RAY_H
#define RAY_H

#include "Math.h"

class Ray
{
public:
    vec3d origin, direction;
    Ray(vec3d o, vec3d d) : origin(o), direction(d) {}
};

enum Reflection_t
{
    DIFFUSE,
    SPECULAR,
    REFRACTION
};

#endif