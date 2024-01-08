#pragma once
#ifndef _H_QUAT_
#define _H_QUAT_
#include "vec3.h"
#include "mat4.h"

struct quat {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			vec3 vector;
			float scalar;
		};
		float v[4];
	};
	inline quat() :
		x(0), y(0), z(0), w(1) { }
	inline quat(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {}
};
#endif

quat angleAxis(float angle, const vec3& axis);

//Most intuitive way of creating a quaternion.
quat fromTo(const vec3& from, const vec3& to);

vec3 getAxis(const quat& quat);
float getAngle(const quat& quat);

quat operator+(const quat& a, const quat& b);
quat operator-(const quat& a, const quat& b);
quat operator*(const quat& a, float b);
quat operator-(const quat& q);

bool operator==(const quat& left, const quat& right);
bool operator!=(const quat& a, const quat& b);
bool sameOrientation(const quat& l, const quat& r);

//dot product measures how similar two quaternions are.
float dot(const quat& a, const quat& b);

float lenSq(const quat& q);
float len(const quat& q);

void normalize(quat& q);
quat normalized(const quat& q);

//The inverse of a normalized quaternion is its conjugate.
quat conjugate(const quat& q);

//The proper inverse of a quaternion is the conjugate divided by the squared length of the quaternion.
quat inverse(const quat& q);

quat operator*(const quat& Q1, const quat& q2);
vec3 operator*(const quat& q, const vec3& v);

// Interpolation
quat SampleFunction(const quat& a, quat& b);
quat mix(const quat & from, const quat & to, float t);
quat nlerp(const quat & from, const quat & to, float t);
quat operator^(const quat & q, float f);

// The input vectors to slerp should be normalized
quat slerp(const quat & start, const quat & end, float t);

quat lookRotation(const vec3 & direction, const vec3 & up);

mat4 quatToMat4(const quat & q);
quat mat4ToQuat(const mat4 & m);