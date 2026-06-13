#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "type/type.h"
#include "tri/fill.h"

typedef struct Grid {
    int* data;
    int width;
    int height;
    int depth;
} Grid;

typedef struct Pixel {
	Color color;
	float depth;
} Pixel;

Pixel pixel(float r, float g, float b, float a, float depth);
void write(Pixel* pixel, Pixel writee);

typedef struct {
	Vec3_float pos;
	float focalLength;
} Camera;

void render(unsigned char* image, size_t width, size_t height);
Vec3_float project(Vec3_float p, Camera c);
