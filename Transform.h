#pragma once

#include "math_includes.h"
#include "vec3.h"
#include "quat.h"

//Joints can be represented as transforms
struct Transform {
	vec3 position;
	quat rotation;
	vec3 scale;

Transform(const vec3& p, const quat& r, const vec3& s) :
	position(p), rotation(r), scale(s) {}
Transform() :
	position(vec3(0, 0, 0)),
	rotation(quat(0, 0, 0, 1)),
	scale(vec3(1, 1, 1))
{}
}; // End of transform struct

/*
Transforms can be combined in the same way as matrices and quaternions and the
effects of two transforms can be combined into one transform. To keep things consistent,
combining transforms should maintain a right-to-left combination order.
*/
Transform combine(const Transform& a, const Transform& b);

/*
A transform maps from one space into another space. It's possible
to reverse that mapping and map the transform back into the original space. As with
matrices and quaternions, transforms can also be inverted.
*/
Transform inverse(const Transform& t);

/*
* To linear blend/mix/interpolate transforms
*/
Transform mix(const Transform& a, const Transform& b, float t);

/* Convert transforms into matrices. Graphics APIs work with matrices not transforms.*/
mat4 transformToMat4(const Transform& t);

Transform mat4ToTransform(const mat4& m);

/*
Modify points (for vertex transfromations) and vectors.

 TransfromPoint does the same thing that multiplying a matrixand
a point does, just one step at a time.
*/
vec3 transformPoint(const Transform& a, const vec3& b);
vec3 transformVector(const Transform& a, const vec3& b);