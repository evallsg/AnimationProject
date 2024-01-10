#include "Lab1.h"
#include "../mat4.h"
#include "../quat.h"

void Lab1::Initialize() {
	
	cube = new DebugDraw();
	cube->Push(vec3(-1.0f, -1.0f, -1.0f)); // triangle 1 : begin
	cube->Push(vec3(-1.0f, -1.0f, 1.0f));
	cube->Push(vec3(-1.0f, 1.0f, 1.0f)); // triangle 1 : end
	cube->Push(vec3(1.0f, 1.0f, -1.0f)); // triangle 2 : begin
	cube->Push(vec3(-1.0f, -1.0f, -1.0f));
	cube->Push(vec3(-1.0f, 1.0f, -1.0f)); // triangle 2 : end
	cube->Push(vec3(1.0f, -1.0f, 1.0f));
	cube->Push(vec3(-1.0f, -1.0f, -1.0f));
	cube->Push(vec3(1.0f, -1.0f, -1.0f));
	cube->Push(vec3(1.0f, 1.0f, -1.0f));
	cube->Push(vec3(1.0f, -1.0f, -1.0f));
	cube->Push(vec3(-1.0f, -1.0f, -1.0f));
	cube->Push(vec3(-1.0f, -1.0f, -1.0f));
	cube->Push(vec3(-1.0f, 1.0f, 1.0f));
	cube->Push(vec3(-1.0f, 1.0f, -1.0f));
	cube->Push(vec3(1.0f, -1.0f, 1.0f));
	cube->Push(vec3(-1.0f, -1.0f, 1.0f));
	cube->Push(vec3(-1.0f, -1.0f, -1.0f));
	cube->Push(vec3(-1.0f, 1.0f, 1.0f));
	cube->Push(vec3(-1.0f, -1.0f, 1.0f));
	cube->Push(vec3(1.0f, -1.0f, 1.0f));
	cube->Push(vec3(1.0f, 1.0f, 1.0f));
	cube->Push(vec3(1.0f, -1.0f, -1.0f));
	cube->Push(vec3(1.0f, 1.0f, -1.0f));
	cube->Push(vec3(1.0f, -1.0f, -1.0f));
	cube->Push(vec3(1.0f, 1.0f, 1.0f));
	cube->Push(vec3(1.0f, -1.0f, 1.0f));
	cube->Push(vec3(1.0f, 1.0f, 1.0f));
	cube->Push(vec3(1.0f, 1.0f, -1.0f));
	cube->Push(vec3(-1.0f, 1.0f, -1.0f));
	cube->Push(vec3(1.0f, 1.0f, 1.0f));
	cube->Push(vec3(-1.0f, 1.0f, -1.0f));
	cube->Push(vec3(-1.0f, 1.0f, 1.0f));
	cube->Push(vec3(1.0f, 1.0f, 1.0f));
	cube->Push(vec3(-1.0f, 1.0f, 1.0f));
	cube->Push(vec3(1.0f, -1.0f, 1.0f));
	
	cube->UpdateOpenGLBuffers();

	initPos = vec3(0, 0, 0);
	currentPos = initPos;
	finalPos = vec3(3, 0, 0);
	duration = 3;
}

void Lab1::Update(float inDeltaTime) {
	/*
	mRotation += inDeltaTime * 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}
	*/
	if (elapsedTime < duration) {
		currentPos = lerp(initPos, finalPos, elapsedTime / duration);
		elapsedTime += inDeltaTime;
	}
}

void Lab1::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	//mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(0, 0, 1)));
	mat4 model;
	model.position = vec4(currentPos.x, currentPos.y, currentPos.y, 1);
	cube->Draw(DebugDrawMode::Triangles, vec3(1, 1, 0), projection*view*model);
}

void Lab1::ImGui(nk_context* context) {
	nk_begin(context, "Controls", nk_rect(5.0f, 150.0f, 190.0f, 250.0f), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR);
	nk_layout_row_static(context, 25, 168, 1);
	nk_property_float(context, "Duration", 0, &duration, 100.f, 0.1, 1);
	nk_label(context, "Start position", NULL);
	nk_layout_row_static(context, 25, 60, 3);
	nk_property_float(context, "x", -100, &initPos.x, 100, 0.1, 1);
	nk_property_float(context, "y", -100, &initPos.y, 100, 0.1, 1);
	nk_property_float(context, "z", -100, &initPos.z, 100, 0.1, 1);
	
	nk_label(context, "End position", NULL);
	nk_layout_row_static(context, 25, 60, 3);
	nk_property_float(context, "x", -100, &finalPos.x, 100, 0.1, 1);
	nk_property_float(context, "y", -100, &finalPos.y, 100, 0.1, 1);
	nk_property_float(context, "z", -100, &finalPos.z, 100, 0.1, 1);

	if (nk_button_label(context, "Reset")) {
		elapsedTime = 0;
	}
	nk_end(context);
	
}

void Lab1::Shutdown() {
	
}

void Lab1::OnLeftMouseButtonDown() {
	/*mRotation +=  45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}*/
}

void Lab1::OnRightMouseButtonDown() {
	/*mRotation -= 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}*/
}
void Lab1::OnWheelMouseButtonDown() {}

void Lab1::OnLeftMouseButtonUp() {}
void Lab1::OnRightMouseButtonUp() {}
void Lab1::OnWheelMouseButtonUp() {}