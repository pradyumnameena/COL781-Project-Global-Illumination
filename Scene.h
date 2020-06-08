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
    vec3 ray_tracer(const Ray &r, int depth, unsigned short *xi);
};

#endif;