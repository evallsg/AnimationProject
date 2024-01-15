#pragma once
#include <vector>
#include "Transform.h"

//used to hold the transformation of every bone in an animated hierarchy
class Pose {
protected:
	std::vector<Transform> mJoints;
	std::vector<int> mParents; // parent joints Id

public:
	Pose();
	Pose(const Pose& p);
	Pose& operator=(const Pose& p);
	Pose(unsigned int numJoints);

	void Resize(unsigned int size);
	unsigned int Size();

	int GetParent(unsigned int index);
	void SetParent(unsigned int index, int parent);

	Transform GetLocalTransform(unsigned int index);
	void SetLocalTransform(unsigned int index, const Transform& transform);
	Transform GetGlobalTransform(unsigned int index);
	Transform operator[](unsigned int index);	
	
	//Takes a reference to a vector of matrices and fills it out with the global transform matrix of each joint in the pose 
	//to be passed to OpenGL, it needs to be converted into a linear array of matrices
	void GetMatrixPalette(std::vector<mat4>& out);

	bool operator==(const Pose& other);
	bool operator!=(const Pose& other);
};