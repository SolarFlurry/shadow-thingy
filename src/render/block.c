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

VertexBuffer fromGrid(int* grid, size_t gridWidth, size_t gridHeight) {
	VertexBuffer vertices = newBuffer();
	for (int j = 0; j <= gridHeight; j++) {
		for (int i = 0; i <= gridWidth; i++) {
			float x = i - gridWidth/2.0;
			float y = j - gridHeight/2.0;
			if (
				index2D(grid, i, j, gridWidth, gridHeight, 1) !=
				index2D(grid, i, j - 1, gridWidth, gridHeight, 1)
			) {
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 1), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x + 1, y, 1), .adjacent = 0});
			}
			if (
				index2D(grid, i, j, gridWidth, gridHeight, 1) !=
				index2D(grid, i - 1, j, gridWidth, gridHeight, 1)
			) {
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y + 1, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 0), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y + 1, 1), .adjacent = 0});
				bufferPush(&vertices, (Vertex) {.pos = vec3(x, y, 1), .adjacent = 0});
			}
		}
	}

	return vertices;
}