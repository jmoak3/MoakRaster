#include "3DMath.h"

bool DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit)
{
	//if (!WorldBounds.Intersect(ray)) //ADD BB TO TRI
	//	return false;
	Ray r = *ray;
	
	Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
	Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
	Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
	Vector e1;
	SubVec(&b, &a, &e1);
	Vector e2;
	SubVec(&c, &a, &e2);
	Vector s1; 
	Cross(&(r.d), &e2, &s1);
	
	float denominator = Dot(&s1, &e1);
	if (denominator>-0.001f && denominator<0.001f)
		return false;
	float invDenom = 1.f/denominator;
	
	Vector3 s;
	SubVec3(&(r.o), &a, &s);
	
	float b1 = Dot(&s, &s1)*invDenom;
	if (b1 < 0.f || b1 > 1.f)
		return false;

	Vector s2;
	Cross(&s, &e1, &s2);
	float b2 = Dot(&(r.d), &s2)*invDenom;
	if (b2 < 0.f || b1+b2 > 1.f)
		return false;
	
	float t = Dot(&e2, &s2)*invDenom;
	if (t < r.min)
		return false;

	hit->tHit = t;
	hit->eps = 1e-3*t;
	hit->shapeID = ShapeID;
	hit->color = Mat;
	return true;
}
