#include "type.h"

VEC3_FLOAT_IMPL(float, sqrtf)
VEC3_IMPL(int)

Color color (float r, float g, float b, float a) {
	return (Color) {
		.r = r,
		.g = g,
		.b = b,
		.a = a
	};
}

Color colorLerp(Color p0, Color p1, float t) {
	return (Color) {
		.r = p0.r + t * (p1.r - p0.r),
		.g = p0.g + t * (p1.g - p0.g),
		.b = p0.b + t * (p1.b - p0.b),
		.a = p0.a + t * (p1.a - p0.a),
	};
}

Color colorDiv(Color p0, float p1) {
	return color(
		p0.r / p1,
		p0.g / p1,
		p0.b / p1,
		p0.a / p1
	);
}
