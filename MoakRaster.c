#include <stdio.h>
#include "3DMath.h"
#include "TriangleMesh.h"
#include <malloc.h>

int main()
{
	printf("Hello World\n");
	TriangleMesh sphere;
	Vector3 offset; offset.x = 0.f; offset.y = 0.f; offset.z = 5.f;
	Transform t = MakeTranslation(&offset);
	printf("Translation Made correctly\n");
	Material mat; mat.red = 1.f; mat.green = 0.f; mat.blue = 0.f;
	FormTriangleMesh("Sphere.obj", &sphere, &t, &mat);
	printf("TriangleMesh Formed Made correctly\n");
	Triangle * tris = malloc(sizeof(Triangle)*sphere.numTris);
	GetTrianglesFromMesh(&sphere, tris);
	printf("Triangles harvested correctly\n");
	return 0;
}
