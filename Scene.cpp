#include <stdlib.h>
#include "Scene.h"
#include "Math.h"
#include "Sphere.h"
#include "Ray.h"

Scene::Scene()
{
	scene_obj.resize(0);
}

Scene::Scene(Sphere sp[], int n)
{
	std::copy(sp, sp + n, scene_obj.begin());
}

vec3d Scene::ray_tracer(const Ray &r, int depth, unsigned short *Xi)
{
	double d, t;
	double inf = 1e20;

	int id = 0;
	int len = scene_obj.size();

	for (int i = len; i--;)
	{
		d = scene_obj[i].intersect(r);
		if (d < t)
		{
			t = d;
			id = i;
		}
	}

	if (d >= inf)
	{
		return vec3d();
	}

	const Sphere &obj = scene_obj[id];
	vec3d x = r.origin + r.direction * t;
	vec3d n = (x - obj.position).normalize();
	vec3d f = obj.color;
	vec3d nl = n;

	if (n.dot(r.direction) >= 0)
	{
		nl = n * -1;
	}

	double p = max(f.x, max(f.y, f.z));

	if (++depth > 5)
	{
		if (erand48(Xi) < p)
			f = f * (1 / p);
		else
			return obj.emission;
	}

	if (obj.type == DIFFUSE)
	{
		double r1, r2, r2s;
		r1 = 2 * M_PI * erand48(Xi);
		r2 = erand48(Xi);
		r2s = sqrt(r2);

		vec3d w = nl;
		vec3d u, v;

		if (fabs(w.x) > 0.1)
		{
			// May be wrong. Need to be checked
			u = vec3d(0, 1, 0);
		}
		else
		{
			// May be wrong. Need to be checked
			u = vec3d(1, 0, 0);
		}

		u = (u.cross(w)).normalize();
		v = w.cross(u);
		vec3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

		return obj.emission + f.mult(ray_tracer(Ray(x, d), depth, Xi));
	}
	else if (obj.type == SPECULAR)
	{
		Ray r3(x, r.direction - n * 2 * n.dot(r.direction));
		return obj.emission + f.mult(ray_tracer(r3, depth, Xi));
	}

	Ray reflectedRay(x, r.direction - n * 2 * n.dot(r.direction));

	bool into = n.dot(nl) > 0;
	double nc, nt, nnt, ddn, cos2t;
	int temp;
	nc = 1;
	nt = 1.5;
	ddn = r.direction.dot(nl);
	nnt = nt / nc;
	temp = -1;
	if (into)
	{
		nnt = nc / nt;
		temp = 1;
	}

	cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
	if (cos2t < 0)
	{
		return obj.emission + f.mult(ray_tracer(reflectedRay, depth, Xi));
	}

	vec3d tdir = (r.direction * nnt - n * temp * (ddn * nnt + sqrt(cos2t))).normalize();

	double a, b, R0, c;
	a = nt - nc;
	b = nt + nc;
	R0 = a * a / (b * b);
	c = 1 - tdir.dot(n);
	if (into)
	{
		c = 1 + ddn;
	}

	double Re, Tr, P, RP, TP;
	Re = R0 + (1 - R0) * c * c * c * c * c;
	Tr = 1 - Re;
	P = 0.25 + 0.5 * Re;
	RP = Re / P;
	TP = Tr / (1 - P);

	vec3d slack;
	if (depth > 2)
	{
		if (erand48(Xi) < P)
		{
			slack = ray_tracer(reflectedRay, depth, Xi) * RP;
		}
		else
		{
			slack = ray_tracer(Ray(x, tdir), depth, Xi) * TP;
		}
	}
	else
	{
		slack = ray_tracer(reflectedRay, depth, Xi) * Re + ray_tracer(Ray(x, tdir), depth, Xi) * Tr;
	}

	return obj.emission + f.mult(slack);
}

bool Scene::intersect(const Ray &r, double &t, int &id)
{
	double t = infinity;
	int n = scene_obj.size();
	double d;
	for (int i = n - 1; i--;)
	{
		d = scene_obj[i].intersect(r);
		if (d && d < t)
		{
			t = d;
			id = i;
		}
	}
	return t < infinity;
}