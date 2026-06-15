#pragma once

#include "math.h"

#define VEC3(T) \
typedef struct Vec3_ ## T {\
	T x, y, z;\
} Vec3_ ## T;\
Vec3_ ## T vec3_ ## T(T x, T y, T z);\
Vec3_ ## T vec3Add_ ## T(Vec3_ ## T p0, Vec3_ ## T p1);\
Vec3_ ## T vec3Sub_ ## T(Vec3_ ## T p0, Vec3_ ## T p1);\

#define VEC3_FLOAT(T) \
VEC3(T)\
Vec3_ ## T vec3Lerp_ ## T(Vec3_ ## T p0, Vec3_ ## T p1, T t);\
Vec3_ ## T vec3Normalise_ ## T(Vec3_ ## T p);\

#define VEC3_IMPL(T) \
Vec3_ ## T vec3_ ## T(T x, T y, T z) {\
	return (Vec3_ ## T){x,y,z};\
}\
Vec3_ ## T vec3Add_ ## T(Vec3_ ## T p0, Vec3_ ## T p1) {\
	return vec3_ ## T(\
		p0.x + p1.x,\
		p0.y + p1.y,\
		p0.z + p1.z\
	);\
}\
Vec3_ ## T vec3Sub_ ## T(Vec3_ ## T p0, Vec3_ ## T p1) {\
	return vec3_ ## T(\
		p0.x - p1.x,\
		p0.y - p1.y,\
		p0.z - p1.z\
	);\
}\

#define VEC3_FLOAT_IMPL(T, sqrtFunc) \
VEC3_IMPL(T)\
Vec3_ ## T vec3Lerp_ ## T(Vec3_ ## T p0, Vec3_ ## T p1, T t) {\
	return vec3_ ## T(\
		p0.x + t*(p1.x - p0.x),\
		p0.y + t*(p1.y - p0.y),\
		p0.z + t*(p1.z - p0.z)\
	);\
}\
Vec3_ ## T vec3Div_ ## T(Vec3_ ## T p0, T p1) {\
	return vec3_ ## T(\
		p0.x / p1,\
		p0.y / p1,\
		p0.z / p1\
	);\
}\
Vec3_ ## T vec3Normalise_ ## T(Vec3_ ## T p) {\
	T length = sqrtFunc(p.x*p.x + p.y*p.y + p.z*p.z);\
	return vec3Div_ ## T(p, length);\
}\

VEC3_FLOAT(float)
VEC3(int)

typedef struct Color {
	float r, g, b, a;
} Color;

Color color(float r, float g, float b, float a);
Color colorLerp(Color p0, Color p1, float t);
Color colorDiv(Color p0, float p1);
