#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include "Math.h"
#include "Ray.h"
#include <vector>

using namespace std;

class Scene
{
public:
    Scene();
    vector<Sphere> scene_obj;
    vec3d ray_tracer(const Ray &, int, unsigned short *);
};

#endif