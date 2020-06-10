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

// struct AABB
// {
//     vec3d l, u;
//     AABB() : l(1e20, 1e20, 1e20), u(-1e20, -1e20, -1e20) {}
//     void fit(const vec3d &p)
//     {
//         l.x = std ::min(p.x, l.x);
//         l.y = std::min(p.y, l.y);
//         l.z = std::min(p.z, l.z);
//         u.x = std::max(p.x, u.x);
//         u.y = std::max(p.y, u.y);
//         u.z = std::max(p.z, u.z);
//     }
// };

#endif