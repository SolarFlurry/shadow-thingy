#include "render.h"

#include <stdio.h>
#include <stdbool.h>

Pixel pixel(float r, float g, float b, float a, float depth) {
	Pixel pixel;
	pixel.color.r = r;
	pixel.color.g = g;
	pixel.color.b = b;
	pixel.color.a = a;
	pixel.depth = depth;
	return pixel;
}

void write(Pixel* pixel, Pixel writee) {
	if (writee.depth >= pixel->depth) {
		pixel->color = writee.color;
		pixel->depth = writee.depth;
	}
}

Vec3 project(Vec3 p, Camera c) {
	Vec3 relative = vec3(p.x - c.pos.x, p.y - c.pos.y, p.z - c.pos.z);
	if (relative.z <= 0) return vec3(0, 0, -1);
	float x = (c.focalLength * relative.x) / (c.focalLength + relative.z);
	float y = (c.focalLength * relative.y) / (c.focalLength + relative.z);
	return vec3(x, y, relative.z);
}

void render(unsigned char* image, size_t width, size_t height) {
	int* grid = (int*)calloc(10 * 10 * 10, sizeof(int));

	for (int k = 0; k < 10; k++) {
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 10; i++) {
				// if (i == 0 || i == 9 || j == 0 || j == 9) {
				// 	grid[i * 10 + j] = 0;
				// } else {
				// 	grid[i * 10 + j] = 1;
				// }
				if (i == j && j == k) {
					grid[(k * 10 + j) * 10 + i] = 1;
				} else {
					grid[(k * 10 + j) * 10 + i] = 0;
				}
			}
		}
	}

	Pixel* pixels = calloc(width * height, sizeof(Pixel));
	for (int i = 0; i < width * height; i++) {
		pixels[i] = pixel(0, 0, 0, 0, -INFINITY);
	}

	VertexBuffer vertices = fromGrid(grid, 10, 10, 10);

	Camera camera;
	camera.pos = vec3(0, 0, -1);
	camera.focalLength = 10;

	float scaleFactor = 50.0;
	float shadowRatio = 1/6.0;

	for (int i = 0; i < vertices.len; i+=4) {
		bool shouldContinue = false;
		Vec3 face[4];
		int adjacent[4];
		for (int j = 0; j < 4; j++) {
			Vertex* vertex = bufferGet(&vertices, i + j);
			Vec3 projected = project(vertex->pos, camera);
			if (projected.z == -1) {
				shouldContinue = true;
				break;
			}
			int x = (int)(scaleFactor * projected.x) + width/2;
			int y = (int)(scaleFactor * projected.y) + height/2;
			face[j] = vec3(x, y, projected.z);
			adjacent[j] = vertex->adjacent;
		}
		if (shouldContinue) continue;
		Vec3 v0 = bufferGet(&vertices, i + 0)->pos;
		Vec3 v1 = bufferGet(&vertices, i + 1)->pos;
		Vec3 v2 = bufferGet(&vertices, i + 2)->pos;
		Vec3 a = vec3(
			v1.x - v0.x,
			v1.y - v0.y,
			v1.z - v0.z
		);
		Vec3 b = vec3(
			v2.x - v0.x,
			v2.y - v0.y,
			v2.z - v0.z
		);
		float shading = fabs(normalise(vec3(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		)).z);
		shading = (1 - shading) * 0.3;
		trifill(pixels,
			(FillVertex) {
				.pos = face[0],
				.color = color(0, 0, 0, adjacent[0]*shadowRatio + shading)
			},
			(FillVertex) {
				.pos = face[1],
				.color = color(0, 0, 0, adjacent[1]*shadowRatio + shading)
			},
			(FillVertex) {
				.pos = face[2],
				.color = color(0, 0, 0, adjacent[2]*shadowRatio + shading)
			},
			width, height
		);
		trifill(pixels,
			(FillVertex) {
				.pos = face[1],
				.color = color(0, 0, 0, adjacent[1]*shadowRatio + shading)
			},
			(FillVertex) {
				.pos = face[2],
				.color = color(0, 0, 0, adjacent[2]*shadowRatio + shading)
			},
			(FillVertex) {
				.pos = face[3],
				.color = color(0, 0, 0, adjacent[3]*shadowRatio + shading)
			},
			width, height
		);
	}

	for (int i = 0; i < width * height; i++) {
		image[i * 4 + 0] = pixels[i].color.r * 255;
		image[i * 4 + 1] = pixels[i].color.g * 255;
		image[i * 4 + 2] = pixels[i].color.b * 255;
		image[i * 4 + 3] = pixels[i].color.a * 255;
	}

	freeBuffer(&vertices);
	free(grid);
	free(pixels);
}