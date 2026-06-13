#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "type/type.h"
#include "render.h"

typedef struct {
	Vec3_int pos;
	unsigned int adjacent;
} Vertex;

typedef struct {
	Vec3_int normal;
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

int index3D(Grid grid, Vec3_int pos);

FaceBuffer fromGrid(Grid grid);
