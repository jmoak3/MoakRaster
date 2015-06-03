#include "TriangleMesh.h"
#include "MeshLoad.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void FormTriangleMesh(char * fileName, TriangleMesh * mesh, Transform * t, Material * mat)
{
	//printf("Inside FormTriangleMesh\n");
	int ** indices = malloc(sizeof(int*));
	Vector3 ** points = malloc(sizeof(Vector3*));
	Vector3 ** normals = malloc(sizeof(Vector3*));
	Vector2 ** tex = malloc(sizeof(Vector2*));
	int ** texInds = malloc(sizeof(int*));
	int ** normalInds = malloc(sizeof(int*));
	int numInd = 0;
	int numPts = 0;
	int numTex = 0;
	int numTexInds = 0;
	int numNormals = 0;
	int numNormalInds = 0;
	//printf("Loading Mesh:\n");
	LoadMesh(fileName, indices, points, tex, texInds, normals, normalInds,
		          &numInd, &numPts, &numTex, &numTexInds, &numNormals, &numNormalInds);
	//printf("Mesh Loaded Correctly\n");

	mesh->material = *mat;
	mesh->numTris = numInd/3;
	mesh->numVerts = numPts;
	mesh->numTex = numTex;
	mesh->numTexInds = numTexInds;
	mesh->numNormals = numNormals;
	mesh->numNormalInds = numNormalInds;
	mesh->vertIndices = *indices;
	mesh->vertPoints = *points;
	mesh->texCoords = *tex;
	mesh->texIndices = *texInds;
	mesh->normals = *normals;
	mesh->normalIndices = *normalInds;

	printf("tris %i tex %i norm %i\n", numInd/3, numTex, numNormals);

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
}

void GetTrianglesFromMesh(TriangleMesh * mesh, Triangle * tri)
{
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		Triangle t;
		t.mesh = mesh;
		t.vert = &(mesh->vertIndices[3*i]);
		t.tex = &(mesh->texIndices[3*i]);
		t.normal = &(mesh->normalIndices[3*i]);
		//printf("Tri verts initialized\n");
		Vector3 g = mesh->vertPoints[0];
		//printf("%f %f %f\n", g.x, g.y, g.z);
		Vector3 a = mesh->vertPoints[t.vert[0]];
		Vector3 b = mesh->vertPoints[t.vert[1]];
		Vector3 c = mesh->vertPoints[t.vert[2]];
		//printf("bbox initializing\n");
		BoundingBox2D bbox; bbox.min.x = a.x; bbox.min.y = a.y;
						    bbox.max.x = a.x; bbox.max.y = a.y;
		UnionVec3(&bbox, &b, &bbox);
		UnionVec3(&bbox, &c, &bbox);
		//printf("BBox initialized Min: %f,%f Max: %f,%f\n",
		//			bbox.min.x, bbox.min.y, bbox.max.x, bbox.max.y);
		t.bbox = bbox;
		tri[i] = t;
	}
}

void PrepareRasterizedDataBuffers(Triangle *tris, TriangleMesh *mesh, 
	 							  Triangle **outTris, TriangleMesh **outMesh)
{
	(*outMesh) = malloc(sizeof(TriangleMesh));
	(*outMesh)->material = mesh->material;	
	(*outMesh)->shapeID = mesh->shapeID;	
	(*outMesh)->numTris = mesh->numTris;	
	(*outMesh)->numVerts = mesh->numVerts;	
	(*outMesh)->numTex = mesh->numTex;	
	(*outMesh)->numTexInds = mesh->numTexInds;	
	(*outMesh)->texCoords = mesh->texCoords;
	(*outMesh)->texIndices = mesh->texIndices;
	(*outMesh)->vertIndices = mesh->vertIndices;
	(*outMesh)->normals = mesh->normals;
	(*outMesh)->normalIndices = mesh->normalIndices;
	(*outMesh)->vertPoints = malloc(sizeof(Vector3)*mesh->numVerts);
	int k;
	for (k=0;k<mesh->numVerts;++k)
	{
		(*outMesh)->vertPoints[k] = mesh->vertPoints[k];
	}
	(*outTris) = malloc(sizeof(Triangle)*mesh->numTris);
	for (k=0;k<mesh->numTris;++k)
	{
		(*outTris)[k].vert = tris[k].vert;
		(*outTris)[k].mesh = (*outMesh);
		(*outTris)[k].tex  = tris[k].tex;
		(*outTris)[k].normal  = tris[k].normal;
	}
}

void TransformTrianglesAndMesh(Triangle *tris, Transform *t, 
							   Triangle *outTris, TriangleMesh *outMesh)
{
	TriangleMesh *mesh = tris[0].mesh;
	int numVerts = mesh->numVerts;
	int numTris = mesh->numTris;
	int i=0;

	for (i=0;i<numVerts;++i) // Apply new transform to verts!
		TransformVec3(t, &(mesh->vertPoints[i]), &(outMesh->vertPoints[i]));
	//TransformBBox(t, &(mesh->bbox), &(outMesh->bbox)); Doesn't work! FIX BBoxTRANS
	Vector3 n;
	for (i=0;i<numTris;++i)
	{
		Vector3 a = outMesh->vertPoints[tris[i].vert[0]];
		Vector3 b = outMesh->vertPoints[tris[i].vert[1]];
		Vector3 c = outMesh->vertPoints[tris[i].vert[2]];
		BoundingBox2D bbox; bbox.min.x = a.x; bbox.min.y = a.y;
						    bbox.max.x = a.x; bbox.max.y = a.y;
		UnionVec3(&bbox, &b, &bbox);
		UnionVec3(&bbox, &c, &bbox);
		outTris[i].bbox = bbox;
		//TransformBBox(t, &(tris[i].bbox), &(outTris[i].bbox));
	}
	//IF:
	//Z less than 0, skip
	//Normal facing wrong way, skip
}

int DoesRayIntersectTri(Triangle * tri, Ray * ray, Hit * hit)
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

int DoesRayIntersectMesh(TriangleMesh * mesh, Ray * ray, Hit * hit)
{
	if (!DoesIntersectBBox2D(&(mesh->bbox), ray) )
		return 0;
	Triangle * tris = (Triangle*)malloc(sizeof(Triangle)*mesh->numTris);
	GetTrianglesFromMesh(mesh, tris);
	int result=0;
	int i=0;
	for (i=0;i<mesh->numTris;++i)
	{
		result = result || DoesRayIntersectTri(&tris[i], ray, hit);
	}
	return result;
}

int DoesPointLieOnTri(Triangle * tri, Vector2 * pt, Hit * hit)
{
	//Solve for NBC
	//If more than 1 we done here
	//Otherwise, hit.
	//Interpolate the edge colors!
	//Use Top-Left!!!

	return AffineTest(tri, pt, hit);
}

int AffineTest(Triangle * tri, Vector2 * pt, Hit * hit)
{
	Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
	Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
	Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
	Vector2 V0V1; V0V1.x = b.x-a.x; V0V1.y = b.y-a.y;
	Vector2 V2V0; V2V0.x = a.x-c.x; V2V0.y = a.y-c.y;
	Vector2 PV0;  PV0.x = a.x-pt->x; PV0.y = a.y-pt->y;
	Vector2 PV2;  PV2.x = c.x-pt->x; PV2.y = c.y-pt->y;
	float den = 1.f/DetVec2(&V0V1, &V2V0);
	float n1 = DetVec2(&V2V0, &PV2)*den;
	if (n1<0.f || n1>1.f) return 0;
	float n2 = DetVec2(&V0V1, &PV0)*den;
	if (n2<0.f || n2>1.f) return 0;

	float avgZ = a.z*(1.f-(n1+n2)) + b.z*n1 + c.z*n2;
	float r = Maximum(0.f, Minimum((sinf(avgZ*2.5f))*255.f, 255.f));
	float g = 100.f;
	float bl = 25.f;
	
	hit->material.red = r;
	hit->material.green = g;
	hit->material.blue = bl;

	//hit->material = tri->mesh->material;
	return n1+n2<1.001f;
}

int EdgeTest(Triangle * tri, Vector2 * pt, Hit * hit)
{
	Vector3 a = tri->mesh->vertPoints[tri->vert[0]];
	Vector3 b = tri->mesh->vertPoints[tri->vert[1]];
	Vector3 c = tri->mesh->vertPoints[tri->vert[2]];
	Vector2 V0V1; V0V1.x = b.x-a.x; V0V1.y = b.y-a.y;
	Vector2 V1V2; V1V2.x = c.x-b.x; V1V2.y = c.y-b.y;
	Vector2 V2V0; V2V0.x = a.x-c.x; V2V0.y = a.y-c.y;
	Vector2 PV0;  PV0.x = a.x-pt->x; PV0.y = a.y-pt->y;
	Vector2 PV1;  PV1.x = b.x-pt->x; PV1.y = b.y-pt->y;
	Vector2 PV2;  PV2.x = c.x-pt->x; PV2.y = c.y-pt->y;
	hit->material = tri->mesh->material;
	return (DetVec2(&V0V1, &PV0) > 0.f) 
		&& (DetVec2(&V1V2, &PV1) > 0.f)
		&& (DetVec2(&V2V0, &PV2) > 0.f); 
}

int ReleaseMeshData(TriangleMesh * mesh)
{
	free(mesh->vertIndices);
	free(mesh->vertPoints);
}

int ReleaseTriangleDataOnly(Triangle * tris)
{
	free(tris);
}