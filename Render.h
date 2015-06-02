#ifndef RENDER_H
#define RENDER_H
#include "3DMath.h"
#include "TriangleMesh.h"

static char * Pixels;
static float * DepthBuffer;

void InitializePixels(int width, int height);
void ReleasePixels();
void ResetBuffer(int width, int height);
void BuildRasterTransform(Transform *rt, int width, int height);
void RenderMesh(int width, int height, Triangle *tris);
void Render(int width, int height, char *fileName, Vector3 camLocation);

#endif