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
	Vertex* data;
	size_t len;
	size_t capacity;
} VertexBuffer;

VertexBuffer newBuffer();
void freeBuffer(VertexBuffer* buffer);
void bufferPush(VertexBuffer* buffer, Vertex item);
Vertex* bufferGet(VertexBuffer* buffer, size_t index);

typedef struct {
	Vertex* vertices[4];
} Face;

int index3D(int* grid, Vec3 pos, Vec3 size);

VertexBuffer fromGrid(int* grid, size_t width, size_t height, size_t depth);