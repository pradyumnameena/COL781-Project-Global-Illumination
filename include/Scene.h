#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include "Math.h"
#include "Ray.h"
#include "Photon.h"
#include "Snowman.h"
#include "KDTree.h"
#include <vector>

class Scene
{
public:
    Scene();
    Scene(Sphere[], int, vec3d, vec3d, int);
    void generatePrimes(int);
    void addSnowman(const Snowman &);
    vec3d ray_tracer(const Ray &, int, unsigned short *);
    vec3d photon_tracer(const Ray &, int, bool, const vec3d &, int);
    bool intersect(const Ray &, double &, int &);
    vec3d generateRandomDirection(vec3d &, unsigned short *Xi);
    void generatePhotonRay(Ray *, vec3d *, int i);
    std::vector<Sphere> scene_obj;
    std::vector<Photon *> photons;
    std::vector<int> primes;
    vec3d lightPos, lightPow;
    KDTree tree;
    int numPhotons;
    
    inline int rev(const int p, const int i)
    {
        return (i == 0) ? i : p - i;
    }

    double hal(const int b, int j)
    { // Halton sequence with reverse permutation
        const int p = primes[b];
        double h = 0.0;
        double f = 1.0 / (double)p;
        double fct = f;
        while (j > 0)
        {
            h += rev(p, j % p) * fct;
            j /= p;
            fct *= f;
        }
        return h;
    }
};

#endif