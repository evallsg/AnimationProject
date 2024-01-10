#ifndef _H_Lab1_
#define _H_Lab1_

#include "../vec3.h"
#include "../vec2.h"

#include "../Application.h"
#include "DebugDraw.h"

#include "../math_includes.h"

class Lab1 : public Application {
protected:
	DebugDraw* cube;
	vec3 initPos;
	vec3 currentPos;
	vec3 finalPos;
	float duration;
	float elapsedTime;
	float mRotation;
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

	void OnKeyUp(int keyCode) { }
	void OnKeyDown(int keyCode) { }
};

#endif