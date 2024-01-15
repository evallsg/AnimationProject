#pragma once
#ifndef _H_LabMesh_
#define _H_LabMesh_
#include "../Application.h"
#include "DebugDraw.h"
#include "../Mesh.h"
#include "../Shader.h"

class LabMesh: public Application {
protected:
	int dragging;
	vec2 lastMousePosition;

	vec3 cameraPos;
	vec3 cameraTarget;
	float fov;

	int mDebug;
	int mShowRestPose;
	int mShowBindPose;

	Skeleton skeleton;
	Pose mRestPose;
	Pose mBindPose;
	Pose mCurrentPose;

	DebugDraw* mUpAxis;
	DebugDraw* mRightAxis;
	DebugDraw* mForwardAxis;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mBindPoseVisual;
	DebugDraw* mCurrentPoseVisual;
	
	std::vector<Mesh> mMeshes;

	Shader *mStaticShader;
	Texture *mDiffuseTexture;

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