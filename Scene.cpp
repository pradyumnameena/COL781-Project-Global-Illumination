#include <stdlib.h>
#include "Scene.h"
// #include "Sphere.h"
// #include "Math.h"

Scene::Scene()
{
	// TO-DO
	// not including anything now
	// Add method to include spheres from file
}

vec3 Scene::ray_tracer(const Ray &r, int depth, unsigned short *Xi){
	double d,t;
	double inf = 1e20;

	int id = 0;
	int n = scene_obj.size();

	for(int i = n;i--;){
		if(d=scene_obj[i].intersect(r) && d<t){
			t = d;
			id = i;
		}
	}

	if(d>=inf){
		return vec3();
	}

	const Sphere &obj = scene_obj[id];
	vec3 x = r.origin + r.direction*t;
	vec3 n = (x - obj.position).normalize();
	vec3 f = obj.color;
	vec3 nl = n;

	if(n.dot(r.direction)>=0){
		nl = n*-1;
	}

	double p = max(f.x, max(f,y,f.z));

	if(++depth>5){
		// TO-Do if statement
	}

	if(obj.type == DIFF){
		double r1,r2,r2s;
		r1 = 2 * M_PI * erand48(Xi);
		r2 = erand48(Xi);
		r2s = sqrt(r2);

		vec3 w = nl;
		vec3 u;
		
		if(fabs(w.x)>0.1){
			u = vec3(0,1);
		}else{
			u = vec3(1);
		}

		u = (u%w).normalize();
		v = w%u;
		vec3 d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).normalize();

		Ray r2(x,d);
		return obj.emission + f.mult(ray_tracer(r2,depth,Xi));
	}else if(obj.type == SPEC){
		Ray r3(x, r.direction - n*2*n.dot(r.direction));
		return obj.emission + f.mult(ray_tracer(r3,depth,Xi));
	}

	Ray reflectedRay(x, r.direction - n*2*n.dot(r.direction));

	bool into = n.dot(nl)>0;
	double nc,nt,nnt,ddn,cos2t
	int temp;
	nc = 1;
	nt = 1.5;
	ddn = r.direction.dot(nl);
	nnt = nt/nc;
	temp = -1;
	if(into){
		nnt = nc/nt;
		temp = 1;
	}

	cos2t = 1 - nnt*nnt*(1 - ddn*ddn);
	if(cos2t<0){
		return obj.emission + f.mult(ray_tracer(reflectedRay,depth,Xi));
	}

	vec3 tdir = (r.direction*nnt - n*temp*(ddn*nnt + sqrt(cos2t))).normalize();

	double a,b,R0,c;
	a = nt - nc;
	b = nt + nc;
	R0 = a*a/(b*b);
	c = 1 - tdir.dot(n);
	if(into){
		c = 1 + ddn;
	}

	double Re,Tr,P,RP,TP;
	Re = R0 + (1-R0)*c*c*c*c*c;
	Tr = 1 - Re;
	P = 0.25 + 0.5*Re;
	RP = Re/P;
	TP = Tr/(1-P);

	vec3 slack;
	if(depth>2){
		if(erand48(Xi)<P){
			slack = ray_tracer(reflectedRay,depth,Xi)*RP;
		}else{
			slack = ray_tracer(Ray(x,tdir),depth,Xi)*TP;
		}
	}else{
		slack = ray_tracer(reflectedRay,depth,Xi)*Re + ray_tracer(Ray(x,tdir),depth,Xi)*Tr;
	}

	return obj.emission + f.mult(slack);
}