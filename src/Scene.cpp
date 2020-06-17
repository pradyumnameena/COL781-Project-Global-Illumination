#include <stdlib.h>
#include "./../include/Scene.h"
#include "./../include/Math.h"
#include "./../include/Sphere.h"
#include "./../include/Ray.h"
#include "./../include/KDTree.h"
#include <iostream>

Scene::Scene() : tree()
{
	scene_obj.resize(0);
	generatePrimes(500);
}

Scene::Scene(Sphere sp[], int n, vec3d pos, vec3d pow, int estimate = 0) : tree()
{
	scene_obj.resize(n);
	for (int i = 0; i < n; i++)
	{
		scene_obj[i] = sp[i];
	}
	lightPos = pos;
	lightPow = pow;
	numPhotons = estimate;
	generatePrimes(500);
}

void Scene::generatePrimes(int n)
{
	bool prime[n+1];
	primes.resize(0);
	for(int i = 0;i<=n;i++){
		prime[i] = true;
	}

	for(int p = 2;p*p<=n;p++){
		if(prime[p]==true){
			for(int j = p*p;j<=n;j+=p){
				prime[j] = false;
			}
		}
	}

	for(int p = 2;p<=n;p++){
		if(prime[p]){
			primes.push_back(p);
		}
	}
}

void Scene::addSnowman(const Snowman &s)
{
	scene_obj.resize(scene_obj.size() + 2);
	scene_obj[scene_obj.size() - 2] = s.lower;
	scene_obj[scene_obj.size() - 1] = s.upper;
}

vec3d Scene::ray_tracer(const Ray &r, int depth, unsigned short *Xi)
{
	int id = 0;
	double t;
	// cout << "Before for loop" << endl;
	if (!intersect(r, t, id))
		return vec3d();

	vec3d x = r.origin + r.direction * t;
	const Sphere &obj = scene_obj[id];
	vec3d n = (x - obj.position).normalize();
	vec3d f = obj.color;
	vec3d n1 = n;

	if (n.dot(r.direction) >= 0)
	{
		n1 = n * -1;
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
		vec3d w = n1;
		double r1, r2, r2s;
		r1 = 2 * M_PI * erand48(Xi);
		r2 = erand48(Xi);
		r2s = sqrt(r2);

		vec3d u, v;

		if (fabs(w.x) > 0.1)
		{
			// May be wrong. Need to be checked
			u = vec3d(0, 1);
		}
		else
		{
			// May be wrong. Need to be checked
			u = vec3d(1);
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

	bool into = n.dot(n1) > 0;
	double nc, nt, nnt, ddn, cos2t;
	int temp;
	nc = 1;
	nt = 1.5;
	ddn = r.direction.dot(n1);
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
	double inf = t = 1e20;
	int n = scene_obj.size();
	double d;
	for (int i = n; i--;)
	{
		d = scene_obj[i].intersect(r);
		if (d && d < t)
		{
			t = d;
			id = i;
		}
	}
	return t < inf;
}

void Scene::generatePhotonRay(Ray *r, vec3d *f, int i)
{
	*f = lightPow;
	r->origin = lightPos;
	double p = 2 * M_PI * hal(0, i);
	double t = 2 * acos(sqrt(1.0 - hal(1, i)));
	double st = sin(t);
	r->direction = vec3d(cos(p) * st, cos(t), sin(p) * st);
}

vec3d Scene::photon_tracer(const Ray &r, int depth, bool m, const vec3d &fl, int i)
{
	double t;
	int id = 0;
	int d3 = depth * 3;
	// cout << "Before for loop" << endl;
	if ((++depth * 3) >= 20 || !intersect(r, t, id))
		return vec3d();

	const Sphere &obj = scene_obj[id];
	vec3d x = r.origin + r.direction * t;
	vec3d n = (x - obj.position).normalize();
	const vec3d &f = obj.color;
	vec3d n1 = n;

	if (n.dot(r.direction) >= 0)
	{
		n1 = n * -1;
	}

	if (obj.type == DIFFUSE)
	{
		// std::cout << "Diffuse" << std::endl;
		if (m)
		{
			Photon **b = (Photon **)alloca(sizeof(Photon *) * numPhotons);
			double *r = (double *)alloca(sizeof(double) * numPhotons);
			tree.nearest(b, r, numPhotons, x, infinity);
			vec3d flux(0);
			const double fr = 1.0 / M_PI;
			int j, k;
			for (j = k = 0; j < numPhotons; k = j++)
			{
				Photon *ph = b[j];
				if (!ph)
				{
					break;
				}
				flux = flux + ph->fl * fr;
			}
			// cout << "FLUX " << flux.x << " " << flux.y << " " << flux.z << endl;
			return flux.mult(f) * (1.0 / (M_PI * r[k]));
		}
		Photon *q = new Photon(x, fl);
		photons.push_back(q);

		double p = max(f.x, max(f.y, f.z));
		if (hal(d3 + 1, i) >= p)
		{
			return vec3d();
		}
		vec3d &w = n1;

		double r1, r2, r2s;
		r1 = 2. * M_PI * hal(d3 - 1, i);
		r2 = hal(d3 + 0, i);
		r2s = sqrt(r2);

		vec3d u, v;

		if (fabs(w.x) > 0.1)
		{
			// May be wrong. Need to be checked
			u = vec3d(0, 1);
		}
		else
		{
			// May be wrong. Need to be checked
			u = vec3d(1);
		}

		u = (u.cross(w)).normalize();
		v = w.cross(u);
		vec3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
		// cout << "d: " << d.x << " " << d.y << " " << d.z << endl;
		return photon_tracer(Ray(x, d), depth, m, f.mult(fl) * (1.0 / p), i);
	}
	else if (obj.type == SPECULAR)
	{
		// std::cout << "Specular" << std::endl;
		Ray r3(x, r.direction - n * 2 * n.dot(r.direction));
		return photon_tracer(r3, depth, m, f.mult(fl), i).mult(f);
	}

	// std::cout << "Refraction" << std::endl;
	Ray reflectedRay(x, r.direction - n * 2 * n.dot(r.direction));

	bool into = n.dot(n1) > 0;
	double nc, nt, nnt, ddn, cos2t;
	int temp;
	nc = 1;
	nt = 1.5;
	ddn = r.direction.dot(n1);
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
		return photon_tracer(reflectedRay, depth, m, fl, i);
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

	double Re, Tr, P;
	Re = R0 + (1 - R0) * c * c * c * c * c;
	Tr = 1 - Re;
	P = Re;

	if (m)
	{
		return f.mult(photon_tracer(reflectedRay, depth, m, fl, i) * Re + photon_tracer(Ray(x, tdir), depth, m, fl, i) * Tr);
	}
	else
	{
		if (hal(d3 - 1, i) < P)
		{
			return photon_tracer(reflectedRay, depth, m, f.mult(fl), i);
		}
		else
		{
			return photon_tracer(Ray(x, tdir), depth, m, f.mult(fl), i);
		}
	}
}