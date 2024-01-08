#pragma once
#ifndef _H_VEC3_
#define _H_VEC3_
#include <math.h>
#include "math_includes.h"

struct vec3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};
	inline vec3() : x(0.0f), y(0.0f), z(0.0f) { }
	inline vec3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) { }
	inline vec3(float* fv) :
		x(fv[0]), y(fv[1]), z(fv[2]) { }
};
#endif

/*
Comparing floating point numbers is difficult. Instead of comparing two floating point
numbers directly, you need to compare them using an epsilon. An epsilon is an arbitrarily
small positive number that is the minimum difference two numbers need to have to be
considered different numbers.
*/

vec3 operator+(const vec3& l, const vec3& r);
vec3 operator-(const vec3& l, const vec3& r);
vec3 operator*(const vec3& v, float f);

vec3 operator*(const vec3& l, const vec3& r);

/*The dot product is used to measure how similar two vectors are.*/
float dot(const vec3& l, const vec3& r);

/*
Finding the length of a vector involves a square root operation, which should
be avoided when possible. When checking the length of a vector, the check can
be done in squared space to avoid the square root. Ex: (dot(A, A) < 5 * 5).
*/
float lenSq(const vec3& v);

float len(const vec3& v);

//It akes a reference to a vectorand normalizes it in place.
void normalize(vec3& v);

//Takes a constant reference and does not modify the input vector. It returns a new one.
vec3 normalized(const vec3& v);

//In radians
float angle(const vec3& l, const vec3& r);

vec3 project(const vec3& a, const vec3& b);
vec3 reject(const vec3& a, const vec3& b);

//Bounce reflection. For a mirror reflection, negate the result or the incident vector.
vec3 reflect(const vec3& a, const vec3& b);

//Cross product returns a third vector that is perpendicular to both input vectors.
vec3 cross(const vec3& l, const vec3& r);

//Linear interpolation. The amount to lerp by is a normalized value between 0 and 1; Interpolates on the shortest path from one vector to another.
vec3 lerp(const vec3& s, const vec3& e, float t);

//Spherical linear interpolation. Interpolates on the shortest arc.
vec3 slerp(const vec3& s, const vec3& e, float t);

// It's a very close approximation and much cheaper to calculate than slerp .The only time it makes sense to use slerp instead is if constant interpolation velocity is required.
vec3 nlerp(const vec3& s, const vec3& e, float t);

bool operator==(const vec3& l, const vec3& r);
bool operator!=(const vec3& l, const vec3& r);