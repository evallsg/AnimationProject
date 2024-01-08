#pragma once
#include <vector>
#include <string>
#include "TransformTrack.h"
#include "Pose.h"

class Clip {
protected:
	std::vector<TransformTrack> mTracks;
	std::string mName;
	float mStartTime;
	float mEndTime;
	bool mLooping;

protected:
	float AdjustTimeToFitRange(float inTime);

public:
	Clip();

	//gets joint Id based for a specific track index
	unsigned int GetIdAtIndex(unsigned int index);
	//sets joint ID based on the index of the joint in the clip
	void SetIdAtIndex(unsigned int idx, unsigned int id);
	unsigned int Size();

	//samples the animation clip at the provided time into the Pose reference
	float Sample(Pose& outPose, float inTime);
	//returns a transform track for the specified joint
	TransformTrack& operator[](unsigned int index);

	//sets the start/end time of the animation clip based on the tracks that make up the clip
	void RecalculateDuration();
	
	std::string& GetName();
	void SetName(const std::string& inNewName);
	float GetDuration();
	float GetStartTime();
	float GetEndTime();
	bool GetLooping();
	void SetLooping(bool inLooping);
};