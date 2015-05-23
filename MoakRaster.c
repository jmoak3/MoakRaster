#include <stdio.h>
#include "3DMath.h"
#include "TriangleMesh.h"

int main()
{
	printf("Hello World\n");
	TriangleMesh sphere;
	Vector3 offset; offset.x = 0.f; offset.y = 0.f; offset.z = 5.f;
	Transform t = MakeTranslation(&offset);
	Material mat; mat.red = 1.f; mat.green = 0.f; mat.blue = 0.f;
	FormTriangleMesh("Sphere.obj", &sphere, &t, &mat);
	Triangle * tris = malloc(sizeof(Triangle)*sphere.numTris);
	GetTrianglesFromMesh(&sphere, tris);
	return 0;
}
