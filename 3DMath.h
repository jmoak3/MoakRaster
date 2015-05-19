#ifndef 3DMATH_H
#define 3DMATH_H

typedef struct
{
	float x=0.f;
	float y=0.f;
	float z=0.f;
	float filler=0.f;
} Vector3;

typedef struct 
{
	float x=0.f;
	float y=0.f;
	float z=0.f;
	float w=0.f;
} Vector4;

typedef struct
{
	Vector3 o;
	Vector3 d;
	float min=5e-4;
	float dist=0.f;
} Ray;

struct Transform
{
	float m[4][4];	
} TRANSFORM;

inline Transform MakeTranslation(Vector3 * v); // define using extern
inline Transform MakeScale(Vector3 * v);
inline Transform RotateX(float angle);
inline Transform RotateY(float angle);
inline Transform RotateZ(float angle);
void TransformVec(Transform * t, Vector3 * srcV, Vector3 * destV);
void TransformTrans(Transform * t, Transform *srcT, Transform *destT);
void TransformRay(Transform * t, Ray * srcR, Ray * destR);
void Normalize(Vector3 * srcV, Vector3 * destV);
float Dot(Vector *v1, Vector *v2);
void Cross(Vector *v1, Vector *v2, Vector *outV);
void SubVec(Vector3 * v1, Vector3 * v2, Vector3 * outV);
void AddVec(Vector3 * v1, Vector3 * v2, Vector3 * outV);

#endif
