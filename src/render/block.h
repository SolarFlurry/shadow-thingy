#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	Vec3* vertices[4];
} Face;

Vec3 vec3(float x, float y, float z);

Vec3* fromGrid(int* grid, size_t gridWidth, size_t gridHeight, size_t* size);