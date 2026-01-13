#include "block.h"

VertexBuffer newBuffer() {
	VertexBuffer buffer = {0};
	buffer.capacity = 1;
	buffer.len = 0;
	buffer.data = malloc(sizeof(Vertex));
	return buffer;
}

void freeBuffer(VertexBuffer* buffer) {
	free(buffer->data);
}

void bufferPush(VertexBuffer* buffer, Vertex item) {
	if (buffer->len >= buffer->capacity) {
		buffer->data = realloc(buffer->data, buffer->capacity * 2 * sizeof(Vertex));
		buffer->capacity *= 2;
	}
	buffer->data[buffer->len] = item;
	buffer->len += 1;
}

Vertex* bufferGet(VertexBuffer* buffer, size_t index) {
	if (index >= buffer->len) return NULL; // the buffer code is really bad ok dont judge
	return &buffer->data[index];
}

int index2D(int* grid, size_t x, size_t y, size_t width, size_t height, int outBounds) {
	if (x < 0 || x >= width || y < 0 || y >= width) return outBounds;

	return grid[y * width + x];
}

VertexBuffer fromGrid(int* grid, size_t width, size_t height) {
	VertexBuffer vertices = newBuffer();
	for (int j = 0; j <= height; j++) {
		for (int i = 0; i <= width; i++) {
			float x = i - width/2.0;
			float y = j - height/2.0;
			if (
				index2D(grid, i, j, width, height, 1) !=
				index2D(grid, i, j - 1, width, height, 1)
			) {
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 1), .adjacent = 2});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y, 1), .adjacent = 2});
			}
			if (
				index2D(grid, i, j, width, height, 1) !=
				index2D(grid, i - 1, j, width, height, 1)
			) {
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y + 1, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y + 1, 1), .adjacent = 2});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 1), .adjacent = 2});
			}
			int top = index2D(grid, i, j - 1, width, height, 1);
			int left = index2D(grid, i - 1, j, width, height, 1);
			int bottom = index2D(grid, i, j + 1, width, height, 1);
			int right = index2D(grid, i + 1, j, width, height, 1);
			int z = 1 - index2D(grid, i, j, width, height, 1);
			bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, z), .adjacent = z * (top + left + index2D(grid, i-1, j-1, width, height, 1))});
			bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y, z), .adjacent = z * (top + right + index2D(grid, i+1, j-1, width, height, 1))});
			bufferPush(&vertices, (Vertex) {.pos = vec3(x, y + 1, z), .adjacent = z * (bottom + left + index2D(grid, i-1, j+1, width, height, 1))});
			bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y + 1, z), .adjacent = z * (bottom + right + index2D(grid, i+1, j+1, width, height, 1))});
		}
	}

	return vertices;
}