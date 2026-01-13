#pragma once

#include "math.h"

typedef struct Vec3 {
	float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z);
Vec3 vec3Lerp(Vec3 p0, Vec3 p1, float t);
Vec3 normalise(Vec3 p);

typedef struct Color {
	float r, g, b, a;
} Color;

Color color(float r, float g, float b, float a);
Color colorLerp(Color p0, Color p1, float t);
Color colorDiv(Color p0, float p1);