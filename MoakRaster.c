#include <stdio.h>
#include <time.h>
#include "3DMath.h"
#include "TriangleMesh.h"
#include <malloc.h>
#include "Render.h"

void edgeTests()
{
	struct timespec timeobj;
	double lastTime = 0.0;
	double currTime = 0.0;
	double totalTime = 0.0;
	long i;
	float width = 100.f, height = 100.f;
	Vector2 inPt; inPt.x = 0.5f*width; inPt.y = 0.5f*height;
	Vector2 outPt; outPt.x = 0.1f*width; outPt.y = 0.1f*height;
	Hit hit;
	TriangleMesh mesh; mesh.vertPoints = malloc(sizeof(Vector3)*3);
	Vector3 v0; v0.x = 0.25f*width; v0.y = 0.75f*height; v0.z = 0.f;
	mesh.vertPoints[0] = v0;
	Vector3 v1; v1.x = 0.25f*width; v1.y = 0.25f*height; v1.z = 0.f;
	mesh.vertPoints[1] = v1;
	Vector3 v2; v2.x = 0.75f*width; v2.y = 0.5f*height; v2.z = 0.f;
	mesh.vertPoints[2] = v2;
	mesh.vertIndices = malloc(sizeof(int)*3);
	mesh.vertIndices[0] = 0; mesh.vertIndices[1] = 1; mesh.vertIndices[2] = 2;
	Triangle tri; tri.mesh = &mesh; tri.vert = &(mesh.vertIndices[0]);
	printf("Edge Tests Initialized!\n");

	//Test Edge Detection with Affine Space
	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	lastTime = timeobj.tv_sec*1000000000.0 + timeobj.tv_nsec;

	for (i=0;i<10000000;++i)
		if (!AffineTest(&tri, &inPt, &hit)) {printf("Affine in failed\n"); return;}
	for (i=0;i<10000000;++i)
		if (AffineTest(&tri, &outPt, &hit)) {printf("Affine out failed\n"); return;}

	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	currTime = timeobj.tv_sec*1000000000.0 + timeobj.tv_nsec;
	totalTime = (currTime-lastTime)/20000000.0;
	printf("Affine Edge Test Method: %fns\n", totalTime);


	//Test Edge Detection using Det Product
	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	lastTime = timeobj.tv_sec*1000000000.0 + timeobj.tv_nsec;

	for (i=0;i<10000000;++i)
		if (!EdgeTest(&tri, &inPt, &hit)) {printf("edge in failed\n"); return;}
	for (i=0;i<10000000;++i)
		if (EdgeTest(&tri, &outPt, &hit)) {printf("edge out failed\n"); return;}

	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	currTime = timeobj.tv_sec*1000000000.0 + timeobj.tv_nsec;
	totalTime = (currTime-lastTime)/20000000.0;
	printf("Det Edge Test Method: %fns\n", totalTime);

	printf("Done Testing\n");
}

int main()
{
	printf("Moak Raster Starting\n");
	Vector3 cam; cam.x = 0.0f; cam.y = 0.f; cam.z = 0.f;
	int width = 800;
	int height = 600;
	Render(width, height, "Sphere.obj", cam);
	return 0;
}
