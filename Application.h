#ifndef _H_APPLICATION_
#define _H_APPLICATION_

#include "vec3.h"
#include "vec2.h"

#include "Shader.h"
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "math_includes.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "./include/nuklear/nuklear.h"

class Application {
private:
	Application(const Application&);
	Application& operator=(const Application&);
public:

	vec2 mousePosition;
	int dragging;

	float fov;
	vec3 cameraPos;
	vec3 cameraTarget;

	int width;
	int height;

	Application();
	inline virtual ~Application() { }
	inline virtual void Initialize() { }
	inline virtual void Initialize(int width, int height) { this->width = width; this->height = height; }
	inline virtual void Update(float inDeltaTime) { }
	inline virtual void Render(float inAspectRatio) { }
	inline virtual void Shutdown() { }

	inline virtual void ImGui(nk_context* inContext) { }

	inline virtual void OnLeftMouseButtonDown() { }
	inline virtual void OnRightMouseButtonDown() { }
	inline virtual void OnWheelMouseButtonDown() { }

	inline virtual void OnLeftMouseButtonUp() { }
	inline virtual void OnRightMouseButtonUp() { }
	inline virtual void OnWheelMouseButtonUp() { }
	inline virtual void OnWheel(int keyCode) { }

	inline virtual void OnKeyUp(int keyCode) { }
	inline virtual void OnKeyDown(int keyCode) { }

	inline virtual void SetMousePosition(int x, int y) { }

	virtual void Orbit(float angle, vec3 axis);

};
#endif