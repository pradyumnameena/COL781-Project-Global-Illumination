#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include "Math.h"
#include "Ray.h"
#include <vector>

class Scene
{
public:
    Scene();
    Scene(Sphere[], int);
    std::vector<Sphere> scene_obj;
    vec3d ray_tracer(const Ray &, int, unsigned short *);
    bool intersect(const Ray &, double &, int &);
};

#endif