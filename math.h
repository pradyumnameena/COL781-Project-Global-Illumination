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
};

#endif;