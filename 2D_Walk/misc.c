#include "misc.h"

int** get2dMatrix(int width, int height)
{
	int* matrix = calloc(width*height, sizeof(int));
	if (matrix == NULL) return NULL;

	int** indices = calloc(height, sizeof(int));
	if (indices == NULL) return indices;

	for (int i = 0; i < height; i++)
		indices[i] = matrix + i*width;
	return indices;
}