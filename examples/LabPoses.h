#pragma once
#ifndef _H_LabPoses_
#define _H_LabPoses_
#include "../Application.h"
#include "DebugDraw.h"
#include <vector>
#include "../Skeleton.h"
#include "../Clip.h"

class LabPoses : public Application {
protected:
	int mousePositionX;
	int mousePositionY;
	int lastMousePositionX;
	int lastMousePositionY;

	int dragging;

	vec3 cameraPos;
	vec3 cameraTarget;
	float fov;

	int mDebug;
	int mShowRestPose;
	int mShowBindPose;

	Pose mRestPose;
	Pose mBindPose;
	Pose mCurrentPose;
	std::vector<Clip> mClips;
	unsigned int mCurrentClip;
	float mPlaybackTime;
	DebugDraw* mUpAxis;
	DebugDraw* mRightAxis;
	DebugDraw* mForwardAxis;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mBindPoseVisual;
	DebugDraw* mCurrentPoseVisual;

	void Orbit(float angle, vec3 axis);
public:

	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
	void ImGui(nk_context* inContext);
	void OnKeyDown(int keyCode);

	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();
	void OnWheel(int keyCode);

	void SetMousePosition(int x, int y);

};

#endif
