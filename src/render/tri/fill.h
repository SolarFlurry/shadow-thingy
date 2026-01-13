#pragma once

#include <math.h>

#include "block.h"
#include "../type/type.h"
#include "../render.h"

typedef struct Pixel Pixel;

typedef struct {
	Vec3 pos;
	Color color;
} FillVertex;

void trifill(Pixel* buffer, FillVertex p0, FillVertex p1, FillVertex p2, int width, int height);