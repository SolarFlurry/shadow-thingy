#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "type/type.h"

typedef struct {
	Vec3 pos;
	unsigned int adjacent;
} Vertex;

typedef struct {
	Vec3 normal;
	Vertex vertices[4];
} Face;

typedef struct {
	Face* data;
	size_t len;
	size_t capacity;
} FaceBuffer;

FaceBuffer newBuffer();
void freeBuffer(FaceBuffer* buffer);
void bufferPush(FaceBuffer* buffer, Face item);
Face* bufferGet(FaceBuffer* buffer, size_t index);

int index3D(int* grid, Vec3 pos, Vec3 size);

FaceBuffer fromGrid(int* grid, size_t width, size_t height, size_t depth);