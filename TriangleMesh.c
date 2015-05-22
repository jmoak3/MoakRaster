#include "3DMath.h"
#include "MeshLoad.h"

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat)
{
	int sizePts = 8;
	int sizeInds = 12;
	int * indices = (int*)malloc(sizeof(int)*sizePts);
	Vector3 * points = (Vector3*)malloc(sizeof(Vector3)*sizeInds);
	int numInd = 0;
	int numPoints = 0;
	MeshLoad("dragon.obj", indices, points, &numInd, &numPoints, sizePts, sizeInds);

	//Generate Bounding Box
	BoundingBox2D bbox; bbox.min = points[0]; bbox.max = points[0];
	int i=0;
	for (i=0;i<numPoints;++i)
		Union(&bbox, &points[i], &bbox);
	
	if (*mesh==NULL) printf("MESH IS NULL!!!\n");
	*mesh.o2w = t;
	*mesh.material = *mat;
	*mesh.numTris = numInd/3;
	*mesh.numPoints = numPts/3;
	*mesh.vertIndices = indices;
	*mesh.vertPoints = points;
	*mesh.bbox = bbox;
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

void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * triangles)
{
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		Triangle t;
		t.o2w = mesh->o2w;
		t.mesh = mesh;
		Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
		Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
		Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
		BoundingBox2D bbox; bbox.min = a; bbox.max = a;
		Union(&bbox, &b, &bbox);
		Union(&bbox, &c, &bbox);
		t.bbox = bbox;
		triangles[i] = t;
	}
}

int DoesIntersectTri(Triangle * tri, Ray * ray, Hit * hit)
{
	//BEFORE TESTING IF TRI INTERSECT, ALWAYS TEST THAT MESH INTERSECT FIRST!!!!!!
	if (!DoesIntersectBBox2D(&(tri->bbox), ray) 
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
		return false;
	float invDenom = 1.f/denominator;
	
	Vector3 s;
	SubVec3(&(r.o), &a, &s);
	
	float b1 = DotVec3(&s, &s1)*invDenom;
	if (b1 < 0.f || b1 > 1.f)
		return false;
	
	Vector3 s2;
	CrossVec3(&s, &e1, &s2);
	float b2 = DotVec3(&(r.d), &s2)*invDenom;
	if (b2 < 0.f || b1+b2 > 1.f)
		return false;
	
	float t = DotVec3(&e2, &s2)*invDenom;
	if (t < r.min)
		return false;

	hit->tHit = t;
	hit->eps = 1e-3*t;
	hit->shapeID = tri->mesh->shapeID;
	hit->color = tri->mesh->material;
	return 1;
}

int DoesIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit)
{
	if (!DoesIntersectBBox2D(&(mesh->bbox), ray) 
		return 0;
	Triangle * tris = (Triangle*)malloc(sizeof(Triangle)*mesh->numTris);
	GetTrisFromMesh(mesh, tris);
	int i=0;
	
	return 1;
}






