#ifndef RAY_H
#define RAY_H

#include "math.h"

class Ray
{
public:
    vec3f origin, direction;
    Ray(vec3f o, vec3f d) : origin(o), direction(d) {}
};

enum Reflection_t
{
    DIFFUSE,
    SPECULAR,
    REFRACTION
};

#endif;