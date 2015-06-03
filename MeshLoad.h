#ifndef MESHLOAD_H
#define MESHLOAD_H

#include "3DMath.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

int LoadMesh(char * fileName, int ** pindices, Vector3 ** ppoints, 
	         Vector2 ** ptex, int ** ptexinds, 
	         Vector3 ** pnormals, int ** pnorminds,
	         int *numInds, int *numPts, int *numTex, int *numTexInds, 
	         int *numNormals, int *numNormalInds)
{
	char header[64];
	FILE *fp = fopen(fileName, "r");
	printf("File found!\n");
	*numPts = 0;
	*numInds = 0;
	*numTex = 0;
	*numTexInds = 0;
	*numNormals = 0;
	*numNormalInds = 0;
	int sizePts = 36;
	int sizeInds = 12;
	int sizeTex = 36;
	int sizeTexInds = 12;
	int sizeNormals = 36;
	int sizeNormalInds = 12;
	Vector3 *points = malloc(sizeof(Vector3)*sizePts);
	int *indices = malloc(sizeof(int)*sizeInds);
	Vector3 *normals = malloc(sizeof(Vector3)*sizeNormals);
	int *normalInds = malloc(sizeof(int)*sizeNormalInds);
	Vector2 *tex = malloc(sizeof(Vector2)*sizeTex);
	int *texInds = malloc(sizeof(int)*sizeTexInds);
	//printf("nums and temp ptrs initialized!\n");
	int vVtVn = 0;
	int done = 0;
	while (!feof(fp) && !done)
	{
		char data[128];
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
		else if (strcmp(header, "vn")==0)
		{
			vVtVn = 1;
			//printf("Adding Normals:");
			fscanf(fp, "%s", data);
			normals[*numNormals].x = atof(data);
			fscanf(fp, "%s", data);
			normals[*numNormals].y = atof(data);
			fscanf(fp, "%s", data);
			normals[*numNormals].z = atof(data);
			NormalizeVec3(&(normals[*numNormals]), &(normals[*numNormals]));
			//printf("%f %f %f", 	normals[*numNormals].x, 
			//	normals[*numNormals].y, normals[*numNormals].z);
			++(*numNormals);
			if (*numNormals >= sizeNormals)
			{
				Vector3 * temp = (Vector3*)malloc(sizeof(Vector3)*sizeNormals*2);
				memcpy(temp, normals, sizeof(Vector3)*sizeNormals);
				Vector3 * tempNormals = normals;
				normals = temp;
				free(tempNormals);
				tempNormals = 0;
				temp = 0;
				sizeNormals = 2*sizeNormals;
			}
			//printf("\n");
		}
		else if (strcmp(header, "vt")==0)
		{
			vVtVn = 1;
			//printf("Adding TexCoords:");
			fscanf(fp, "%s", data);
			tex[*numTex].x = atof(data);
			fscanf(fp, "%s", data);
			tex[*numTex].y = atof(data);
			//printf("%f %f", tex[*numTex].x, tex[*numTex].y);
			++(*numTex);
			if (*numTex >= sizeTex)
			{
				Vector2 * temp = (Vector2*)malloc(sizeof(Vector2)*sizeTex*2);
				memcpy(temp, tex, sizeof(Vector2)*sizeTex);
				Vector2 * tempTex = tex;
				tex = temp;
				free(tempTex);
				tempTex = 0;
				temp = 0;
				sizeTex = 2*sizeTex;
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
				//printf("Read %s\n", read);

				if (!vVtVn)
				{
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
					//printf("New Ind %i ", indices[*numInds]);
					++(*numInds);
					//printf("NumInds %i", *numInds);
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
				else 
				{
					int k; j=0;
					for (k=0;k<3;++k)
					{ 
						int t=0;
						for (t=0;t<128;++t)
							tri[t] = 0;
						t=0;
						for (;j<128;++j)
						{
							if (read[j] == '/' || read[j] == ' ')
							{
								j+=1;
								break;
							}
							tri[t] = read[j];
							++t;
						}
						int bummy = atoi(tri);
						if (k==0)
						{ 
							indices[*numInds] = bummy-1;
							//printf("NewInd %i ", indices[*numInds]);
							++(*numInds);
							//printf("NumInds %i", *numInds);
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
							//printf("\n");
						}
						if (k==1)
						{
							texInds[*numTexInds] = bummy-1;
							//printf("NewTexInd %i ", texInds[*numTexInds]);
							++(*numTexInds);
							//printf("NumTexInd %i", *numTexInds);
							if (*numTexInds >= sizeTexInds)
							{
								int * temp = (int*)malloc(sizeof(int)*sizeTexInds*2);
								memcpy(temp, texInds, sizeof(int)*sizeTexInds);
								int * tempInds = texInds;
								texInds = temp;
								free(tempInds);
								tempInds = 0;
								temp = 0;
								sizeTexInds = 2*sizeTexInds;
							}
							//printf("\n");
						}
						if (k==2)
						{
							normalInds[*numNormalInds] = bummy-1;
							//printf("NewNormalInd %i ", normalInds[*numInds]);
							++(*numNormalInds);
							//printf("numNormalInds %i", *numNormalInds);
							if (*numNormalInds >= sizeNormalInds)
							{
								int * temp = (int*)malloc(sizeof(int)*sizeNormalInds*2);
								memcpy(temp, normalInds, sizeof(int)*sizeNormalInds);
								int * tempInds = normalInds;
								normalInds = temp;
								free(tempInds);
								tempInds = 0;
								temp = 0;
								sizeNormalInds = 2*sizeNormalInds;
							}
							//printf("\n");
						}
					}
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
	printf("End of reading\n");

	//*pindices = malloc(sizeof(int)*(*numInds));
	//*ppoints = malloc(sizeof(Vector3)*(*numPts));

	//memcpy(*pindices, indices, sizeof(int)*(*numInds));
	//memcpy(*ppoints, points, sizeof(Vector3)*(*numPts));
	*ppoints = points;
	*pindices = indices;
	*ptex = tex;
	*ptexinds = texInds;
	*pnormals = normals;
	*pnorminds = normalInds;

	printf("Copied pointers \n");
    
	if (0)
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
