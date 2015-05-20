#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "3DMath.h"

typedef struct
{
	Vector2 min;
	Vector2 max;
} BoundingBox2D;


void Union(BoundingBox *b1, BoundingBox *b2, BoundingBox *outb);
bool DoesIntersectBBox2D(BoundingBox2D *bbox, Ray * ray);

#endif
