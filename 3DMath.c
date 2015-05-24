#include "3DMath.h"
#include <math.h>

Transform MakeTranslation(Vector3 * v)
{
	Transform t;
	
	t.m[0][0] = 1.f;
	t.m[0][1] = 0.f;
	t.m[0][2] = 0.f;
	t.m[0][3] = v->x;

	t.m[1][0] = 0.f;
	t.m[1][1] = 1.f;
	t.m[1][2] = 0.f;
	t.m[1][3] = v->y;
	
	t.m[2][0] = 0.f;
	t.m[2][1] = 0.f;
	t.m[2][2] = 1.f;
	t.m[2][3] = v->z;
	
	t.m[3][0] = 0.f;
	t.m[3][1] = 0.f;
	t.m[3][2] = 0.f;
	t.m[3][3] = 1.f;

	return t;
}

Transform MakeScale(Vector3 * v)
{
	Transform t;
	
	t.m[0][0] = v->x;
	t.m[0][1] = 0.f;
	t.m[0][2] = 0.f;
	t.m[0][3] = 0.f;

	t.m[1][0] = 0.f;
	t.m[1][1] = v->y;
	t.m[1][2] = 0.f;
	t.m[1][3] = 0.f;

	t.m[2][0] = 0.f;
	t.m[2][1] = 0.f;
	t.m[2][2] = v->z;
	t.m[2][3] = 0.f;

	t.m[3][0] = 0.f;
	t.m[3][1] = 0.f;
	t.m[3][2] = 0.f;
	t.m[3][3] = 1.f;
	return t;
}

extern Transform RotateX(float angle)
{
	Transform t;

	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	t.m[0][0] = 1.f;
	t.m[0][1] = 0.f;
	t.m[0][2] = 0.f;
	t.m[0][3] = 0.f;

	t.m[1][0] = 0.f;
	t.m[1][1] = cost;
	t.m[1][2] = -sint;
	t.m[1][3] = 0.f;

	t.m[2][0] = 0.f;
	t.m[2][1] = sint;
	t.m[2][2] = cost;
	t.m[2][3] = 0.f;

	t.m[3][0] = 0.f;
	t.m[3][1] = 0.f;
	t.m[3][2] = 0.f;
	t.m[3][3] = 1.f;
	return t;
}

extern Transform RotateY(float angle)
{
	Transform t;

	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	t.m[0][0] = cost;
	t.m[0][1] = 0.f;
	t.m[0][2] = sint;
	t.m[0][3] = 0.f;

	t.m[1][0] = 0.f;
	t.m[1][1] = 1.f;
	t.m[1][2] = 0.f;
	t.m[1][3] = 0.f;

	t.m[2][0] = -sint;
	t.m[2][1] = 0.f;
	t.m[2][2] = cost;
	t.m[2][3] = 0.f;

	t.m[3][0] = 0.f;
	t.m[3][1] = 0.f;
	t.m[3][2] = 0.f;
	t.m[3][3] = 1.f;
	return t;
}

extern Transform RotateZ(float angle)
{
	Transform t;

	float sint = sinf((0.017453f*angle));
	float cost = cosf((0.017453f*angle));
	t.m[0][0] = cost;
	t.m[0][1] = -sint;
	t.m[0][2] = 0.f;
	t.m[0][3] = 0.f;

	t.m[1][0] = sint;
	t.m[1][1] = cost;
	t.m[1][2] = 0.f;
	t.m[1][3] = 0.f;
	
	t.m[2][0] = 0.f;
	t.m[2][1] = 0.f;
	t.m[2][2] = 0.f;
	t.m[2][3] = 0.f;

	t.m[3][0] = 0.f;
	t.m[3][1] = 0.f;
	t.m[3][2] = 0.f;
	t.m[3][3] = 1.f;
	return t;
}

void TransformVec2(Transform * t, Vector2 * srcV, Vector2 * destV)
{
	float x = srcV->x; float y = srcV->y;
	destV->x = t->m[0][0]*x + t->m[0][1]*y + t->m[0][3];
	destV->y = t->m[1][0]*x + t->m[1][1]*y + t->m[1][3];
}

void TransformVec3(Transform * t, Vector3 * srcV, Vector3 * destV)
{
	float x = srcV->x; float y = srcV->y; float z = srcV->z;
	destV->x = t->m[0][0]*x + t->m[0][1]*y + t->m[0][2]*z + t->m[0][3];
	destV->y = t->m[1][0]*x + t->m[1][1]*y + t->m[1][2]*z + t->m[1][3];
	destV->z = t->m[2][0]*x + t->m[2][1]*y + t->m[2][2]*z + t->m[2][3];
}

void TransformTrans(Transform * t1, Transform *t2, Transform *destT)
{
	int i,j;
	for (i=0;i<4;++i)
		for (j=0;j<4;++j)
			destT->m[i][j]= t1->m[i][0] * t2->m[0][j] +
							t1->m[i][1] * t2->m[1][j] +
							t1->m[i][2] * t2->m[2][j] +
							t1->m[i][3] * t2->m[3][j];
}

void TransformRay(Transform * t, Ray * srcR, Ray * destR)
{
	TransformVec3(t, &(srcR->o), &(destR->o));
	TransformVec3(t, &(srcR->d), &(destR->d));
	destR->min = srcR->min;
}

void TransformBBox(Transform * t, BoundingBox2D * srcBox, BoundingBox2D * destBox)
{
	Vector2 transformedVec;
	TransformVec2(t, &(srcBox->min), &transformedVec);
	destBox->min = transformedVec;
	TransformVec2(t, &(srcBox->max), &transformedVec);
	
	destBox->max = transformedVec;
}

void NormalizeVec3(Vector3 * srcV, Vector3 * destV)
{
	float x = srcV->x; float y = srcV->y; float z = srcV->z; 
	float inv = 1.f/sqrtf(x*x + y*y + z*z);
	destV->x = x*inv;
	destV->y = y*inv;
	destV->z = z*inv;
}

float DotVec3(Vector3 *v1, Vector3 *v2)
{ 
	return (v1->x*v2->x + v1->y*v2->y + v1->z*v2->z); 
}

void CrossVec3(Vector3 *v1, Vector3 *v2, Vector3 *outV)
{
	outV->x = (v1->y * v2->z) - (v1->z * v2->y);
	outV->y = (v1->z * v2->x) - (v1->x * v2->z);
	outV->z = (v1->x * v2->y) - (v1->y * v2->x);
}

void SubVec3(Vector3 * v1, Vector3 * v2, Vector3 * outV)
{
	outV->x = v1->x - v2->x;
	outV->y = v1->y - v2->y;
	outV->z = v1->z - v2->z;
}

void AddVec3(Vector3 * v1, Vector3 * v2, Vector3 * outV)
{
	outV->x = v1->x + v2->x;
	outV->y = v1->y + v2->y;
	outV->z = v1->z + v2->z;
}

float Minimum(float a, float b)
{
	float min = a;
	if (b < a) min = b;
	return min;
}

float Maximum(float a, float b)
{
	float max = a;
	if (b > a) max = b;
	return max;
}


void UnionBB(BoundingBox2D *b1, BoundingBox2D *b2, BoundingBox2D *outb)
{
	outb->min.x = Minimum(b1->min.x, b2->min.x);
	outb->min.y = Minimum(b1->min.y, b2->min.y);
	outb->max.x = Maximum(b1->max.x, b2->max.x);
	outb->max.y = Maximum(b1->max.y, b2->max.y);
}

void UnionVec3(BoundingBox2D *b1, Vector3 *pt, BoundingBox2D *outb)
{
	outb->min.x = Minimum(b1->min.x, pt->x);
	outb->min.y = Minimum(b1->min.y, pt->y);
	outb->max.x = Maximum(b1->max.x, pt->x);
	outb->max.y = Maximum(b1->max.y, pt->y);
}

int DoesIntersectBBox2D(BoundingBox2D *bbox, Ray * ray)
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
	if (t0 > t1) return 0;
	
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
	if (t0 > t1) return 0;

	if (t0 < ray->min)
		return 0;
 	return 1;
}


