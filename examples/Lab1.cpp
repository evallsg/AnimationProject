#include "Lab1.h"
#include "../mat4.h"
#include "../quat.h"

void Lab1::Initialize() {
	
	cubeDraw = new DebugDraw();
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f)); // triangle 1 : begin
	cubeDraw->Push(vec3(-1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, 1.0f)); // triangle 1 : end
	cubeDraw->Push(vec3(1.0f, 1.0f, -1.0f)); // triangle 2 : begin
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, -1.0f)); // triangle 2 : end
	cubeDraw->Push(vec3(1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, -1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(-1.0f, 1.0f, 1.0f));
	cubeDraw->Push(vec3(1.0f, -1.0f, 1.0f));
	
	cubeDraw->UpdateOpenGLBuffers();

	//std::vector<vec3> vertices;

	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f)); // triangle 1 : begin
	//vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, 1.0f)); // triangle 1 : end
	//vertices.push_back(vec3(1.0f, 1.0f, -1.0f)); // triangle 2 : begin
	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, -1.0f)); // triangle 2 : end
	//vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, -1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(-1.0f, 1.0f, 1.0f));
	//vertices.push_back(vec3(1.0f, -1.0f, 1.0f));

	//cube.SetPositions(vertices);
	cubeDraw->UpdateOpenGLBuffers();

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
	if (dragging) {

		vec2 delta = lastMousePosition - mousePosition;

		lastMousePosition = mousePosition;
		/*cameraPos.x += deltaX*0.01;
		cameraPos.y += deltaY*0.01;*/
		Orbit((float)delta.x * 0.1, vec3(0, 1, 0));
		Orbit((float)delta.y * 0.1, vec3(1, 0, 0));
	}

	if (elapsedTime < duration) {
		if (interpolation == LERP) {
			currentPos = lerp(initPos, finalPos, elapsedTime / duration);
		}
		else {
			currentPos = Hermite(elapsedTime / duration, initPos, vec3(0, 0, 0), vec3(1,0,0), finalPos);

		}
		elapsedTime += inDeltaTime;
	}
}

vec3 Lab1::Hermite(float t, const vec3 p1, const vec3 s1, const vec3 _p2, const vec3 s2) {
	float tt = t * t;

	float ttt = tt * t;
	vec3 p2 = _p2;

	float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
	float h2 = -2.0f * ttt + 3.0f * tt;
	float h3 = ttt - 2.0f * tt + t;
	float h4 = ttt - tt;
	vec3 result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
	return result;
}

void Lab1::Render(float inAspectRatio) {
	mat4 projection = perspective(fov, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(cameraPos, cameraTarget, vec3(0, 1, 0));
	//mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(0, 0, 1)));
	mat4 model;
	model.position = vec4(currentPos.x, currentPos.y, currentPos.y, 1);
	cubeDraw->Draw(DebugDrawMode::Triangles, vec3(1, 1, 0), projection*view*model);
}

//https://github.com/vurtun/nuklear/blob/master/example/extended.c
void Lab1::ImGui(nk_context* context) {
	nk_begin(context, "Controls", nk_rect(5.0f, 150.0f, 200.0f, 250.0f), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_NO_SCROLLBAR);
	nk_layout_row_dynamic(context, 30, 2);
	nk_label(context, "Duration", NK_TEXT_ALIGN_LEFT);
	nk_property_float(context, "Duration", 0, &duration, 100.f, 0.1, 1);
	nk_label(context, "Start position", NK_TEXT_ALIGN_LEFT);
	nk_layout_row_static(context, 25, 60, 3);
	nk_property_float(context, "x", -100, &initPos.x, 100, 0.1, 1);
	nk_property_float(context, "y", -100, &initPos.y, 100, 0.1, 1);
	nk_property_float(context, "z", -100, &initPos.z, 100, 0.1, 1);
	
	nk_label(context, "End position", NK_TEXT_ALIGN_LEFT);
	
	nk_layout_row_static(context, 25, 60, 3);
	nk_property_float(context, "x", -100, &finalPos.x, 100, 0.1, 1);
	nk_property_float(context, "y", -100, &finalPos.y, 100, 0.1, 1);
	nk_property_float(context, "z", -100, &finalPos.z, 100, 0.1, 1);
	
	nk_label(context, "Inteprolation", NK_TEXT_ALIGN_LEFT);
	if (nk_option_label(context, "Lerp", interpolation == LERP) && interpolation != LERP) {
		interpolation = LERP;		
		currentPos = initPos;		
		elapsedTime = 0;
	}
	if (nk_option_label(context, "Hermite", interpolation == SLERP) && interpolation != SLERP) {
		interpolation = SLERP;
		currentPos = initPos;
		elapsedTime = 0;
	}
	

	if (nk_button_label(context, "Reset")) {
		elapsedTime = 0;
	}
	struct nk_rect r = nk_window_get_bounds(context);
	if (mousePosition.x > r.x && mousePosition.x < (r.x + r.w) && mousePosition.y > r.y && mousePosition.y < (r.y + r.h)) {
		dragging = 0;
	}
	nk_end(context);

}

void Lab1::Shutdown() {
	
}
void Lab1::SetMousePosition(int x, int y) {
	mousePosition = vec2(x, y);
}


void Lab1::OnLeftMouseButtonDown() {
	lastMousePosition = mousePosition;
	dragging = 1;
}
void Lab1::OnLeftMouseButtonUp() {
	lastMousePosition = mousePosition;
	dragging = 0;
}

void Lab1::OnWheel(int amount) {
	fov -= amount / 120;
}


void Lab1::OnRightMouseButtonDown() {}
void Lab1::OnWheelMouseButtonDown() {}

void Lab1::OnRightMouseButtonUp() {}
void Lab1::OnWheelMouseButtonUp() {}