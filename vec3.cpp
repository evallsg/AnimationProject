#include "vec3.h"

vec3 operator+(const vec3& l, const vec3& r) {
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator-(const vec3& l, const vec3& r) {
	return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator*(const vec3& v, float f) {
	return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(const vec3& l, const vec3& r) {
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

/*The dot product is used to measure how similar two vectors are.*/
float dot(const vec3& l, const vec3& r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

/*
Finding the length of a vector involves a square root operation, which should
be avoided when possible. When checking the length of a vector, the check can
be done in squared space to avoid the square root. Ex: (dot(A, A) < 5 * 5).
*/
float lenSq(const vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float len(const vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < EPSILON) {
		return 0.0f;
	}
	return sqrtf(lenSq);
}

void normalize(vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < EPSILON) { return; }
	float invLen = 1.0f / sqrtf(lenSq);
	v.x *= invLen;
	v.y *= invLen;
	v.z *= invLen;
}

vec3 normalized(const vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < EPSILON) { return v; }
	float invLen = 1.0f / sqrtf(lenSq);
	return vec3(
		v.x * invLen,
		v.y * invLen,
		v.z * invLen
	);
}

//Angle in radians
float angle(const vec3& l, const vec3& r) {
	float sqMagL = l.x * l.x + l.y * l.y + l.z * l.z;
	float sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;
	if (sqMagL < EPSILON || sqMagR < EPSILON) {
		return 0.0f;
	}
	float dot = l.x * r.x + l.y * r.y + l.z * r.z;
	float len = sqrtf(sqMagL) * sqrtf(sqMagR);
	return acosf(dot / len);
}

vec3 project(const vec3& a, const vec3& b) {
	float magBSq = len(b);
	if (magBSq < EPSILON) {
		return vec3();
	}
	float scale = dot(a, b) / magBSq;
	return b * scale;
}

/*
To find the rejection of A onto B, subtract the
projection of A onto B from vector A
*/

vec3 reject(const vec3& a, const vec3& b) {
	vec3 projection = project(a, b);
	return a - projection;
}

vec3 reflect(const vec3& a, const vec3& b) {
	float magBSq = len(b);
	if (magBSq < EPSILON) {
		return vec3();
	}
	float scale = dot(a, b) / magBSq;
	vec3 proj2 = b * (scale * 2);
	return a - proj2;
}

vec3 cross(const vec3& l, const vec3& r) {
	return vec3(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}

/*
Two vectors can be interpolated linearly by scaling the difference between the two
vectors and adding the result back to the original vector.
*/
vec3 lerp(const vec3& s, const vec3& e, float t) {
	return vec3(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
}

/*When the value of t is close to 0, as slerp will yield unexpected results.
When the value of t is close to 0, fall back on lerp or normalized lerp (nlerp)
*/
vec3 slerp(const vec3& s, const vec3& e, float t) {
	if (t < 0.01f) {
		return lerp(s, e, t);
	}
	vec3 from = normalized(s);
	vec3 to = normalized(e);
	float theta = angle(from, to);
	float sin_theta = sinf(theta);
	float a = sinf((1.0f - t) * theta) / sin_theta;
	float b = sinf(t * theta) / sin_theta;
	return from * a + to * b;
}

/*
nlerp approximates slerp. Unlike slerp, nlerp is not constant in velocity. 
nlerp is much faster than slerp and easier to implement; just normalize the result of lerp.
*/
vec3 nlerp(const vec3& s, const vec3& e, float t) {
	vec3 linear(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
	return normalized(linear);
}
/*
Compare each element using an epsilon. Another way is to subtract the two vectors. If they were equal, subtracting them would yield a vector with no length.
*/
bool operator==(const vec3& l, const vec3& r) {
	vec3 diff(l - r);
	return lenSq(diff) < EPSILON;
}
bool operator!=(const vec3& l, const vec3& r) {
	return !(l == r);
}