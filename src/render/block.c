#include "block.h"

Vec3 vec3(float x, float y, float z) {
	Vec3 vec = {0};
	vec.x = x;
	vec.y = y;
	vec.z = z;

	return vec;
}

Vec3* fromGrid(int* grid, size_t gridWidth, size_t gridHeight, size_t* size) {
	*size = ((gridWidth + 1) * (gridHeight + 1)) * 2;
	Vec3* vertices = calloc(*size, sizeof(Vec3));
	for (int j = 0; j <= gridHeight; j++) {
		for (int i = 0; i <= gridWidth; i++) {
			vertices[(j * (gridWidth + 1) + i) * 2] = vec3(i, j, 0);
			vertices[(j * (gridWidth + 1) + i) * 2 + 1] = vec3(i, j, 1);
		}
	}

	return vertices;
}