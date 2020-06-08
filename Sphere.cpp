#include "Sphere.h"

Sphere::Sphere(const double &rad, const vec3d &p, const vec3d &e, const vec3d &c, const Reflection_t &t) : Object(), radius(rad), radius2(rad * rad)
{
    position = p;
    emission = e;
    color = c;
}

double Sphere::intersect(const Ray &r) const
{
    vec3d op = position - r.origin;
    double b = op.dot(r.direction);
    double D = b * b - op.norm() + radius2;
    // Solving the equation  t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    if (D < 0)
        return 0;
    else
        D = sqrt(D);

    if ((b - D) > epsilon)
    {
        return b - D;
    }
    else if ((b + D) > epsilon)
    {
        return b + D;
    }
    else
        return 0;
}