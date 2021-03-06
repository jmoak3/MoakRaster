#include "3DMath.h"
#include <math.h>
#include <string.h>

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

Transform RotateX(float angle)
{
	Transform t;

	float sint = sin((0.017453f*angle));
	float cost = cos((0.017453f*angle));
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

Transform RotateY(float angle)
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

Transform RotateZ(float angle)
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
	t.m[2][2] = 1.f;
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

void SubVec2(Vector2 * v1, Vector2 * v2, Vector2 * outV)
{
	outV->x = v1->x - v2->x;
	outV->y = v1->y - v2->y;
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

float DotVec3(Vector3 *v1, Vector3 *v2)
{ 
	return (v1->x*v2->x + v1->y*v2->y + v1->z*v2->z); 
}

float DetVec2(Vector2 *v1, Vector2 *v2)
{
	return v2->x*v1->y - v2->y*v1->x;
}

void UnionBB(BoundingBox2D *b1, BoundingBox2D *b2, BoundingBox2D *outb)
{
	outb->min.x = Minimum(b1->min.x, b2->min.x);//  Min-------| since Raster!!
	outb->min.y = Minimum(b1->min.y, b2->min.y);//   |        | 
	outb->max.x = Maximum(b1->max.x, b2->max.x);//   |        | 
	outb->max.y = Maximum(b1->max.y, b2->max.y);//   --------Max
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

void InvertTrans(Transform * t, Transform *it)
{
	//SPECIAL THANKS TO
	//PHYSICALLY BASED RENDERING: FROM THEORY TO IMPLEMENTATION
	//BY MATT PHARR AND GREG HUMPHREYS
	//FOR THEIR GREAT IMPLEMENTATION OF 
	//OF A NUMERICALLY STABLE GUASSIAN-JORDAN ROUTINE!!!
	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	//memcpy(it->m, t->m, 4 * 4 * sizeof(float));
	int a,b;
	for (a=0;a<4;++a)
	{
		for (b=0;b<4;++b)
		{
			it->m[a][b] = t->m[a][b];
		}
	}
	int i;
	for (i = 0; i < 4; ++i) 
	{
		int irow = -1, icol = -1;
		float big = 0.f;
		int j;
		for (j = 0; j < 4; j++) 
		{
			if (ipiv[j] != 1)
			{
				int k;
				for (k = 0; k < 4; ++k) 
				{
					if (ipiv[k] == 0) 
					{
						if (fabsf(it->m[j][k]) >= big) 
						{
							big = fabsf(it->m[j][k]);
							irow = j;
							icol = k;
						}
					}
				}
			}
		}
		++ipiv[icol];
		if (irow != icol) 
		{
			int k;
			for (k = 0; k < 4; ++k)
			{
				float temp = it->m[irow][k];
				it->m[irow][k] = it->m[icol][k];
				it->m[icol][k] = temp;
			}
		}
		indxr[i] = irow;
		indxc[i] = icol;
		float pivinv = 1.f / it->m[icol][icol];
		it->m[icol][icol] = 1.f;
		for (j = 0; j < 4; j++)
			it->m[icol][j] *= pivinv;

		for (j = 0; j < 4; ++j)
		 {
			if (j != icol) 
			{
				float save = it->m[j][icol];
				it->m[j][icol] = 0;
				int k;
				for (k = 0; k < 4; k++)
					it->m[j][k] -= it->m[icol][k] * save;
			}
		}
	}
	int j; 
	for (j = 3; j >= 0; --j) 
	{
		if (indxr[j] != indxc[j]) {
			int k; for (k = 0; k < 4; k++)
			{
				float temp = it->m[k][indxr[j]];
				it->m[k][indxr[j]] = it->m[k][indxc[j]];
				it->m[k][indxc[j]] = temp;
			}
		}
	}
}
