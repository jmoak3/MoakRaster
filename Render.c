#include "Render.h"
#include "Window.h"
#include "TriangleMesh.h"


void InitializePixels(int width, int height)
{
	//initiialize pixels - 2D array 
	Pixels = malloc(sizeof(int)*height*width);
	int i=0;
	for (int i=0;i<width*height;++i)
		Pixels[i] = malloc(sizeof(int)*3);
	//done
}

void ReleasePixels()
{
	int i=0;
	for (int i=0;i<width*height;++i)
		free(Pixels[i]);
	free(Pixels);
	Pixels = 0;
}

void RenderMesh((int width, int height, char *fileName, Vector3 cam)
{
	//near plane is 0.5 away
	//form near plane
	//form far plane
	//form camera transforms
	float temp = 0.1f/(-cam.z);
	Vector3 v1; v1.x = temp*cam.x; v1.y = temp*cam.y; v1.z = -cam.z;
	Transform t1 = MakeScale(v1);
	

	TriangleMesh sphere;
	Vector3 offset; offset.x = 0.f; offset.y = 0.f; offset.z = 5.f;
	Transform t = MakeTranslation(&offset);
	printf("Translation Made correctly\n");
	Material mat; mat.red = 1.f; mat.green = 0.f; mat.blue = 0.f;
	FormTriangleMesh(fileName, &sphere, &t, &mat);
	printf("TriangleMesh Formed Made correctly\n");
	Triangle * tris = malloc(sizeof(Triangle)*sphere.numTris);
	GetTrianglesFromMesh(&sphere, tris);
	printf("Triangles harvested correctly\n");

	InitializePixels(width, height);
	int column, row, i;
	int numTris = sphere.numTris;
	for (tris=0;tris<numTris;++i)
	{
		for (row=0;row<height;++row)
		{
			for (column=0;column<width;++column)
			{

			}
		}
	}	
}