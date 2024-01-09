#pragma once
#include <vector>
#include "Pose.h"

//Keeps track of the bind pose, inverse bind pose, and joint names of an animated model
class Skeleton
{
protected:
	Pose mRestPose;
	Pose mBindPose;
	std::vector<mat4> mInvBindPose;
	std::vector<std::string> mJointNames;

protected:
	//updates the inverse bind pose matrix any time that the bind pose is set
	void UpdateInverseBindPose();

public:
	Skeleton();
	Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	void Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	Pose& GetBindPose();
	Pose& GetRestPose();
	std::vector<mat4>& GetInvBindPose();
	std::vector<std::string>& GetJointNames();
	std::string& GetJointName(unsigned int index);
}; // End Skeleton class


