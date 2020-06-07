#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cmath>

using namespace std;

template <typename T>
class vec3
{
private:
    T x, y, z;

public:
    vec3() : x(0), y(0), z(0) {}
    vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    vec3(T _v) : x(_v), y(_v), z(_v) {}
    vec3 operator+(const vec3 &v) const
    {
        return vec3(x + v.x, y + v.y, z + v.z);
    }
    vec3 operator-(const vec3 &v) const
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }
    vec3 operator*(T val) const
    {
        return vec3(x * val, y * val, z * val);
    }

    T norm() const
    {
        return x * x + y * y + z * z;
    }

    vec3 &normalize() const
    {
        return *this = *this * (1 / sqrt(x * x + y * y + z * z));
    }

    T dot(const vec3 &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    vec3 cross(const vec3 &v) const
    {
        return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

class ray{
private:
    vec3 o, d;

public:
    ray(vec o, vec d) : o(o), d(d){}
}

enum Refl_t{ DIFF, SPEC, REFR };

class sphere{
private:
    vec3 p, e, c;
    double rad;
    Refl_t refl;

public:
    sphere(double rad_, vec p_, vec e_, vec c_, Refl_t refl_): rad(rad_), p(p_), e(e_), c(c_) , refl(refl_) {}

    double intersect(const ray &r) const
    {
        vec op = p - r.o;
        double t;
        double eps = 1e-4;
        double b = op.dot(r.d);
        double det = b*b - op.dot(op) + rad*rad;

        if(det<0){
            return 0;
        }else{
            det = sqrt(det);
        }

        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
    }
}

#endif;