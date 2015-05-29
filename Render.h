#ifndef RENDER_H
#define RENDER_H
#include "3DMath.h"
#include "TriangleMesh.h"

static char * Pixels;

void InitializePixels(int width, int height);
void ReleasePixels();
void RenderMesh(int width, int height, Triangle *tris);
void Render(int width, int height, char *fileName, Vector3 camLocation);

#endif