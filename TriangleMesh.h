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
	TriangleMesh * mesh;
	BoundingBox2D bbox;
	int * vert;	
} Triangle;

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat);
void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * triangles);
void TransformTriangles(Triangle * tris, Transform * t);
int DoesRayIntersectTri(Triangle * tri, Ray * ray, Hit * hit);
int DoesRayIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit);
int DoesPointLieOnTri(Triangle * tri, Vector2 * pt, Hit * hit);
int AffineTest(Triangle * tri, Vector2 * pt, Hit * hit);
int EdgeTest(Triangle * tri, Vector2 * pt, Hit * hit);
int ReleaseMeshData(TriangleMesh * mesh);
int ReleaseTriangleDataOnly(Triangle * tris);

#endif
