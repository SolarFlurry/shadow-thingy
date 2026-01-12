#include "render.h"

Pixel pixel(float r, float g, float b, float a, float depth) {
	Pixel pixel;
	pixel.r = r;
	pixel.g = g;
	pixel.b = b;
	pixel.a = a;
	pixel.depth = depth;
	return pixel;
}

void write(Pixel* pixel, Pixel writee) {
	if (writee.depth <= pixel->depth) {
		pixel->r = writee.r;
		pixel->g = writee.g;
		pixel->b = writee.b;
		pixel->a = writee.a;
		pixel->depth = writee.depth;
	}
}

void render(unsigned char* image, size_t width, size_t height) {
	int* grid = (int*)calloc(10 * 10, sizeof(int));

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (i == 0 || i == 9 || j == 0 || j == 9) {
				grid[i * 10 + j] = 1;
			} else {
				grid[i * 10 + j] = 0;
			}
		}
	}

	Pixel* pixels = calloc(width * height, sizeof(Pixel));
	for (int i = 0; i < width * height; i++) {
		pixels[i] = pixel(0, 0, 0, 0, 0);
	}

	size_t verticesSize;
	Vec3* vertices = fromGrid(grid, 10, 10, &verticesSize);

	Camera camera;
	camera.pos = vec3(0, 0, -1);
	camera.focalLength = 10;

	for (int i = 0; i < verticesSize; i++) {
		Vec3 vertex = vertices[i];
		Vec3 relative = vec3(vertex.x - camera.pos.x, vertex.y - camera.pos.y, vertex.z - camera.pos.z);
		if (relative.z <= 0) continue;
		int x = (int)(camera.focalLength * (relative.x-5)/relative.z);
		int y = (int)(camera.focalLength * (relative.y-5)/relative.z);

		x += width/2;
		y += height/2;

		if (x < 0 || x >= width || y < 0 || y >= height) continue;

		write(&pixels[y * width + x], pixel(255, 0, 0, 255, 0));
	}

	for (int i = 0; i < width * height; i++) {
		image[i * 4 + 0] = pixels[i].r;
		image[i * 4 + 1] = pixels[i].g;
		image[i * 4 + 2] = pixels[i].b;
		image[i * 4 + 3] = pixels[i].a;
	}

	free(vertices);
	free(grid);
	free(pixels);
}