#include <stdlib.h>
#include <algorithm>
#include "./../include/KDTree.h"
#include "./../include/Photon.h"
#include "./../include/Math.h"

KDTree::KDTree()
{
	// do nothing
	// Photon** photons;
	// char* nodes;
	// unsigned total;
}

bool KDTree::cmpx(const Photon *a, const Photon *b)
{
	return a->x.x < b->x.x;
}

bool KDTree::cmpy(const Photon *a, const Photon *b)
{
	return a->x.y < b->x.y;
}

bool KDTree::cmpz(const Photon *a, const Photon *b)
{
	return a->x.z < b->x.z;
}

int KDTree::sepaxis(Photon **photons, unsigned n)
{
	vec3d l(1e20, 1e20, 1e20);
	vec3d u(-1e20, -1e20, -1e20);

	for (unsigned i = 0; i < n; ++i)
	{
		l.x = std::min(photons[i]->x.x, l.x);
		l.y = std::min(photons[i]->x.y, l.y);
		l.z = std::min(photons[i]->x.z, l.z);
		u.x = std::max(photons[i]->x.x, u.x);
		u.y = std::max(photons[i]->x.y, u.y);
		u.z = std::max(photons[i]->x.z, u.z);
	}

	vec3d w = u - l;

	if (w.x > w.y)
	{
		if (w.x > w.z)
		{
			return 0;
		}
		else if (w.y > w.z)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if (w.y > w.z)
		{
			return 1;
		}
		else if (w.x > w.z)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
}

void KDTree::build(Photon **buf, unsigned total_)
{
	total = total_;
	nodes = new char[total / 2];
	photons = new Photon *[total];
	sep(buf, 0, total);
}

void KDTree::nearest(Photon **buf, double *dist, int n, const vec3d &x, double d2)
{
	// Check for start and end conditions if error occurs
	int i = 0;
	while(i<n){
		buf[i] = NULL;
		dist[i] = d2;
		i++;
	}
	// for (int i = 0; i < n; ++i)
	// {
	// 	buf[i] = NULL;
	// 	dist[i] = d2;
	// }
	trav(buf, dist, n, x, 0);
}

void KDTree::sep(Photon **buf, unsigned i, unsigned n)
{
	if (n > 1)
	{
		int axis = sepaxis(buf, n);
		if (axis == 0)
		{
			std::sort(buf, buf + n, cmpx);
		}
		else if (axis == 1)
		{
			std::sort(buf, buf + n, cmpy);
		}
		else
		{
			std::sort(buf, buf + n, cmpz);
		}

		int m = median(n);
		nodes[i] = axis;
		photons[i] = buf[m];

		sep(buf, i * 2 + 1, m);
		sep(buf + m + 1, i * 2 + 2, n - m - 1);
	}
	else if (n == 1)
	{
		photons[i] = *buf;
	}
}

void KDTree::trav(Photon **buf, double *dist, int n, const vec3d &x, unsigned i)
{
	if (i * 2 + 1 < total)
	{
		int axis = nodes[i];
		vec3d v = photons[i]->x;
		double *pd2 = &dist[n - 1];
		double e = 0;

		if (axis == 0)
		{
			e = x.x - v.x;
		}
		else if (axis == 1)
		{
			e = x.y - v.y;
		}
		else
		{
			e = x.z - v.z;
		}

		bool allow_condition = (e*e < *(pd2));
		
		if (e >= 0)
		{	
			trav(buf, dist, n, x, i * 2 + 2);
			if (allow_condition)
			{
				trav(buf, dist, n, x, i * 2 + 1);
			}
		}
		else
		{
			trav(buf, dist, n, x, i * 2 + 1);
			if (allow_condition)
			{
				trav(buf, dist, n, x, i * 2 + 2);
			}
		}
	}
	else if (i >= total)
	{
		return;
	}

	double e2 = (photons[i]->x - x).norm();
	if (e2 >= dist[n - 1])
	{
		return;
	}

	int l = 0;
	int r = n;
	int m;
	while (l < r)
	{
		m = (l + r) / 2;
		if (e2 < dist[m])
		{
			r = m;
		}
		else
		{
			l = m + 1;
		}
	}

	// the initialisation might be error (if yes try j = n)
	// int j = n - 1;
	for(int j = l;j<n-1;j++){
		dist[j + 1] = dist[j];
		buf[j + 1] = buf[j];
	}

	buf[l] = photons[i];
	dist[l] = e2;
}