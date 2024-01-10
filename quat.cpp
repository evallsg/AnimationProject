#include "quat.h"
#include <cmath>

quat angleAxis(float angle, const vec3& axis) {
	vec3 norm = normalized(axis);
	float s = sinf(angle * 0.5f);
	return quat(norm.x * s,
		norm.y * s,
		norm.z * s,
		cosf(angle * 0.5f)
	);
}

/*
* Shortest arc between two points lies on a plane that contains both points and the center fo the sphere.
* The plane is perpendicular to the axis of rotation between those vectors.
* To get the axis of rotation --> normalize them and compute the cross product.
* To get the angle --> compute dot product.
* Finding the angle between the two vectors would be expensive, but the half-angle can be counted without knowing what the angle is.
* Construct a quaternion using v1 and this halfway vector.
* 
* Quaternions double the angle --> compute the angle and divide it by 2 or compute the rotation with the half vector,
* 
* q.x = axis.x
* q.y = axis.y
* q.z = axis.z
* q.w = cos(angle/2) (?)
*/
quat fromTo(const vec3& from, const vec3& to) {
	vec3 f = normalized(from);
	vec3 t = normalized(to);

	//parallel vectors 
	if (f == t) {
		return quat();
	}
	// check whether the two vectors are opposites of each other
	else if (f == t * -1.0f) {
		vec3 ortho = vec3(1, 0, 0);
		if (fabsf(f.y) < fabsf(f.x)) {
			ortho = vec3(0, 1, 0);
		}
		if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
			ortho = vec3(0, 0, 1);
		}
		vec3 axis = normalized(cross(f, ortho));
		return quat(axis.x, axis.y, axis.z, 0);
	}

	//half vector between the from and to vectors.
	vec3 half = normalized(f + t);
	vec3 axis = cross(f, half);
	return quat(axis.x, axis.y, axis.z, dot(f, half));
}

vec3 getAxis(const quat& quat) {
	return normalized(vec3(quat.x, quat.y, quat.z));
}

// The angle of rotation is double the inverse cosine of the real component.
float getAngle(const quat& quat) {
	return 2.0f * acosf(quat.w);
}

quat operator+(const quat& a, const quat& b) {
	return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
quat operator-(const quat& a, const quat& b) {
	return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
quat operator*(const quat& a, float b) {
	return quat(a.x * b, a.y * b, a.z * b, a.w * b);
}
quat operator-(const quat& q) {
	return quat(-q.x, -q.y, -q.z, -q.w);
}

bool operator==(const quat& left, const quat& right) {
	return (fabsf(left.x - right.x) <= EPSILON &&
		fabsf(left.y - right.y) <= EPSILON &&
		fabsf(left.z - right.z) <= EPSILON &&
		fabsf(left.w - right.w) <= EPSILON);
}
bool operator!=(const quat& a, const quat& b) {
	return !(a == b);
}

bool sameOrientation(const quat& l, const quat& r) {
	return (fabsf(l.x - r.x) <= EPSILON &&
		fabsf(l.y - r.y) <= EPSILON &&
		fabsf(l.z - r.z) <= EPSILON &&
		fabsf(l.w - r.w) <= EPSILON) ||
		(fabsf(l.x + r.x) <= EPSILON &&
			fabsf(l.y + r.y) <= EPSILON &&
			fabsf(l.z + r.z) <= EPSILON &&
			fabsf(l.w + r.w) <= EPSILON);
}

float dot(const quat& a, const quat& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//the length of a quaternion is the dot product of the quaternion with itself.
float lenSq(const quat& q) {
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float len(const quat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < EPSILON) {
		return 0.0f;
	}
	return sqrtf(lenSq);
}

void normalize(quat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < EPSILON) {
		return;
	}
	float i_len = 1.0f / sqrtf(lenSq);
	q.x *= i_len;
	q.y *= i_len;
	q.z *= i_len;
	q.w *= i_len;
}

quat normalized(const quat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < EPSILON) {
		return quat();
	}
	float il = 1.0f / sqrtf(lenSq); // il: inverse length
	return quat(q.x * il, q.y * il, q.z * il, q.w * il);
}

quat conjugate(const quat& q) {
	return quat(
		-q.x,
		-q.y,
		-q.z,
		q.w
	);
}

quat inverse(const quat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < EPSILON) {
		return quat();
	}
	float recip = 1.0f / lenSq;
	return quat(-q.x * recip,
		-q.y * recip,
		-q.z * recip,
		q.w * recip
	);
}

//Right to left multiplication (as matrices)
quat operator*(const quat& q1, const quat& q2) {
	return quat(
		q2.x * q1.w + q2.y * q1.z - q2.z * q1.y + q2.w * q1.x,
		-q2.x * q1.z + q2.y * q1.w + q2.z * q1.x + q2.w * q1.y,
		q2.x * q1.y - q2.y * q1.x + q2.z * q1.w + q2.w * q1.z,
		-q2.x * q1.x - q2.y * q1.y - q2.z * q1.z + q2.w * q1.w
	);
}

//Alternative computation of quaternions multiplication
//quat operator*(const quat& q1, const quat& q2) {
//	quat result;
//	result.scalar = q2.scalar * q1.scalar - dot(q2.vector, q1.vector);
//	result.vector = q1.vector * q2.scalar + q2.vector * q1.scalar + cross(q1.vector, q2.vector);
//	return result;
//}

vec3 operator*(const quat& q, const vec3& v) {
	return q.vector * 2.0f * dot(q.vector, v) +
		v * (q.scalar * q.scalar - dot(q.vector, q.vector)) +
		cross(q.vector, v) * 2.0f * q.scalar;
}

//Neighborhooding --> to take the shortest arc
quat SampleFunction(const quat& a, quat& b) {
	if (dot(a, b) < 0.0f) {
		b = -b;
	}
	return slerp(a, b, 0.5f);
}

/*Interpolation*/

//lerp
quat mix(const quat& from, const quat& to, float t) {
	return from * (1.0f - t) + to * t;
}

//nlerp
quat nlerp(const quat& from, const quat& to, float t) {
	return normalized(from + (to - from) * t);
}

/*
* 
To raise a quaternion to some power, it needs to be decomposed into an angle and an axis.
Then, the angle can be adjusted by the power and a new quaternion can be built from the adjusted angle and axis.
* 
*/
quat operator^(const quat& q, float f) {
	float angle = 2.0f * acosf(q.scalar);
	vec3 axis = normalized(q.vector);
	float halfCos = cosf(f * angle * 0.5f);
	float halfSin = sinf(f * angle * 0.5f);
	return quat(axis.x * halfSin,
		axis.y * halfSin,
		axis.z * halfSin,
		halfCos
	);
}

//slerp
/*
To interpolate between two quaternions, find the delta quaternion from the start rotation
to the end rotation. This delta quaternion is the interpolation path. Raise the angle to
the power of how much the two quaternions are being interpolated between (usually
represented as t) and multiply the start quaternion back.

The input vectors to slerp should be normalized, which means you could use
conjugate instead of inverse in the slerp function. Most of the time, nlerp will
be used over slerp.
*/
quat slerp(const quat& start, const quat& end, float t) {
	if (fabsf(dot(start, end)) > 1.0f - EPSILON) {
		return nlerp(start, end, t);
	}
	quat delta = inverse(start) * end;
	return normalized((delta ^ t) * start);
}

/*
matrix lookAt function creates a view matrix, which is the inverse of the camera
transform. The rotation of lookAt and the result of lookRotation are
going to be the inverse of each other.
*/
quat lookRotation(const vec3& direction, const vec3& up) {
	// Find orthonormal basis vectors
	vec3 f = normalized(direction); // Object Forward
	vec3 u = normalized(up); // Desired Up
	vec3 r = cross(u, f); // Object Right
	u = cross(f, r); // Object Up
	// From world forward to object forward
	quat worldToObject = fromTo(vec3(0, 0, 1), f);
	// what direction is the new object up?
	vec3 objectUp = worldToObject * vec3(0, 1, 0);
	// From object up to desired up
	quat u2u = fromTo(objectUp, u);
	// Rotate to forward direction first
	// then twist to correct up
	quat result = worldToObject * u2u;
	// Don't forget to normalize the result
	return normalized(result);
}

mat4 quatToMat4(const quat& q) {
	vec3 r = q * vec3(1, 0, 0);
	vec3 u = q * vec3(0, 1, 0);
	vec3 f = q * vec3(0, 0, 1);
	return mat4(r.x, r.y, r.z, 0,
		u.x, u.y, u.z, 0,
		f.x, f.y, f.z, 0,
		0, 0, 0, 1
	);
}

quat mat4ToQuat(const mat4& m) {
	vec3 up = normalized(vec3(m.up.x, m.up.y, m.up.z));
	vec3 forward = normalized(vec3(m.forward.x, m.forward.y, m.forward.z));
	vec3 right = cross(up, forward);
	up = cross(forward, right);
	return lookRotation(forward, up);
}