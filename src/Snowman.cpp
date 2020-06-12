#include <algorithm>
#include "./../include/Snowman.h"
#include "./../include/Sphere.h"

Snowman::Snowman()
{
    // do nothing
}

Snowman::Snowman(const double &r1, const vec3d &p1, const vec3d &e1, const vec3d &c1, const Reflection_t &t1, const double &r2, const vec3d &p2, const vec3d &e2, const vec3d &c2, const Reflection_t &t2)
{
    lower = Sphere(r1,p1,e1,c1,t1);
    upper = Sphere(r2,p2,e2,c2,t2);
}

double Snowman::intersect(const Ray &r) const
{
    double t1 = lower.intersect(r);
    double t2 = upper.intersect(r);

    if(t1==0 && t2==0){
        return 0;
    }else{
        return std::min(t1,t2);
    }
}