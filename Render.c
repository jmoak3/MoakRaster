#include "Render.h"
#include "Window.h"
#include "TriangleMesh.h"
#include "time.h"

void InitializePixels(int width, int height)
{
	//initiialize pixels - 2D array 
	Pixels = malloc(sizeof(char*)*height*width*3);
	DepthBuffer = malloc(sizeof(char*)*height*width);
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
			DepthBuffer[i*width + j] = INFINITY;
		}
	}
}

void RenderMesh(int width, int height, Triangle *tris)
{
    int column, row, i;
	int numTris = tris[0].mesh->numTris;
	Hit hit; Vector2 pt, V0V1, V2V0; int top,bot,lft,rgt; //preallocate
	Vector3 a,b,c; float avgZ;
	for (i=0;i<numTris;++i)
	{
		top = tris[i].bbox.min.y-1;
		bot = Minimum(tris[i].bbox.max.y+1, height);
		lft = tris[i].bbox.min.x-1;
		rgt = Minimum(tris[i].bbox.max.x+1, width);
		//printf("t %i b %i l %i r %i\n", t,b,l,r);

		//Check if back-facing!
		a = tris[i].mesh->vertPoints[tris[i].vert[0]];
		b = tris[i].mesh->vertPoints[tris[i].vert[1]];
		c = tris[i].mesh->vertPoints[tris[i].vert[2]];
		V0V1; V0V1.x = b.x-a.x; V0V1.y = b.y-a.y;
		V2V0; V2V0.x = a.x-c.x; V2V0.y = a.y-c.y;
		if (DetVec2(&V0V1, &V2V0) < 0.f) continue;

		for (row=top;row<bot;++row)
		{
			for (column=lft;column<rgt;++column)
			{
				pt.x = column; pt.y = row;
				if (DoesPointLieOnTri(&(tris[i]), &pt, &hit))
				{
					avgZ = (a.z+b.z+c.z)/3.f;
					if (avgZ < DepthBuffer[row*width + column])
					{
						DepthBuffer[row*width + column] = avgZ;
						Pixels[row*width*3 + column*3 + 0] = (char)hit.material.red;
						Pixels[row*width*3 + column*3 + 1] = (char)hit.material.green;
						Pixels[row*width*3 + column*3 + 2] = (char)hit.material.blue;
						//printf("Rendered %f %f %f\n", 
						//		hit.material.red,
						//		hit.material.green,
						//		hit.material.blue);
					}
				}
			}
		}
	}	
}

void Render(int width, int height, char *fileName, Vector3 cam)
{
	
	Transform rasterTrans;
	Transform meshRotX = RotateX( 90.f);
	Vector3 offset; offset.x = 0.f; offset.y = 1.f; offset.z = 0.3f;
	Transform meshOffset = MakeTranslation(&offset);
    TransformTrans(&meshOffset, &meshRotX, &rasterTrans);
	float temp = 0.1f/offset.z;
	Vector3 v; v.x = temp; v.y = -temp; v.z = 1.f;
	Transform t = MakeScale(&v);
    TransformTrans(&t, &rasterTrans, &rasterTrans);
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
	printf("TriangleMesh Formed correctly\n");
	Triangle * tris = malloc(sizeof(Triangle)*mesh.numTris);
	GetTrianglesFromMesh(&mesh, tris);
	printf("Triangles Harvested correctly\n");
	
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
	long i; for (i=0;i<300;++i) 
	{
		clock_gettime(CLOCK_MONOTONIC, &timeobj);
		currTime = timeobj.tv_sec + timeobj.tv_nsec/1000000000.0;
		
		dt = Maximum(currTime-prevTime, 0.00001f); 
		
		//Make him move a bit
		move.x = 50.f*dt; move.y = 0.f; move.z = 0.f;
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
	double finalTime = (endTime-startTime)/300.0;
	printf("Rendering at %fms per frame on avg\n", finalTime);

	//Release Pixels
	ReleasePixels(width, height);
	ReleaseTriangleDataOnly(tris);
}