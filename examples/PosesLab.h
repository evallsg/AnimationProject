#pragma once
#ifndef _H_PosesLab_
#define _H_PosesLab_
#include "../Application.h"
#include "DebugDraw.h"
#include <vector>
#include "../Pose.h"
#include "../Clip.h"

class PosesLab : public Application {
protected:
	Pose mRestPose;
	Pose mCurrentPose;
	std::vector<Clip> mClips;
	unsigned int mCurrentClip;
	float mPlaybackTime;
	DebugDraw* mUpAxis;
	DebugDraw* mRightAxis;
	DebugDraw* mForwardAxis;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mCurrentPoseVisual;
	int mDebug;
public:

	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
	void ImGui(nk_context* inContext);
	void OnKeyDown(int keyCode);
};

#endif
