#ifndef OBJECT_H

#define OBJECT_H
#include "Math.h"
#include "Ray.h"

class Object
{
public:
    Object(){};
    virtual ~Object();
    virtual bool intersect() const = 0;
    Reflection_t type;
    vec3f position, emission, color;
};

#endif