#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "3DMath.h"
#include "Material.h"
#include "BoundingBox.h"

//TODO: INITIALIZE MESH!!!!!
//Loading Meshes

typedef struct 
{
	float tHit = INFINITY;
	float eps = 0.f;
	Material material;
	int shapeID = -1;
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
	
} TriangleMesh;

typedef struct
{
	Transform * o2w;
	TriangleMesh * mesh;
	BoundingBox2D bbox;
	
} Triangle;

void FormTriangleMesh(
void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * triangles);
bool DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit);
bool DoesIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit);

#endif
