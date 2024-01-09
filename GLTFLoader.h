#pragma once
#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_
#include "./include/cgltf/cgltf.h"
#include "Pose.h"
#include "Transform.h"
#include "Track.h"
#include "Clip.h"
#include "Skeleton.h"

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* handle);
Pose LoadRestPose(cgltf_data* data);
Pose LoadBindPose(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);
Skeleton LoadSkeleton(cgltf_data* data);

namespace GLTFHelpers {
	//gets the local transform of cgltf_node
	Transform GetLocalTransform(cgltf_node& n);

	int GetNodeIndex(cgltf_node* target, cgltf_node* allNodes, unsigned int numNodes);

	//reads the floating-point values of a gltf accessor
	void GetScalarValues(std::vector<float>& out, unsigned int compCount, const cgltf_accessor& inAccessor);
	template<typename T, int N>
	void TrackFromChannel(Track<T, N>& result, const cgltf_animation_channel& channel);
}
#endif