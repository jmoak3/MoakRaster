#include "TriangleMesh.h"
#include "MeshLoad.h"

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat)
{
	int sizePts = 8;
	int sizeInds = 12;
	int * indices = (int*)malloc(sizeof(int)*sizePts);
	Vector3 * points = (Vector3*)malloc(sizeof(Vector3)*sizeInds);
	int numInd = 0;
	int numPts = 0;
	MeshLoad("dragon.obj", indices, points, &numInd, &numPts, sizePts, sizeInds);

	//Generate Bounding Box
	BoundingBox2D bbox; bbox.min.x = points[0].x; bbox.min.y = points[0].y; 
						bbox.max.x = points[0].x; bbox.max.y = points[0].y;
	int i=0;
	for (i=0;i<numPts;++i)
		Union(&bbox, &points[i], &bbox);
	
	mesh->o2w = t;
	mesh->material = *mat;
	mesh->numTris = numInd/3;
	mesh->numVerts = numPts/3;
	mesh->vertIndices = indices;
	mesh->vertPoints = points;
	mesh->bbox = bbox;
	mesh->shapeID = -1; //FIX ME LATER
	/*
	Transform * o2w;
	Material material;
	int numTris;
	int numVerts;
	int *vertIndices;
	Vector3 *vertPoints;
	BoundingBox2D bbox;
	*/
}

void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * tri)
{
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		Triangle t;
		t.o2w = mesh->o2w;
		t.mesh = mesh;
		t.vert = malloc(sizeof(int)*3);
		t.vert = &(mesh->vertIndices[3*i]);
		
		Vector3 a = mesh->vertPoints[t.vert[0]];
		Vector3 b = mesh->vertPoints[t.vert[1]];
		Vector3 c = mesh->vertPoints[t.vert[2]];
		BoundingBox2D bbox; bbox.min.x = a.x; bbox.min.y = a.y;
						    bbox.max.x = a.x; bbox.max.y = a.y;
		Union(&bbox, &b, &bbox);
		Union(&bbox, &c, &bbox);
		t.bbox = bbox;
		tri[i] = t;
	}
}

int DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit)
{
	//BEFORE TESTING IF TRI INTERSECT, ALWAYS TEST THAT MESH INTERSECT FIRST!!!!!!
	if (!DoesIntersectBBox2D(&(tri->bbox), ray)) 
		return 0;
	Ray r = *ray;
	
	Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
	Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
	Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
	Vector3 e1;
	SubVec(&b, &a, &e1);
	Vector3 e2;
	SubVec3(&c, &a, &e2);
	Vector3 s1; 
	CrossVec3(&(r.d), &e2, &s1);
	
	float denominator = DotVec3(&s1, &e1);
	if (denominator>-0.001f && denominator<0.001f)
		return 0;
	float invDenom = 1.f/denominator;
	
	Vector3 s;
	SubVec3(&(r.o), &a, &s);
	
	float b1 = DotVec3(&s, &s1)*invDenom;
	if (b1 < 0.f || b1 > 1.f)
		return 0;
	
	Vector3 s2;
	CrossVec3(&s, &e1, &s2);
	float b2 = DotVec3(&(r.d), &s2)*invDenom;
	if (b2 < 0.f || b1+b2 > 1.f)
		return 0;
	
	float t = DotVec3(&e2, &s2)*invDenom;
	if (t < r.min)
		return 0;

	hit->tHit = t;
	hit->eps = 1e-3*t;
	hit->shapeID = tri->mesh->shapeID;
	hit->material = tri->mesh->material;
	return 1;
}

int DoesIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit)
{
	if (!DoesIntersectBBox2D(&(mesh->bbox), ray) )
		return 0;
	Triangle * tris = (Triangle*)malloc(sizeof(Triangle)*mesh->numTris);
	GetTrisFromMesh(mesh, tris);
	int i=0;
	
	return 1;
}






