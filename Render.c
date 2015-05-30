#include "Render.h"
#include "Window.h"
#include "TriangleMesh.h"
#include "time.h"

void InitializePixels(int width, int height)
{
	//initiialize pixels - 2D array 
	Pixels = malloc(sizeof(char*)*height*width*3);
	ResetBuffer(width, height);
	printf("Pixels Initialized!\n");

	//done
}

void ReleasePixels(int width, int height)
{
	free(Pixels);
	Pixels = 0;
}

void ResetBuffer(int width, int height)
{
	//reset buffer
	int bg = 0;
	int i,j;
	for (i=0;i<height;++i)
	{
		for (j=0;j<width;++j)
		{
			Pixels[i*width*3 + j*3 + 0] = (char)bg;
			Pixels[i*width*3 + j*3 + 1] = (char)bg;
			Pixels[i*width*3 + j*3 + 2] = (char)bg;
		}
	}
}

void RenderMesh(int width, int height, Triangle *tris)
{
    int column, row, i;
	int numTris = tris[0].mesh->numTris;
	Hit hit; Vector2 pt; int t,b,l,r; //preallocate
	for (i=0;i<numTris;++i)
	{
		t = tris[i].bbox.min.y;
		b = Minimum(tris[i].bbox.max.y+1, height);
		l = tris[i].bbox.min.x;
		r = Minimum(tris[i].bbox.max.x+1, width);
		//printf("t %i b %i l %i r %i\n", t,b,l,r);
		for (row=t;row<b;++row)
		{
			for (column=l;column<r;++column)
			{
				pt.x = column; pt.y = row;
				if (DoesPointLieOnTri(&(tris[i]), &pt, &hit))
				{
					Pixels[row*width*3 + column*3 + 0] = (char)hit.material.red;
					Pixels[row*width*3 + column*3 + 1] = (char)hit.material.blue;
					Pixels[row*width*3 + column*3 + 2] = (char)hit.material.green;
					//printf("Rendered %f %f %f\n", 
					//		hit.material.red,
					//		hit.material.green,
					//		hit.material.blue);
				}
			}
		}
	}	
}

void Render(int width, int height, char *fileName, Vector3 cam)
{
	
	Transform rasterTrans;
	Vector3 offset; offset.x = 0.f; offset.y = 0.f; offset.z = 0.2f;
	Transform meshOffset = MakeTranslation(&offset);
	float temp = 0.1f/offset.z;
	Vector3 v; v.x = temp; v.y = -temp; v.z = 1.f;
	Transform t = MakeScale(&v);
    TransformTrans(&t, &meshOffset, &rasterTrans);
	v.x = (float)width/(float)height; v.y = 1.f; v.z = 0.f;
	t = MakeTranslation(&v);
    TransformTrans(&t, &rasterTrans, &rasterTrans);
	v.x = height/(2.0f); v.y = height/2.0f; v.z = 1.f;
	t = MakeScale(&v);
    TransformTrans(&t, &rasterTrans, &rasterTrans);

    //Initializemesh
	TriangleMesh mesh;
	Material mat; mat.red = 255.f; mat.green = 0.f; mat.blue = 0.f;
	FormTriangleMesh(fileName, &mesh, &rasterTrans, &mat);
	printf("TriangleMesh Formed Made correctly\n");
	Triangle * tris = malloc(sizeof(Triangle)*mesh.numTris);
	GetTrianglesFromMesh(&mesh, tris);
	printf("Triangles harvested correctly\n");
	
	InitializePixels(width, height);
	MakeWindow(width, height);


	Vector3 move; Transform moveT; //Preallocations
	struct timespec timeobj;
	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	double startTime = timeobj.tv_sec*1000.0 + timeobj.tv_nsec/1000000.0;
	float prevTime = timeobj.tv_sec + timeobj.tv_nsec/1000000000.0;
	float currTime = 0.0;
	float dt = 0.001f;
	printf("\n");
	long i; for (i=0;i<500;++i) 
	{
		clock_gettime(CLOCK_MONOTONIC, &timeobj);
		currTime = timeobj.tv_sec + timeobj.tv_nsec/1000000000.0;
		
		dt = Maximum(currTime-prevTime, 0.00001f); 
		
		move.x = 100.f*dt; move.y = 0.f; move.z = 0.f;
		moveT = MakeTranslation(&move);
	    TransformTriangles(tris, &moveT);

		//Render Mesh to buffer
		RenderMesh(width, height, tris);

		//Display Pixels
		UpdateWindow(Pixels, width, height);

		//Reset to Gray
		ResetBuffer(width, height);

		//currTime is the prevtTime
		prevTime = currTime;
	}
	clock_gettime(CLOCK_MONOTONIC, &timeobj);
	double endTime = timeobj.tv_sec*1000.0 + timeobj.tv_nsec/1000000.0;
	double finalTime = (endTime-startTime)/500.0;
	printf("Rendering at %fms per frame on avg\n", finalTime);

	//Release Pixels
	ReleasePixels(width, height);
	ReleaseTriangleDataOnly(tris);
}