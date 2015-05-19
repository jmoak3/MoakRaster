#include "3DMath.h"

typedef struct
{
	Transform 

} TriangleMesh;

typedef struct
{
	Transform * o2w;
	TriangleMesh * mesh;
	

} Triangle;

bool DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit);
bool DoesIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit);
