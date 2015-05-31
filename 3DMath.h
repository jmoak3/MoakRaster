#ifndef DMATH_H
#define DMATH_H

typedef struct
{
	float x;
	float y;
} Vector2;

typedef struct
{
	float x;
	float y;
	float z;
	float filler;
} Vector3;

typedef struct 
{
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct
{
	Vector3 o;
	Vector3 d;
	float min;
	float dist;
} Ray;

typedef struct
{
	float m[4][4];	
} Transform;

typedef struct
{
	Vector2 min;
	Vector2 max;
} BoundingBox2D;


Transform MakeTranslation(Vector3 * v); // define using extern
Transform MakeScale(Vector3 * v);
Transform RotateX(float angle);
Transform RotateY(float angle);
Transform RotateZ(float angle);
void TransformVec2(Transform * t, Vector2 * srcV, Vector2 * destV);
void TransformVec3(Transform * t, Vector3 * srcV, Vector3 * destV);
void TransformTrans(Transform * t, Transform *srcT, Transform *destT);
void TransformRay(Transform * t, Ray * srcR, Ray * destR);
void TransformBBox(Transform * t, BoundingBox2D * srcBox, BoundingBox2D * destBox);
void NormalizeVec3(Vector3 * srcV, Vector3 * destV);
void CrossVec3(Vector3 *v1, Vector3 *v2, Vector3 *outV);
void SubVec3(Vector3 * v1, Vector3 * v2, Vector3 * outV);
void SubVec2(Vector2 * v1, Vector2 * v2, Vector2 * outV);
void AddVec3(Vector3 * v1, Vector3 * v2, Vector3 * outV);
float Minimum(float a, float b);
float Maximum(float a, float b);
float DetVec2(Vector2 *v1, Vector2 *v2);
float DotVec3(Vector3 *v1, Vector3 *v2);
void UnionBB(BoundingBox2D *b1, BoundingBox2D *b2, BoundingBox2D *outb);
void UnionVec3(BoundingBox2D *b1, Vector3 *pt, BoundingBox2D *outb);
int DoesIntersectBBox2D(BoundingBox2D *bbox, Ray * ray);

#endif
