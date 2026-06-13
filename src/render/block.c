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

int index3D(Grid grid, Vec3_int pos) {
	if (pos.z < 0) {
		return 0;
	}
	if (pos.x < 0 || pos.x >= grid.width || pos.y < 0 || pos.y >= grid.height || pos.z >= grid.depth) {
		return 1;
	}
	return grid.data[(pos.z * grid.height + pos.y) * grid.width + pos.x];
}

int vertexAO(int side0, int side1, int corner) {
	if (side0 && side1) {
		return 3;
	}
	return side0 + side1 + corner;
}

FaceBuffer fromGrid(Grid grid) {
	FaceBuffer faces = newBuffer();

	Vec3_int size = vec3_int(grid.width, grid.height, grid.depth);

	for (int k = 0; k <= grid.depth; k++) {
		for (int j = 0; j <= grid.height; j++) {
			for (int i = 0; i <= grid.width; i++) {
				float x = i - grid.width/2.0;
				float y = j - grid.height/2.0;

				Vec3_int pos = vec3_int(i, j, k);
				Vec3_int screenSpace = vec3_int(x, y, k);

				int middle = index3D(grid, pos);

				int axes[27] = {
                    0, 1, 0, /**/ 0, 0, 1, /**/ 1, 0, 0,
                    0, 0, 1, /**/ 1, 0, 0, /**/ 0, 1, 0,
                    1, 0, 0, /**/ 0, 1, 0, /**/ 0, 0, 1,
				};

				for (int i = 0; i < 3; i++) {
					Vec3_int axisX = vec3_int(axes[i * 9 + 0], axes[i * 9 + 1], axes[i * 9 + 2]);
					Vec3_int axisY = vec3_int(axes[i * 9 + 3], axes[i * 9 + 4], axes[i * 9 + 5]);
					Vec3_int axisZ = vec3_int(axes[i * 9 + 6], axes[i * 9 + 7], axes[i * 9 + 8]);
					if (index3D(grid, vec3Sub_int(pos, axisZ)) != middle) {
						Vec3_int current = pos;
						Vec3_int normal = axisZ;
						if (middle == 1) {
							current = vec3Sub_int(current, normal);
							normal = vec3_int(-normal.x, -normal.y, -normal.z);
						}

						if (normal.z > 0) continue;

						int top = index3D(grid, vec3Sub_int(current, axisY));
						int left = index3D(grid, vec3Sub_int(current, axisX));
						int bottom = index3D(grid, vec3Add_int(current, axisY));
						int right = index3D(grid, vec3Add_int(current, axisX));

						int topleft = index3D(grid, vec3Sub_int(vec3Sub_int(current, axisX), axisY));
						int topright = index3D(grid, vec3Sub_int(vec3Add_int(current, axisX), axisY));
						int bottomleft = index3D(grid, vec3Add_int(vec3Sub_int(current, axisX), axisY));
						int bottomright = index3D(grid, vec3Add_int(vec3Add_int(current, axisX), axisY));
						
						Face face = (Face) {
							.normal = normal,
							.vertices = {
								(Vertex) {.pos = screenSpace, .adjacent = vertexAO(top, left, topleft)},
								(Vertex) {.pos = vec3Add_int(screenSpace, axisX), .adjacent = vertexAO(top, right, topright)},
								(Vertex) {.pos = vec3Add_int(screenSpace, axisY), .adjacent = vertexAO(bottom, left, bottomleft)},
								(Vertex) {.pos = vec3Add_int(vec3Add_int(screenSpace, axisX), axisY), .adjacent = vertexAO(bottom, right, bottomright)}
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
