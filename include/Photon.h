#ifndef PHOTON_H
#define PHOTON_H

#include "Math.h"
#include <algorithm>

class Photon
{
public:
    vec3d x, fl;
    Photon();
    Photon(const vec3d &_x, const vec3d &_fl) : x(_x), fl(_fl) {}
};

#endif