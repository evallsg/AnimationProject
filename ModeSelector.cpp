#include "ModeSelector.h"

#include "examples/Lab1.h"
#include "examples/LabCurves.h"
#include "examples/PosesLab.h"
#include "examples/LabPoses.h"
#include "examples/LabMesh.h"

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
	static int selected = 0;
	static const  char* options[] = {"", "Lab 1", "Curves", "Example poses", "Poses", "Meshes" };
	enum examples { LAB1 = 1, CURVES, EXPOSES, POSES, MESHES };

	nk_begin(context, "Sample Selector", nk_rect(5.0f, 5.0f, 190.0f, 100), NK_WINDOW_MOVABLE | NK_WINDOW_NO_SCROLLBAR);
	if (nk_tree_push(context, NK_TREE_TAB, "Task selector", NK_MINIMIZED)) {
		nk_layout_row_static(context, 25, 168, 1);
		nk_label(context, "Select a Lab to Load", NK_TEXT_LEFT);

		if (nk_combo_begin_label(context, options[selected], nk_vec2(nk_widget_width(context), 200))) {
			nk_layout_row_dynamic(context, 25, 1);
			for (int i = 0; i <6; ++i)
				if (nk_combo_item_label(context, options[i], NK_TEXT_LEFT))
					selected = i;
			nk_combo_end(context);
		}

		switch (selected) {
		case LAB1:
			if (currentMode == 0 || typeid(*currentMode) != typeid(Lab1)) {
				currentMode = new Lab1();
				currentMode->Initialize();
			}
			break;

		case CURVES:
			if (currentMode == 0 || typeid(*currentMode) != typeid(LabCurves)) {
				currentMode = new LabCurves();
				currentMode->Initialize();
			}
			break;
		case EXPOSES:
			if (currentMode == 0 || typeid(*currentMode) != typeid(PosesLab)) {
				currentMode = new PosesLab();
				currentMode->Initialize();
			}
			break;
		case POSES:

			if (currentMode == 0 || typeid(*currentMode) != typeid(LabPoses)) {
				currentMode = new LabPoses();
				currentMode->Initialize();
			}
			break;
		case MESHES:

			if (currentMode == 0 || typeid(*currentMode) != typeid(LabMesh)) {
				currentMode = new LabMesh();
				currentMode->Initialize();
			}
			break;
		}
		struct nk_rect r = nk_window_get_bounds(context);
		if (currentMode != 0 && currentMode->mousePosition.x > r.x && currentMode->mousePosition.x < (r.x + r.w) && currentMode->mousePosition.y > r.y && currentMode->mousePosition.y < (r.y + r.h)) {
			currentMode->dragging = false;
		}
		nk_tree_pop(context);
	}

	/*if (nk_button_label(context, "Lab1") ) {
		if (currentMode == 0 || typeid(*currentMode) != typeid(Lab1)) {
			currentMode = new Lab1();
			currentMode->Initialize();
		}

	}
	if (nk_button_label(context, "Lab2") ) {
		if (currentMode == 0 || typeid(*currentMode) != typeid(LabCurves)) {
			currentMode = new LabCurves();
			currentMode->Initialize();
		}
	}
	if (nk_button_label(context, "Lab3")) {
		if (currentMode == 0 || typeid(*currentMode) != typeid(PosesLab)) {
			currentMode = new PosesLab();
			currentMode->Initialize();
		}
	}
	if (nk_button_label(context, "Poses")) {
		if (currentMode == 0 || typeid(*currentMode) != typeid(LabPoses)) {
			currentMode = new LabPoses();
			currentMode->Initialize();
		}
	}*/

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