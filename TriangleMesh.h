#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "3DMath.h"
#include "Material.h"

//TODO: INITIALIZE MESH!!!!!
//Loading Meshes

typedef struct 
{
	float tHit;
	float eps;
	Material material;
	int shapeID;
	BoundingBox2D bbox;
} Hit;

typedef struct
{
	Transform * o2w;
	Material material;
	int numTris;
	int numVerts;
	int *vertIndices;
	Vector3 *vertPoints;
	BoundingBox2D bbox;
	int shapeID;	
} TriangleMesh;

typedef struct
{
	Transform * o2w;
	TriangleMesh * mesh;
	BoundingBox2D bbox;
	int * vert;	
} Triangle;

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat);
void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * triangles);
int DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit);
int DoesIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit);

#endif
