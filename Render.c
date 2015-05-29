#include "Render.h"
#include "Window.h"
#include "TriangleMesh.h"


void InitializePixels(int width, int height)
{
	//initiialize pixels - 2D array 
	Pixels = malloc(sizeof(char*)*height*width*3);
	printf("Pixels Initialized!\n");
	//done
}

void ReleasePixels(int width, int height)
{
	free(Pixels);
	Pixels = 0;
}

void RenderMesh(int width, int height, Triangle *tris)
{
    int column, row, i;
	int numTris = tris[0].mesh->numTris;
	Hit hit; Vector2 pt; //preallocate
	for (i=0;i<numTris;++i)
	{
		int t = tris[i].bbox.max.y;
		int b = tris[i].bbox.max.y;
		int l = tris[i].bbox.min.x;
		int r = tris[i].bbox.max.x;
		//printf("t %f b %f l %f r %f\n", t,b,l,r);
		for (row=t;row<b;++row)
		{
			for (column=l;column<r;++column)
			{
				pt.x = column; pt.y = row;
				if (DoesPointLieOnTri(&(tris[i]), &pt, &hit))
				{
					Pixels[row*width + column*3 + 0] = (char)hit.material.red;
					Pixels[row*width + column*3 + 1] = (char)hit.material.blue;
					Pixels[row*width + column*3 + 2] = (char)hit.material.green;
					printf("Rendered something!\n");
				}
				else
				{
					Pixels[row*width + column*3 + 0] = (char)60;
					Pixels[row*width + column*3 + 1] = (char)60;
					Pixels[row*width + column*3 + 2] = (char)60;
					printf("Rendered nothing!\n");
				}
			}
		}
	}	
    
	//Display Pixels
	UpdateWindow(Pixels, width, height);
}

void Render(int width, int height, char *fileName, Vector3 cam)
{
	Transform rasterTrans;
	Vector3 offset; offset.x = 0.f; offset.y = 0.f; offset.z = 1.f;
	Transform meshOffset = MakeTranslation(&offset);
	float temp = 0.1f/(-2.f);
	Vector3 v1; v1.x = temp/width; v1.y = -temp/height; v1.z = -1.f;
	Transform t1 = MakeScale(&v1);
    TransformTrans(&t1, &meshOffset, &rasterTrans);
	Vector3 v2; v2.x = 1.f; v2.y = 1.f; v2.z = 0.f;
	Transform t2 = MakeTranslation(&v2);
    TransformTrans(&t2, &rasterTrans, &rasterTrans);

    //Iniitializemesh
	TriangleMesh mesh;
	Material mat; mat.red = 1.f; mat.green = 0.f; mat.blue = 0.f;
	FormTriangleMesh(fileName, &mesh, &rasterTrans, &mat);
	printf("TriangleMesh Formed Made correctly\n");
	Triangle * tris = malloc(sizeof(Triangle)*mesh.numTris);
	GetTrianglesFromMesh(&mesh, tris);
	printf("Triangles harvested correctly\n");
	
	//Render loop
	InitializePixels(width, height);
	MakeWindow(width, height);
	//while (1)
	RenderMesh(width, height, tris);
	long i; for (i=1;i<1300;++i) {RenderMesh(width, height, tris);}

	//Release Pixels
	ReleasePixels(width, height);
}