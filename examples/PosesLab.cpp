#include "PosesLab.h"
#include "../GLTFLoader.h"

vec3 cameraPos = vec3(0, 4, 7);
vec3 cameraTarget = vec3(0, 4, 0);

void PosesLab::Initialize() {

	cgltf_data* gltf = LoadGLTFFile("./assets/Woman.gltf");
	mRestPose = LoadRestPose(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	/*float x = (float)(2 * (this->width/2) / width - 1);
	float y = (float)(2 * (this->height /2) / height - 1);
	vec4 topright = vec4(x, y, 0.01f, 1);
	mat4 view = lookAt(cameraPos, cameraTarget, vec3(0, 1, 0));
	mat4 view_invert = inverse(view);
	mat4 projection = perspective(60.0f, this->width/this->height, 0.01f, 1000.0f);
	mat4 projection_invert = inverse(projection);
	vec4 origin = view_invert* projection_invert* topright;
	

	mUpAxis = new DebugDraw();
	mUpAxis->Push(vec3(origin.x, origin.y, origin.z));
	mUpAxis->Push(vec3(origin.x, origin.y+1, origin.z));
	mUpAxis->UpdateOpenGLBuffers();*/

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

	mRestPoseVisual = new DebugDraw();
	mRestPoseVisual->FromPose(mRestPose);
	mRestPoseVisual->UpdateOpenGLBuffers();

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

void PosesLab::Update(float deltaTime) {
	mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + deltaTime);
	mCurrentPoseVisual->FromPose(mCurrentPose);
}

void PosesLab::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(cameraPos, cameraTarget, vec3(0, 1, 0));
	mat4 mvp = projection * view; // No model

	if (mDebug) {
		mUpAxis->Draw(DebugDrawMode::Lines, vec3(0, 1, 0), mvp);
		mRightAxis->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), mvp);
		mForwardAxis->Draw(DebugDrawMode::Lines, vec3(0, 0, 1), mvp);

	}

	mRestPoseVisual->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), mvp);
	mCurrentPoseVisual->UpdateOpenGLBuffers();
	mCurrentPoseVisual->Draw(DebugDrawMode::Lines, vec3(0, 0, 1), mvp);
}

void PosesLab::Shutdown() {
	delete mRestPoseVisual;
	delete mCurrentPoseVisual;
	mClips.clear();
}

void PosesLab::ImGui(nk_context* ctx) {
	/* init gui state */
	if (nk_begin(ctx, "Controls", nk_rect(5.0f, 150.f, 190.0f, 50), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
		/* fixed widget pixel width */
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_checkbox_label(ctx, "Show origin", &mDebug)){
			/* event handling */
			//mDebug = !mDebug;
		}
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


void PosesLab::OnKeyDown(int keyCode) {
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