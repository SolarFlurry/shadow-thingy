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

Vec3 vec3(float x, float y, float z);
Vec3 vec3Lerp(Vec3 p0, Vec3 p1, float t);

int index2D(int* grid, size_t x, size_t y, size_t width, size_t height, int outBounds);

VertexBuffer fromGrid(int* grid, size_t gridWidth, size_t gridHeight);