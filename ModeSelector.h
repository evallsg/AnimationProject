#pragma once
#pragma once
#ifndef _H_ModeSelector_
#define _H_ModeSelector_
#include "Application.h"
#include "examples/DebugDraw.h"
#include <vector>


class ModeSelector {
private:
	ModeSelector(const ModeSelector&);
	ModeSelector& operator=(const ModeSelector&);
protected:
	Application* currentMode;
	bool mIsRunning;

public:
	ModeSelector();
	~ModeSelector();
	
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
	void ImGui(nk_context* inContext);
	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);
	bool IsRunning();

	Application* GetCurrentMode();
};

#endif
