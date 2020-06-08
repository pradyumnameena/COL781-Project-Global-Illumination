#ifndef OBJECT_H

#define OBJECT_H
#include "Math.h"
#include "Ray.h"

class Object
{
public:
    Object(){};
    virtual ~Object();
    virtual double intersect();
    Reflection_t type;
    vec3d position, emission, color;
};

#endif