#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "type/type.h"
#include "tri/fill.h"
#include "block.h"

typedef struct Pixel {
	Color color;
	float depth;
} Pixel;

Pixel pixel(float r, float g, float b, float a, float depth);
void write(Pixel* pixel, Pixel writee);

typedef struct {
	Vec3 pos;
	float focalLength;
} Camera;

void render(unsigned char* image, size_t width, size_t height);
Vec3 project(Vec3 p, Camera c);