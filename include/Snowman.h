#ifndef SNOWMAN_H
#define SNOWMAN_H

#include "Ray.h"
#include "Math.h"
#include "Sphere.h"

class Snowman
{
public:
	Sphere lower, upper;
	Snowman();
	Snowman(const double &, const vec3d &, const vec3d &, const vec3d &, const Reflection_t &, const double &, const vec3d &, const vec3d &, const vec3d &, const Reflection_t &);
	double intersect(const Ray &) const;
};

#endif