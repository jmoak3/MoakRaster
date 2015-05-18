#ifndef 3DMATH_H
#define 3DMATH_H

struct Vector3
{
	float x=0.f;
	float y=0.f;
	float z=0.f;
} VECTOR3;

struct Vector4
{
	float x=0.f;
	float y=0.f;
	float z=0.f;
	float w=0.f;
} VECTOR4;

struct Transform
{
	float m[4][4];	
} TRANSFORM;

inline Transform MakeTranslation(Vector3 * v); // define using extern
inline Transform MakeScale(Vector3 * v);
inline Transform RotateX(float angle);
inline Transform RotateY(float angle);
inline Transform RotateZ(float angle);
void TranslateVec(Transform * t, Vector3 * srcV, Vector3 * destV);
void TransformTrans(Transform * t, Transform *srcT, Transform *destT);


#endif
