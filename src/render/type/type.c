#include "type.h"

Vec3 vec3(float x, float y, float z) {
	Vec3 vec = {0};
	vec.x = x;
	vec.y = y;
	vec.z = z;

	return vec;
}

Vec3 vec3Lerp(Vec3 p0, Vec3 p1, float t) {
	return vec3(
		p0.x + t*(p1.x - p0.x),
		p0.y + t*(p1.y - p0.y),
		p0.z + t*(p1.z - p0.z)
	);
}

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