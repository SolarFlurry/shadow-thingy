#include "block.h"

FaceBuffer newBuffer() {
	FaceBuffer buffer = {0};
	buffer.capacity = 1;
	buffer.len = 0;
	buffer.data = malloc(sizeof(Face));
	return buffer;
}

void freeBuffer(FaceBuffer* buffer) {
	free(buffer->data);
}

void bufferPush(FaceBuffer* buffer, Face item) {
	if (buffer->len >= buffer->capacity) {
		buffer->capacity *= 2;
		buffer->data = realloc(buffer->data, buffer->capacity * sizeof(Face));
	}
	buffer->data[buffer->len] = item;
	buffer->len += 1;
}

Face* bufferGet(FaceBuffer* buffer, size_t index) {
	if (index >= buffer->len) return NULL; // the buffer code is really bad ok dont judge
	return &buffer->data[index];
}

int index3D(int* grid, Vec3 pos, Vec3 size) {
	if (pos.z < 0) {
		return 0;
	}
	if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y || pos.z >= size.z) {
		return 1;
	}
	return grid[((int)pos.z * (int)size.y + (int)pos.y) * (int)size.x + (int)pos.x];
}

int vertexAO(int side0, int side1, int corner) {
	if (side0 && side1) {
		return 3;
	}
	return side0 + side1 + corner;
}

FaceBuffer fromGrid(int* grid, size_t width, size_t height, size_t depth) {
	FaceBuffer faces = newBuffer();

	Vec3 size = vec3(width, height, depth);

	for (int k = 0; k <= depth; k++) {
		for (int j = 0; j <= height; j++) {
			for (int i = 0; i <= width; i++) {
				float x = i - width/2.0;
				float y = j - height/2.0;

				Vec3 pos = vec3(i, j, k);
				Vec3 screenSpace = vec3(x, y, k);

				int middle = index3D(grid, vec3(i, j, k), size);

				Vec3 axes[3][3] = {
					{vec3(0, 1, 0), vec3(0, 0, 1), vec3(1, 0, 0)},
					{vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 1, 0)},
					{vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1)}
				};

				for (int i = 0; i < 3; i++) {
					Vec3 axisX = axes[i][0];
					Vec3 axisY = axes[i][1];
					Vec3 axisZ = axes[i][2];
					if (index3D(grid, vec3Sub(pos, axisZ), size) != middle) {
						Vec3 current = pos;
						Vec3 normal = axisZ;
						if (middle == 1) {
							current = vec3Sub(current, axisZ);
							normal = vec3(-normal.x, -normal.y, -normal.z);
						}

						if (normal.z > 0) continue;

						int top = index3D(grid, vec3Sub(current, axisY), size);
						int left = index3D(grid, vec3Sub(current, axisX), size);
						int bottom = index3D(grid, vec3Add(current, axisY), size);
						int right = index3D(grid, vec3Add(current, axisX), size);

						int topleft = index3D(grid, vec3Sub(vec3Sub(current, axisX), axisY), size);
						int topright = index3D(grid, vec3Sub(vec3Add(current, axisX), axisY), size);
						int bottomleft = index3D(grid, vec3Add(vec3Sub(current, axisX), axisY), size);
						int bottomright = index3D(grid, vec3Add(vec3Add(current, axisX), axisY), size);
						
						Face face = (Face) {
							.normal = normal,
							.vertices = {
								(Vertex) {.pos = screenSpace, .adjacent = vertexAO(top, left, topleft)},
								(Vertex) {.pos = vec3Add(screenSpace, axisX), .adjacent = vertexAO(top, right, topright)},
								(Vertex) {.pos = vec3Add(screenSpace, axisY), .adjacent = vertexAO(bottom, left, bottomleft)},
								(Vertex) {.pos = vec3Add(vec3Add(screenSpace, axisX), axisY), .adjacent = vertexAO(bottom, right, bottomright)}
							}
						};

						bufferPush(&faces, face);
					}
				}
			}
		}
	}

	return faces;
}