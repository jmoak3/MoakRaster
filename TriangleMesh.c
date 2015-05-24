#include "TriangleMesh.h"
#include "MeshLoad.h"
#include <stdio.h>
#include <string.h>

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat)
{
	printf("Inside FormTriangleMesh\n");
	int ** indices = malloc(sizeof(int*));
	Vector3 ** points = malloc(sizeof(Vector3*));
	int numInd = 0;
	int numPts = 0;
	printf("Loading Mesh:\n");
	LoadMesh("sphere.obj", indices, points, &numInd, &numPts);
	printf("Mesh Loaded Correctly\n");

	if (1)
	{	
		int i=0;
		FILE*fp = fopen("test2.txt", "w");
		for (i=0;i<numPts;i+=1)
		{
			fprintf(fp, "%f, %f, %f\n", (*points)[i].x, (*points)[i].y, (*points)[i].z);
		}
		for (i=0;i<numInd;i+=3)
		{
			fprintf(fp, "%i, %i, %i\n", (*indices)[i], (*indices)[i+1], (*indices)[i+2]);
		}
		fclose(fp);
	}

	mesh->o2w = t;
	mesh->material = *mat;
	mesh->numTris = numInd/3;
	mesh->numVerts = numPts;
	//mesh->vertIndices = malloc(sizeof(int)*numInd);
	//mesh->vertPoints = malloc(sizeof(Vector3)*numPts);
	//memcpy(mesh->vertIndices, indices, sizeof(int)*numInd);
	//memcpy(mesh->vertPoints, points, sizeof(Vector3)*numPts);
	mesh->vertIndices = *indices;
	mesh->vertPoints = *points;
	int i=0;
	for (i=0;i<numPts;++i) // Transform to world space!
		TransformVec3(t, &(mesh->vertPoints[i]), &(mesh->vertPoints[i]));
	//Generate Bounding Box
	BoundingBox2D bbox; bbox.min.x = (*points)[i].x; bbox.min.y = (*points)[i].y; 
						bbox.max.x = (*points)[i].x; bbox.max.y = (*points)[i].y;
	for (i=0;i<numPts;++i)
		UnionVec3(&bbox, &(*points)[i], &bbox);
	mesh->bbox = bbox;
	mesh->shapeID = -1; //FIX ME LATER

	if (1)
	{	
		int i=0;
		FILE*fp = fopen("test3.txt", "w");
		for (i=0;i<numPts;i+=1)
		{
			fprintf(fp, "%f, %f, %f\n", mesh->vertPoints[i].x, mesh->vertPoints[i].y, mesh->vertPoints[i].z);
		}
		for (i=0;i<numInd;i+=3)
		{
			fprintf(fp, "%i, %i, %i\n", mesh->vertIndices[i], mesh->vertIndices[i+1], mesh->vertIndices[i+2]);
		}
		fclose(fp);
	}
}

void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * tri)
{
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		Triangle t;
		t.o2w = mesh->o2w;
		t.mesh = mesh;
		t.vert = &(mesh->vertIndices[3*i]);
		printf("Tri verts initialized\n");
		Vector3 g = mesh->vertPoints[1];
		printf("%f %f %f\n", g.x, g.y, g.z);
		Vector3 a = mesh->vertPoints[t.vert[0]];
		Vector3 b = mesh->vertPoints[t.vert[1]];
		Vector3 c = mesh->vertPoints[t.vert[2]];
		printf("bbox initializing\n");
		BoundingBox2D bbox; bbox.min.x = a.x; bbox.min.y = a.y;
						    bbox.max.x = a.x; bbox.max.y = a.y;
		UnionVec3(&bbox, &b, &bbox);
		UnionVec3(&bbox, &c, &bbox);
		printf("BBox initialized\n");
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
	SubVec3(&b, &a, &e1);
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
	GetTrianglesFromMesh(mesh, tris);
	int result=0;
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		result = result || DoesIntersectTri(&tris[i], ray, hit);
	}
	return result;
}


int ReleaseMeshData(TriangleMesh * mesh)
{
	free(mesh->vertIndices);
	free(mesh->vertPoints);
	free(mesh->o2w);
}

int ReleaseTriangleDataOnly(Triangle * tris)
{
	int numTris = tris[0].mesh->numTris;
	int i=0;
	for (i=0;i<numTris;++i)
	{
		free(tris[i].o2w);
		free(tris[i].vert);
	}
	free(tris);
}