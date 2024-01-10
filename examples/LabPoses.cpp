#include "LabPoses.h"
#include "../GLTFLoader.h"



void LabPoses::Initialize() {
	cameraPos = vec3(0, 4, 7);
	cameraTarget = vec3(0, 4, 0);
	fov = 60.0f;
	cgltf_data* gltf = LoadGLTFFile("./assets/ComputerAnimation.gltf");
	Skeleton skeleton = LoadSkeleton(gltf);
	mRestPose = skeleton.GetRestPose();
	mBindPose = skeleton.GetBindPose();
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	//Debug origin gizmo
	mUpAxis = new DebugDraw();
	mUpAxis->Push(vec3(0, 0, 0));
	mUpAxis->Push(vec3(1, 0, 0));
	mUpAxis->UpdateOpenGLBuffers();

	mRightAxis = new DebugDraw();
	mRightAxis->Push(vec3(0, 0, 0));
	mRightAxis->Push(vec3(1, 0, 0));
	mRightAxis->UpdateOpenGLBuffers();

	mForwardAxis = new DebugDraw();
	mForwardAxis->Push(vec3(0, 0, 0));
	mForwardAxis->Push(vec3(0, 0, -1));
	mForwardAxis->UpdateOpenGLBuffers();

	//Rest Pose
	mRestPoseVisual = new DebugDraw();
	mRestPoseVisual->FromPose(mRestPose);
	mRestPoseVisual->UpdateOpenGLBuffers();

	//Bind Pose
	mBindPoseVisual = new DebugDraw();
	mBindPoseVisual->FromPose(mBindPose);
	mBindPoseVisual->UpdateOpenGLBuffers();

	mCurrentClip = 0;
	mCurrentPose = mRestPose;

	mCurrentPoseVisual = new DebugDraw();
	mCurrentPoseVisual->FromPose(mCurrentPose);
	mCurrentPoseVisual->UpdateOpenGLBuffers();

	// For the UI
	unsigned int numUIClips = (unsigned int)mClips.size();
	for (unsigned int i = 0; i < numUIClips; ++i) {
		if (mClips[i].GetName() == "Walking") {
			mCurrentClip = i;
			break;
		}
	}
}

void LabPoses::Update(float deltaTime) {
	if (dragging) {
	
		vec2 delta = lastMousePosition - mousePosition;
		//int deltaY = lastMousePositionY - mousePositionY;

		//lastMousePositionX = mousePositionX;
		//lastMousePositionY = mousePositionY;
		/*cameraPos.x += deltaX*0.01;
		cameraPos.y += deltaY*0.01;*/
		Orbit((float)delta.x*0.1, vec3(0, 1, 0));
		Orbit((float)delta.y * 0.1, vec3(1, 0, 0));
	}

	mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + deltaTime);
	mCurrentPoseVisual->FromPose(mCurrentPose);
}

void LabPoses::Render(float inAspectRatio) {

	mat4 projection = perspective(fov, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(cameraPos, cameraTarget, vec3(0, 1, 0));
	mat4 mvp = projection * view; // No model

	if (mDebug) {
		mUpAxis->Draw(DebugDrawMode::Lines, vec3(0, 1, 0), mvp);
		mRightAxis->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), mvp);
		mForwardAxis->Draw(DebugDrawMode::Lines, vec3(0, 0, 1), mvp);

	}
	if (mShowRestPose) {
		//Rest pose
		mRestPoseVisual->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), mvp);
	}
	if (mShowBindPose) {
		//Bind pose
		mRestPoseVisual->Draw(DebugDrawMode::Lines, vec3(0, 1, 0), mvp);
	}

	//Current pose (animated)
	mCurrentPoseVisual->UpdateOpenGLBuffers();
	mCurrentPoseVisual->Draw(DebugDrawMode::Lines, vec3(0, 0, 1), mvp);
}

void LabPoses::Shutdown() {
	delete mRestPoseVisual;
	delete mBindPoseVisual;
	delete mCurrentPoseVisual;
	mClips.clear();
}

void LabPoses::ImGui(nk_context* ctx) {
	/* init gui state */
	if (nk_begin(ctx, "Controls", nk_rect(5.0f, 150.f, 190.0f, 150), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
		/* fixed widget pixel width */
		nk_layout_row_static(ctx, 30, 80, 1);
		
		nk_checkbox_label(ctx, "Show origin", &mDebug);
		nk_checkbox_label(ctx, "Show Rest Pose", &mShowRestPose);
		nk_checkbox_label(ctx, "Show Bind Pose", &mShowBindPose);
	}


	/////* fixed widget window ratio width */
	////nk_layout_row_dynamic(&ctx, 30, 2);
	////if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
	////if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;

	/////* custom widget pixel width */
	////nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
	////{
	////	nk_layout_row_push(&ctx, 50);
	////	nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
	////	nk_layout_row_push(&ctx, 110);
	////	nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
	////}
	//nk_layout_row_end(ctx);
	nk_end(ctx);
}


void LabPoses::OnKeyDown(int keyCode) {
	switch (keyCode)
	{
	case 0x25: //left arrow
		cameraPos.x++;
		break;
	case 0x27: //right arrow
		cameraPos.x--;
		break;
	case 0x26: //up arrow
		cameraPos.y++;
		break;
	case 0x28: //down arrow
		cameraPos.y--;
		break;
	case 0xBB: //+
		cameraPos.z++;
		break;
	case 0xBD: //-
		cameraPos.z--;
		break;
	default:
		break;
	}
}

void LabPoses::OnLeftMouseButtonDown() {
	lastMousePosition = mousePosition;
	dragging = 1;
}
void LabPoses::OnLeftMouseButtonUp() {
	lastMousePosition = mousePosition;
	dragging = 0;
}

void LabPoses::OnWheel(int amount) {
	fov -= amount / 120;
}

void LabPoses::SetMousePosition(int x, int y) {
	mousePosition = vec2(x, y);
}

void LabPoses::Orbit(float angle, vec3 axis) {
	vec3 front = cameraPos - cameraTarget;
	//normalize(front);
	quat rotation = angleAxis(angle, axis);
	Transform m;
	m.rotation = rotation;

	vec3 newFront = transformVector(m, front);
	cameraPos = newFront + cameraTarget;

}