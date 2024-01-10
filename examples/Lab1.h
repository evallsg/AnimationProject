#ifndef _H_Lab1_
#define _H_Lab1_

#include "../vec3.h"
#include "../vec2.h"

#include "../Application.h"
#include "DebugDraw.h"

#include "../math_includes.h"

#include "../Mesh.h"
class Lab1 : public Application {
protected:
	DebugDraw* cubeDraw;
	Mesh cube;
	vec3 initPos;
	vec3 currentPos;
	vec3 finalPos;
	float duration;
	float elapsedTime;
	float mRotation;

	int interpolation;
	enum iTypes {LERP, SLERP};
	int dragging;
	vec2 lastMousePosition;

public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
	void ImGui(nk_context* context);

	void OnLeftMouseButtonDown();
	void OnRightMouseButtonDown();
	void OnWheelMouseButtonDown();

	void OnLeftMouseButtonUp();
	void OnRightMouseButtonUp();
	void OnWheelMouseButtonUp();

	void OnWheel(int amount);

	void SetMousePosition(int x, int y);

	void OnKeyUp(int keyCode) { }
	void OnKeyDown(int keyCode) { }

	vec3 Hermite(float t, const vec3 p1, const vec3 s1, const vec3 _p2, const vec3 s2);
};

#endif