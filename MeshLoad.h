#ifndef MESHLOAD_H
#define MESHLOAD_H

#include "3DMath.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

int LoadMesh(char * fileName, int ** pindices, Vector3 ** ppoints, int *numInds, int *numPts)
{
	char header[64];
	FILE *fp = fopen(fileName, "r");
	//printf("File formed!\n");
	*numPts = 0;
	*numInds = 0;
	int sizePts = 36;
	int sizeInds = 12;
	Vector3 *points = malloc(sizeof(Vector3)*sizePts);
	int *indices = malloc(sizeof(int)*sizeInds);
	//printf("numInds and numPts initialized!\n");
	int done = 0;
	while (!feof(fp) && !done)
	{
		char data[64];
		fscanf(fp, "%s", header);
		if (strcmp(header, "v")==0)
		{
			//printf("Adding Vertices:");
			fscanf(fp, "%s", data);
			points[*numPts].x = atof(data);
			fscanf(fp, "%s", data);
			points[*numPts].y = atof(data);
			fscanf(fp, "%s", data);
			points[*numPts].z = atof(data);
			//printf("%f %f %f", points[*numPts].x, points[*numPts].y, points[*numPts].z);
			++(*numPts);
			if (*numPts >= sizePts) 
			{
				Vector3 * temp = (Vector3*)malloc(sizeof(Vector3)*sizePts*2);
				memcpy(temp, points, sizeof(Vector3)*sizePts);
				Vector3 * tempPoints = points;
				points = temp;
				free(tempPoints);
				tempPoints = 0;			
				temp = 0;					
				sizePts = 2*sizePts;
			}
			//printf("\n");
		}
		else if (strcmp(header, "f")==0)
		{
			//printf("Adding Indices:");
			//f 239/408/237 239/408/237 239/408/237 239/408/23
			int i=0;
			for (i=0;i<3;++i)
			{
				char read[128];
				char tri[128];
				int j=0;
				for (j=0;j<128;++j)
				{
					read[j] = 0;
					tri[j] = 0;
				}
				fscanf(fp, "%s", read);
				////printf("%s\n", read);

				for (j=0;j<128;++j)
				{
					if (read[j] == '/' && read[j+1] == '/')
					{
						j+=2;
						break;
					}
					tri[j] = read[j];
				}
				int bummy = atoi(tri);
				indices[*numInds] = bummy-1;
				//printf("%i ", indices[*numInds]);
				++(*numInds);
				//printf("%i", *numInds);
				if (*numInds >= sizeInds) 
				{
					int * temp = (int*)malloc(sizeof(int)*sizeInds*2);
					memcpy(temp, indices, sizeof(int)*sizeInds);
					int * tempInds = indices;
					indices = temp;
					free(tempInds);
					tempInds = 0;			
					temp = 0;			
					sizeInds = 2*sizeInds;
				}
			}
			//printf("\n");
		}
		else if (strcmp(header, "end")==0)
		{
			done = 1;
		}
	}
	fclose(fp);
	//printf("End of reading\n");

	//*pindices = malloc(sizeof(int)*(*numInds));
	//*ppoints = malloc(sizeof(Vector3)*(*numPts));

	//memcpy(*pindices, indices, sizeof(int)*(*numInds));
	//memcpy(*ppoints, points, sizeof(Vector3)*(*numPts));
	*ppoints = points;
	*pindices = indices;

	//printf("Copied pointers \n");

	if (1)
	{	
		int i=0;
		fp = fopen("test.txt", "w");
		for (i=0;i<*numPts;i+=1)
		{
			fprintf(fp, "%f, %f, %f\n", (*ppoints)[i].x, (*ppoints)[i].y, (*ppoints)[i].z);
		}
		for (i=0;i<*numInds;i+=3)
		{
			fprintf(fp, "%i, %i, %i\n", (*pindices)[i], (*pindices)[i+1], (*pindices)[i+2]);
		}
		fclose(fp);
	}
	return 1;
}

#endif
