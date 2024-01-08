#pragma once
#include "Interpolation.h"
#include "Frame.h"
#include <vector>
#include "vec3.h"
#include "quat.h"

//Collection of frames
template<typename T, int N>
class Track {
protected:
	std::vector<Frame<N>> mFrames;
	Interpolation mInterpolation; //interpolation type
public:
	Track();
	void Resize(unsigned int size);
	unsigned int Size();
	Interpolation GetInterpolation();
	void SetInterpolation(Interpolation interp);
	float GetStartTime();
	float GetEndTime();
	//parameters: time value, if the track is looping or not
	T Sample(float time, bool looping);
	Frame<N>& operator[](unsigned int index);
protected:
	//helper functions, a sample for each type of interpolation
	T SampleConstant(float time, bool looping);
	T SampleLinear(float time, bool looping);
	T SampleCubic(float time, bool looping);
	//helper function to evaluate Hermite splines (tangents)
	T Hermite(float time, const T& p1, const T& s1,
		const T& p2, const T& s2);
	int FrameIndex(float time, bool looping);
	//takes an input time that is outside the range of the track and adjusts it to be a valid time on the track
	float AdjustTimeToFitTrack(float t, bool loop);

	//casts an array of floats (the data inside a frame) to the templated type of the track	
	T Cast(float* value); // Will be specialized
};

typedef Track<float, 1> ScalarTrack;
typedef Track<vec3, 3> VectorTrack;
typedef Track<quat, 4> QuaternionTrack;