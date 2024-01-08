#pragma once
#include "Track.h"
#include "Transform.h"
//binds a transform to a joint
class TransformTrack {
protected:
	unsigned int mId; //joint Id
	VectorTrack mPosition;
	QuaternionTrack mRotation;
	VectorTrack mScale;
public:
	TransformTrack();
	unsigned int GetId();
	void SetId(unsigned int id);
	VectorTrack& GetPositionTrack();
	QuaternionTrack& GetRotationTrack();
	VectorTrack& GetScaleTrack();
	float GetStartTime();
	float GetEndTime();
	bool IsValid();
	Transform Sample(const Transform& ref, float time,
		bool looping);
};