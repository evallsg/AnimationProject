#ifndef _H_Lab1_
#define _H_Lab1_

#include "../vec3.h"
#include "../vec2.h"

#include "../Application.h"
#include "../Shader.h"
#include "../Attribute.h"
#include "../IndexBuffer.h"
#include "../Texture.h"

#include "../math_includes.h"

class Lab1 : public Application {
protected:
	Shader* mShader;
	Attribute<vec3>* mVertexPositions;
	Attribute<vec3>* mVertexNormals;
	Attribute<vec2>* mVertexTexCoords;
	IndexBuffer* mIndexBuffer;
	Texture* mDisplayTexture;
	float mRotation;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();

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