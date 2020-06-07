#ifndef OBJECT_H

#define OBJECT_H

#include <cmath>

class Object
{
public:
    Object();
    virtual ~Object();
    virtual bool intersect();
    double radius;
};

#endif