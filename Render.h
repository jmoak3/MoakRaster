#ifndef RENDER_H
#define RENDER_H

static int ** Pixels;

void InitializePixels(int width, int height);
void ReleasePixels();
void RenderMesh(int width, int height, char *fileName, Vector3 camLocation);

#endif