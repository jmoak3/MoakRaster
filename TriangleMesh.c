#include "3DMath.h"

bool DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit)
{
	//if (!WorldBounds.Intersect(ray)) //ADD BB TO TRI
	//	return false;
	Ray r = *ray;
	
	Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
	Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
	Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
	Vector3 e1;
	SubVec(&b, &a, &e1);
	Vector3 e2;
	SubVec3(&c, &a, &e2);
	Vector3 s1; 
	CrossVec3(&(r.d), &e2, &s1);
	
	float denominator = DotVec3(&s1, &e1);
	if (denominator>-0.001f && denominator<0.001f)
		return false;
	float invDenom = 1.f/denominator;
	
	Vector3 s;
	SubVec3(&(r.o), &a, &s);
	
	float b1 = DotVec3(&s, &s1)*invDenom;
	if (b1 < 0.f || b1 > 1.f)
		return false;

	Vector3 s2;
	CrossVec3(&s, &e1, &s2);
	float b2 = DotVec3(&(r.d), &s2)*invDenom;
	if (b2 < 0.f || b1+b2 > 1.f)
		return false;
	
	float t = DotVec3(&e2, &s2)*invDenom;
	if (t < r.min)
		return false;

	hit->tHit = t;
	hit->eps = 1e-3*t;
	hit->shapeID = ShapeID;
	hit->color = Mat;
	return true;
}
