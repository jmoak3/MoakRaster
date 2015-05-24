#ifndef MESHLOAD_H
#define MESHLOAD_H

#include "3DMath.h"
#include <stdio.h>
#include <malloc.h>

int LoadMesh(char * fileName, int * indices, Vector3 * points, int *numInds, int *numPts, int sizePts, int sizeInds)
{
	char header[64];
	FILE *fp = fopen(fileName, "r");
	*numPts = 0;
	*numInds = 0;
	while (!feof(fp))
	{
		float dummy = 0.f;
		char data[64];
		fscanf(fp, "%s", data);
		
		if (strcmp(header, "v")==0)
		{
			fscanf(fp, "%s", data);
			points[*numPts].x = atof(data);
			fscanf(fp, "%s", data);
			points[*numPts].y = atof(data);
			fscanf(fp, "%s", data);
			points[*numPts].z = atof(data);
			++(*numPts);
			if (*numPts >= sizePts) 
			{
				Vector3 * temp = (Vector3*)malloc(sizeof(Vector3)*sizePts*2);
				int i=0;
				for (i=0;i<sizePts;++i)
					temp[i] = points[i];
				free(points);
				points = temp;						
				sizePts = 2*sizePts;
			}
			//printf("\n");
		}
		else if (strcmp(header, "f")==0)
		{
			//printf("Adding Indices:");
			int i=0;
			for (i=0;i<3;++i)
			{
				char read[64];
				char tri[64];
				int j=0;
				for (j=0;j<64;++j)
				{
					read[j] = 0;
					tri[j] = 0;
				}
				fscanf(fp, "%s", data);
				//printf("%s\n", read);
				for (j=0;j<64;++j)
				{
					if (read[j] == '/' && read[j+1] == '/')
					{
						j+=2;
						break;
					}
					tri[j] = read[j];
				}
				int bummy = atoi(tri);
				//printf(" %i", bummy);
				//assert(bummy-1>-1);
				//assert(bummy<*numPts);
				indices[*numInds] = bummy-1;
				++(*numInds);
				if (*numInds >= sizeInds) 
				{
					int * temp = (int*)malloc(sizeof(int)*sizeInds*2);
					int i=0;
					for (i=0;i<sizeInds;++i)
						temp[i] = indices[i];
					free(indices);
					indices = temp;						
					sizeInds = 2*sizeInds;
				}
			}
			//printf("\n");
		}
	}
	fclose(fp);

	if (1)
	{	
		int i=0;
		fp = fopen(fileName, "r");
		for (i=0;i<*numPts;i+=1)
		{
			fprintf(fp, "%f, %f, %f\n", points[i].x, points[i].y, points[i].z);
		}
		for (i=0;i<*numInds;i+=3)
		{
			fprintf(fp, "%f, %f, %f\n", indices[i], points[i+1], points[i+2]);
		}
		fclose(fp);
	}
}

#endif
