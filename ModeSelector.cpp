#include "ModeSelector.h"

#include "examples/Lab1.h"
#include "examples/LabCurves.h"
#include "examples/PosesLab.h"

ModeSelector::ModeSelector() {
	mIsRunning = false;
	currentMode = 0;
}

void ModeSelector::ImGui(nk_context* context) {
	/*if (currentMode != 0) {
		nk_begin(context, "Sample Exit", nk_rect(0, 175.0f + 125.0f, 200.0f, 33.0f), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR);
		nk_layout_row_static(context, 20, 180, 1);

		if (nk_button_label(context, "Close Current Mode")) {
			currentMode->Shutdown();
			delete currentMode;
			currentMode = new Application();;
		}

		nk_end(context);
	}
	else {*/
		nk_begin(context, "Sample Selector", nk_rect(5.0f, 5.0f, 190.0f, 150.0f), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR);
		nk_layout_row_static(context, 25, 168, 1);

		nk_label(context, "Select a Lab to Load", NK_TEXT_LEFT);
		if (nk_button_label(context, "Lab1")) {
			currentMode = new Lab1();
			currentMode->Initialize();
		}
		if (nk_button_label(context, "Lab2")) {
			currentMode = new LabCurves();
			currentMode->Initialize();
		}
		if (nk_button_label(context, "Lab3")) {
			currentMode = new PosesLab();
			currentMode->Initialize();
		}


		nk_end(context);
	/*}*/
}

Application* ModeSelector::GetCurrentMode() {
	return currentMode;
}

ModeSelector::~ModeSelector() {
	
	if (currentMode != 0) {
		std::cout << "Error sample still exists\n";
	}
}

void ModeSelector::Initialize() {
	
	if (mIsRunning) {
		std::cout << "Starting sample selector multiple times, might leak memory and resources\n";
	}

	mIsRunning = true;
	currentMode = 0;
}

void ModeSelector::Shutdown() {
	if (!mIsRunning) {
		std::cout << "Shutting down inactive sample selector\n";
	}
	if (currentMode != 0) {
		currentMode->Shutdown();
		delete currentMode;
	}
	currentMode = 0;
	mIsRunning = false;
}

bool ModeSelector::IsRunning() {
	return mIsRunning;
}

void ModeSelector::OnKeyUp(int keyCode) {
	if (keyCode == 0x4D) { //M
	
		currentMode = new Lab1();
		currentMode->Initialize();
	}
	if (keyCode == 0x43) { //C

		currentMode = new LabCurves();
		currentMode->Initialize();
	}
	if (keyCode == 0x50) { //P
			
		currentMode = new PosesLab();
		currentMode->Initialize();
	}
}