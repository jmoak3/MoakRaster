#include <stdio.h>
#include "3DMath.h"
#include "TriangleMesh.h"
#include <malloc.h>
#include "Render.h"

int main()
{
	printf("Hello World\n");
	Vector3 cam; cam.x = 0.0f; cam.y = 0.f; cam.z = 0.f;
	RenderMesh(width, height, "Sphere.obj", cam);
	return 0;
}
