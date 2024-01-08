#include "Track.h"

template Track<float, 1>;
template Track<vec3, 3>;
template Track<quat, 4>;

//This class is templated, so some functions are declared but not necessary implemented depending of the interpolation type

//Track helpers avoid having to make specialized versions of the interpolation functions
namespace TrackHelpers {

	//linear interpolation for each type of track
	inline float Interpolate(float a, float b, float t) {
		return a + (b - a) * t;
	}
	inline vec3 Interpolate(const vec3& a, const vec3& b,	float t) {
		return lerp(a, b, t);
	}
	inline quat Interpolate(const quat& a, const quat& b,	float t) {
		quat result = mix(a, b, t);
		if (dot(a, b) < 0) { // Neighborhood
			result = mix(a, -b, t);
		}
		return normalized(result); //NLerp, not slerp
	}
	
	// When a Hermite spline is interpolated, if the input type was a quaternion, the result needs to be normalized.
	inline float AdjustHermiteResult(float f) {
		return f;
	}
	inline vec3 AdjustHermiteResult(const vec3& v) {
		return v;
	}
	inline quat AdjustHermiteResult(const quat& q) {
		return normalized(q);
	}

	//Make sure two quaternions are in the correct neighborhood
	inline void Neighborhood(const float& a, float& b) {}
	inline void Neighborhood(const vec3& a, vec3& b) {}
	inline void Neighborhood(const quat& a, quat& b) {
		if (dot(a, b) < 0) {
			b = -b;
		}
	}
}; // End Track Helpers namespace

template<typename T, int N>
Track<T, N>::Track() {
	mInterpolation = Interpolation::Linear;
}
template<typename T, int N>
float Track<T, N>::GetStartTime() {
	return mFrames[0].mTime;
}
template<typename T, int N>
float Track<T, N>::GetEndTime() {
	return mFrames[mFrames.size() - 1].mTime;
}

//call SampleConstant, SampleLinear, or SampleCubic, depending on the track type.
template<typename T, int N>
T Track<T, N>::Sample(float time, bool looping) {
	if (mInterpolation == Interpolation::Constant) {
		return SampleConstant(time, looping);
	}
	else if (mInterpolation == Interpolation::Linear) {
		return SampleLinear(time, looping);
	}

	return SampleCubic(time, looping);
}

template<typename T, int N>
Frame<N>& Track<T, N>::operator[](unsigned int index) {
	return mFrames[index];
}

//size of the frames vector
template<typename T, int N>
void Track<T, N>::Resize(unsigned int size) {
	mFrames.resize(size);
}
template<typename T, int N>
unsigned int Track<T, N>::Size() {
	return mFrames.size();
}

template<typename T, int N>
Interpolation Track<T, N>::GetInterpolation() {
	return mInterpolation;
}
template<typename T, int N>
void Track<T, N>::SetInterpolation(Interpolation
	interpolation) {
	mInterpolation = interpolation;
}

template<typename T, int N>
T Track<T, N>::Hermite(float t, const T& p1, const T& s1,
	const T& _p2, const T& s2) {
	float tt = t * t;

	float ttt = tt * t;
	T p2 = _p2;
	TrackHelpers::Neighborhood(p1, p2);
	float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
	float h2 = -2.0f * ttt + 3.0f * tt;
	float h3 = ttt - 2.0f * tt + t;
	float h4 = ttt - tt;
	T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
	return TrackHelpers::AdjustHermiteResult(result);
}

//return the frame immediately before that time (on the left)
template<typename T, int N>
int Track<T, N>::FrameIndex(float time, bool looping) {
	unsigned int size = (unsigned int)mFrames.size();
	if (size <= 1) {
		return -1;
	}
	//If the track is sampled as looping, the input time needs to be adjusted so that it falls between the start and end frames.
	if (looping) {
		float startTime = mFrames[0].mTime;
		float endTime = mFrames[size - 1].mTime;
		float duration = endTime - startTime;
		time = fmodf(time - startTime,
			endTime - startTime);
		//looping, time needs to be adjusted so that it is within a valid range.
		if (time < 0.0f) {
			time += endTime - startTime;
		}
		time = time + startTime;
	}
	else {
		//clamp the time in the track frames range
		if (time <= mFrames[0].mTime) {
			return 0;
		}
		if (time >= mFrames[size - 2].mTime) {
			//The Sample function always needs a current and next frame (for interpolation), so the index of the second-to-last frame is used.
			return (int)size - 2;
		}
	}
	for (int i = (int)size - 1; i >= 0; --i) {
		if (time >= mFrames[i].mTime) {
			return i;
		}
	}
	// Invalid code, we should not reach here!
	return -1;
} // End of FrameIndex


//Adjusts the time to be in the range of the start/end frames of the track.
template<typename T, int N>
float Track<T, N>::AdjustTimeToFitTrack(float time, bool looping) {
	unsigned int size = (unsigned int)mFrames.size();
	if (size <= 1) {
		return 0.0f;
	}
	float startTime = mFrames[0].mTime;
	float endTime = mFrames[size - 1].mTime;
	float duration = endTime - startTime;
	if (duration <= 0.0f) {
		return 0.0f;
	}
	if (looping) {
		time = fmodf(time - startTime,
			endTime - startTime);
		if (time < 0.0f) {
			time += endTime - startTime;
		}
		time = time + startTime;
	}

	else {
		if (time <= mFrames[0].mTime) {
			time = startTime;
		}
		if (time >= mFrames[size - 1].mTime) {
			time = endTime;
		}
	}
	return time;
}	

//They cast a float array stored in a Frame class into the data type that the Frame class represents
template<> float Track<float, 1>::Cast(float* value) {
	return value[0];
}
template<> vec3 Track<vec3, 3>::Cast(float* value) {
	return vec3(value[0], value[1], value[2]);
}
template<> quat Track<quat, 4>::Cast(float* value) {
	quat r = quat(value[0], value[1], value[2], value[3]);
	return normalized(r);
}

//constant sampling:
//often used for things such as visibility flags, where it makes sense for the value of a variable to change from one frame to the next without any real interpolation
template<typename T, int N>
T Track<T, N>::SampleConstant(float t, bool loop) {
	int frame = FrameIndex(t, loop);
	if (frame < 0 || frame >= (int)mFrames.size()) {
		return T();
	}
	return Cast(&mFrames[frame].mValue[0]);
}

//linear sampling:
//applications provide an option to approximate animation curves by sampling them at set intervals
template<typename T, int N>
T Track<T, N>::SampleLinear(float time, bool looping) {
	int thisFrame = FrameIndex(time, looping);
	if (thisFrame < 0 || thisFrame >= mFrames.size() - 1) {
		return T();
	}
	int nextFrame = thisFrame + 1;
	//make sure the time is valid
	float trackTime = AdjustTimeToFitTrack(time, looping);
	float thisTime = mFrames[thisFrame].mTime;
	float frameDelta = mFrames[nextFrame].mTime - thisTime;
	if (frameDelta <= 0.0f) {
		return T();
	}
	float t = (trackTime - thisTime) / frameDelta;
	T start = Cast(&mFrames[thisFrame].mValue[0]);
	T end = Cast(&mFrames[nextFrame].mValue[0]);
	return TrackHelpers::Interpolate(start, end, t);
}

//cubic sampling:
template<typename T, int N>
T Track<T, N>::SampleCubic(float time, bool looping) {
	int thisFrame = FrameIndex(time, looping);
	if (thisFrame < 0 || thisFrame >= mFrames.size() - 1) {
		return T();
	}
	int nextFrame = thisFrame + 1;

	float trackTime = AdjustTimeToFitTrack(time, looping);
	float thisTime = mFrames[thisFrame].mTime;
	float frameDelta = mFrames[nextFrame].mTime - thisTime;
	if (frameDelta <= 0.0f) {
		return T();
	}

	//Cast function normalizes quaternions, which is bad because slopes are not meant to be quaternions.
	//Using memcpy instead of Cast copies the values directly, avoiding normalization.
	float t = (trackTime - thisTime) / frameDelta;
	size_t fltSize = sizeof(float);
	T point1 = Cast(&mFrames[thisFrame].mValue[0]);
	T slope1;// = mFrames[thisFrame].mOut * frameDelta;
	memcpy(&slope1, mFrames[thisFrame].mOut, N * fltSize);
	slope1 = slope1 * frameDelta;
	T point2 = Cast(&mFrames[nextFrame].mValue[0]);
	T slope2;// = mFrames[nextFrame].mIn[0] * frameDelta;
	memcpy(&slope2, mFrames[nextFrame].mIn, N * fltSize);
	slope2 = slope2 * frameDelta;

	return Hermite(t, point1, slope1, point2, slope2);
}