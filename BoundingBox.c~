#include "BoundingBox.c"
#include <math.h>

void Union(BoundingBox *b1, BoundingBox *b2, BoundingBox *outb)
{
	outb->min.x = Minimum(b1->min.x, b2->min.x);
	outb->min.y = Minimum(b1->min.y, b2->min.y);
	outb->max.x = Minimum(b1->max.x, b2->max.x);
	outb->max.y = Minimum(b1->max.y, b2->max.y);
}

bool DoesIntersectBBox2D(BoundingBox2D *bbox, Ray * ray)
{
	float t0 = ray->min;
 	float t1 = INFINITY;
	float invRayDir = 1.f / ray->d.x;
	float tNear = (bbox->min.x - ray->o.x) * invRayDir;
	float tFar = (bbox->max.x - ray->o.x) * invRayDir;
	if (tNear > tFar)
	{
		float temp = tFar;
		tFar = tNear;
		tNear = tFar;			
	}
	t0 = tNear > t0 ? tNear : t0;
	t1 = tFar < t1 ? tFar : t1;
	if (t0 > t1) return false;
	
	invRayDir = 1.f / ray->d.y;
	tNear = (bbox->min.y - ray->o.y) * invRayDir;
	tFar = (bbox->max.y - ray->o.y) * invRayDir;
	if (tNear > tFar)
	{
		float temp = tFar;
		tFar = tNear;
		tNear = tFar;			
	}
	t0 = tNear > t0 ? tNear : t0;
	t1 = tFar < t1 ? tFar : t1;
	if (t0 > t1) return false;

	if (t0 < ray->min)
		return false;
 	return true;
}
