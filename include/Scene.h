#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include "Math.h"
#include "Ray.h"
#include "Photon.h"
#include "KDTree.h"
#include <vector>

class Scene
{
public:
    Scene();
    Scene(Sphere[], int, vec3d &, vec3d &);
    vec3d ray_tracer(const Ray &, int, unsigned short *);
    vec3d photon_tracer(const Ray &, int, bool, const vec3d &, unsigned short *);
    bool intersect(const Ray &, double &, int &);
    vec3d generateRandomDirection(vec3d &, unsigned short *Xi);
    void generatePhotonRay(Ray *, vec3d *, unsigned short *Xi);
    std::vector<Sphere> scene_obj;
    std::vector<Photon *> photons;
    vec3d lightPos, lightPow;
    KDTree tree;
    int numPhotons;
};

#endif