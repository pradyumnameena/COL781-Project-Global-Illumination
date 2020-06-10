#ifndef KDTREE_H
#define KDTREE_H

#include <algorithm>
#include "Math.h"
#include "Photon.h"

class KDTree
{
public:
	Photon **photons;
	char *nodes;
	unsigned total;

	KDTree();
	int sepaxis(Photon **photons, unsigned n);
	static bool cmpx(const Photon *a, const Photon *b);
	static bool cmpy(const Photon *a, const Photon *b);
	static bool cmpz(const Photon *a, const Photon *b);

	void build(Photon **buf, unsigned total_);
	void nearest(Photon **buf, double *dist, int n, const vec3d &x, double d2);
	void sep(Photon **buf, unsigned i, unsigned n);
	void trav(Photon **buf, double *dist, int n, const vec3d &x, unsigned i);
};

#endif