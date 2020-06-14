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
    // Scene(string, vec3d, vec3d, int);
    void addSnowman(const Snowman &);
    vec3d ray_tracer(const Ray &, int, unsigned short *);
    vec3d photon_tracer(const Ray &, int, bool, const vec3d &, int);
    bool intersect(const Ray &, double &, int &);
    vec3d generateRandomDirection(vec3d &, unsigned short *Xi);
    void generatePhotonRay(Ray *, vec3d *, int i);
    std::vector<Sphere> scene_obj;
    std::vector<Photon *> photons;
    vec3d lightPos, lightPow;
    KDTree tree;
    int numPhotons;
    int primes[61] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
                      83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                      191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283};

    inline int rev(const int i, const int p)
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
            h += rev(j % p, p) * fct;
            j /= p;
            fct *= f;
        }
        return h;
    }
};

#endif