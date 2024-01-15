#include "GLTFLoader.h"
#include <iostream>
#include <algorithm>

//takes a path and returns a cgltf_data pointer
cgltf_data* LoadGLTFFile(const char* path) {
	cgltf_options options;
	memset(&options, 0, sizeof(cgltf_options));
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, path, &data);
	if (result != cgltf_result_success) {
		std::cout << "Could not load: " << path << "\n";
		return 0;
	}
	result = cgltf_load_buffers(&options, data, path);
	if (result != cgltf_result_success) {
		cgltf_free(data);
		std::cout << "Could not load: " << path << "\n";
		return 0;
	}
	result = cgltf_validate(data);
	if (result != cgltf_result_success) {
		cgltf_free(data);
		std::cout << "Invalid file: " << path << "\n";
		return 0;
	}
	return data;
}

void FreeGLTFFile(cgltf_data* data) {
	if (data == 0) {
		std::cout << "WARNING: Can't free null data\n";
	}
	else {
		cgltf_free(data);
	}
}

//gets the local transform of cgltf_node
Transform GLTFHelpers::GetLocalTransform(cgltf_node& n) {
	Transform result;
		
	//if the node stores its transformation as a matrix, use the mat4ToTransform decomposition function;
	if (n.has_matrix) {
		mat4 mat(&n.matrix[0]);
		result = mat4ToTransform(mat);
	}
	if (n.has_translation) {
		result.position = vec3(n.translation[0], n.translation[1], n.translation[2]);
	}
	if (n.has_rotation) {
		result.rotation = quat(n.rotation[0], n.rotation[1], n.rotation[2], n.rotation[3]);
	}

	if (n.has_scale) {
		result.scale = vec3(n.scale[0], n.scale[1], n.scale[2]);
	}
	return result;
}

int GLTFHelpers::GetNodeIndex(cgltf_node* target, cgltf_node* allNodes, unsigned int numNodes) {
	if (target == 0) {
		return -1;
	}
	for (unsigned int i = 0; i < numNodes; ++i) {
		if (target == &allNodes[i]) {
			return (int)i;
		}
	}
	return -1;
}

//reads the floating-point values of a gltf accessor
void GLTFHelpers::GetScalarValues(std::vector<float>& out, unsigned int compCount, const cgltf_accessor& inAccessor) {

	out.resize(inAccessor.count * compCount);
	for (cgltf_size i = 0; i < inAccessor.count; ++i) {
		cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
	}
}

//converts a glTF animation channel into a VectorTrack or a QuaternionTrack
template<typename T, int N>
void GLTFHelpers::TrackFromChannel(Track<T, N>& result, const cgltf_animation_channel& channel) {
		
	cgltf_animation_sampler& sampler = *channel.sampler;
	Interpolation interpolation = Interpolation::Constant;

	//make sure the Interpolation type of the track matches the cgltf_interpolation_type type of the sampler
	if (sampler.interpolation == cgltf_interpolation_type_linear) {
		interpolation = Interpolation::Linear;
	}
	else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) {
		interpolation = Interpolation::Cubic;
	}
	bool isSamplerCubic = interpolation == Interpolation::Cubic;
	result.SetInterpolation(interpolation);

	//convert sampler input and output accessors into linear arrays of floating-point numbers
	std::vector<float> time; // times
	GetScalarValues(time, 1, *sampler.input);

	std::vector<float> val; // values
	GetScalarValues(val, N, *sampler.output);

	unsigned int numFrames = sampler.input->count;
	unsigned int compCount = val.size() / time.size(); //components (vec3 or quat) per frame
	//resize the track to have enough room to store all the frames
	result.Resize(numFrames);

	//parse the time and value arrays into frame structures
	for (unsigned int i = 0; i < numFrames; ++i) {
		int baseIndex = i * compCount;
		Frame<N>& frame = result[i];
		//offset used to deal with cubic tracks since the input and output tangents are as large as the number of components
		int offset = 0;

		frame.mTime = time[i];

		//read input tangent (only if the sample is cubic)
		for (int comp = 0; comp < N; ++comp) {
			frame.mIn[comp] = isSamplerCubic ?
				val[baseIndex + offset++] : 0.0f;
		}
		//read the value
		for (int comp = 0; comp < N; ++comp) {
			frame.mValue[comp] = val[baseIndex +
				offset++];
		}
		//read the output tangent (only if the sample is cubic)
		for (int comp = 0; comp < N; ++comp) {
			frame.mOut[comp] = isSamplerCubic ?
				val[baseIndex + offset++] : 0.0f;
		}
	}
}

/*
It takes a mesh and a cgltf_attribute function, along with some additional data required for parsing. 
The attribute contains one of our mesh components, such as the position, normal, UV coordinate, weights, or influences.
This attribute provides the appropriate mesh data 
*/
void GLTFHelpers::MeshFromAttribute(Mesh& outMesh, cgltf_attribute& attribute, cgltf_skin* skin, cgltf_node* nodes, unsigned int nodeCount) {
	cgltf_attribute_type attribType = attribute.type;
	cgltf_accessor& accessor = *attribute.data;

	//get how many attributes the current component has
	unsigned int componentCount = 0;
	if (accessor.type == cgltf_type_vec2) {
		componentCount = 2;
	}
	else if (accessor.type == cgltf_type_vec3) {
		componentCount = 3;
	}
	else if (accessor.type == cgltf_type_vec4) {
		componentCount = 4;
	}

	//Parse the data
	std::vector<float> values;
	GetScalarValues(values, componentCount, accessor);
	unsigned int acessorCount = accessor.count;

	//Create references to the position, normal, texture coordinate, influences, and weights vectors of the mesh
	std::vector<vec3>& positions = outMesh.GetPosition();
	std::vector<vec3>& normals = outMesh.GetNormal();
	std::vector<vec2>& texCoords = outMesh.GetTexCoord();
	std::vector<ivec4>& influences = outMesh.GetInfluences();
	std::vector<vec4>& weights = outMesh.GetWeights();
	
	//loop through all the values in the current accessor and assign them to the appropriate vector based on the accessor type
	for (unsigned int i = 0; i < acessorCount; ++i) {
		int index = i * componentCount;
		switch (attribType)
		{
		case cgltf_attribute_type_position:
			positions.push_back(vec3(values[index + 0], values[index + 1], values[index + 2]));
			break;

		case cgltf_attribute_type_normal:
		{
			vec3 normal = vec3(values[index + 0], values[index + 1], values[index + 2]);
			//if the normal is not normalized, normalize it
			if (lenSq(normal) < 0.000001f) {
				normal = vec3(0, 1, 0);
			}
			normals.push_back(normalized(normal));
		}
		break;

		case cgltf_attribute_type_texcoord:
			texCoords.push_back(vec2(values[index + 0], values[index + 1]));
			break;

		case cgltf_attribute_type_weights:
			weights.push_back(vec4(values[index + 0], values[index + 1], values[index + 2], values[index + 3]));
			break;

		case cgltf_attribute_type_joints:
		{
			// Joints are stored as floating-point numbers.Convert them into integers: 
			// These indices are skin relative. This function has no information about the skin that is being parsed.
			// Add +0.5f to round, since we can't read integers
			ivec4 joints(
				(int)(values[index + 0] + 0.5f),
				(int)(values[index + 1] + 0.5f),
				(int)(values[index + 2] + 0.5f),
				(int)(values[index + 3] + 0.5f)
			);

			// convert the joint indices so that they go from being relative to the joints array to being relative to the skeleton hierarchy
			joints.x = GetNodeIndex(skin->joints[joints.x], nodes, nodeCount);
			joints.y = GetNodeIndex(skin->joints[joints.y], nodes, nodeCount);
			joints.z = GetNodeIndex(skin->joints[joints.z], nodes, nodeCount);
			joints.w = GetNodeIndex(skin->joints[joints.w], nodes, nodeCount);
			
			//Make sure that even the invalid nodes have a value of 0 (any negative joint indices will break the skinning implementation)
			joints.x = std::max(0, joints.x);
			joints.y = std::max(0, joints.y);
			joints.z = std::max(0, joints.z);
			joints.w = std::max(0, joints.w);

			influences.push_back(joints);
		}
		break;
		}
	}
}

Pose LoadRestPose(cgltf_data* data) {
	unsigned int numBones = data->nodes_count;
	Pose result(numBones);
	//for each bone node
	for (unsigned int i = 0; i < numBones; ++i) {
		cgltf_node* node = &(data->nodes[i]);
		//assign the local transform to the pose
		Transform transform = GLTFHelpers::GetLocalTransform(data->nodes[i]);
		result.SetLocalTransform(i, transform);
		//set the parent to the pose joint
		int parent = GLTFHelpers::GetNodeIndex(node->parent, data->nodes, numBones);
		result.SetParent(i, parent);
	}
	return result;
}

Pose LoadBindPose(cgltf_data* data) {
	Pose restPose = LoadRestPose(data);
	unsigned int numBones = restPose.Size();
	std::vector<Transform> worldBindPose(numBones);
	for (unsigned int i = 0; i < numBones; i++) {
		worldBindPose[i] = restPose.GetGlobalTransform(i);
	}

	unsigned int numSkins = data->skins_count;
	for (unsigned int i = 0; i < numSkins; i++) {
		cgltf_skin* skin = &(data->skins[i]);
		// Read the inverse_bind_matrices accessor into a large vector of float values
		std::vector<float> invBindAccessor;
		//the size of invBinAccessor should be numJoints * 16 (number elements of a 4x4 matrix)
		GLTFHelpers::GetScalarValues(invBindAccessor, 16, *skin->inverse_bind_matrices);

		//for each joint in the skin, get the inverse bind matrix
		unsigned int numJoints = skin->joints_count;
		for (unsigned int j = 0; j < numJoints; j++) {
			//Read the inverse bind matrix of the joint
			float* matrix = &(invBindAccessor[j * 16]);
			mat4 invBindMatrix = mat4(matrix);

			//invert, convert to transform
			mat4 bindMatrix = inverse(invBindMatrix);
			Transform bindTransform = mat4ToTransform(bindMatrix);

			// Set that transform in the worldBindPose.
			cgltf_node* jointNode = skin->joints[j];
			int jointIndex = GLTFHelpers::GetNodeIndex(jointNode, data->nodes, numBones);
			worldBindPose[jointIndex] = bindTransform;
		} //end of each joint
	} //end of each skin

	//Convert the world bind pose to a regular bind pose
	Pose bindPose = restPose;
	for (unsigned int i = 0; i < numBones; i++) {
		Transform current = worldBindPose[i];
		int parentId = bindPose.GetParent(i);
		if (parentId >= 0) {
			//Bring into parent space
			Transform parent = worldBindPose[parentId];
			current = combine(inverse(parent), current);
		}
		bindPose.SetLocalTransform(i, current);
	}
	return bindPose;
} 

//loads the names of every joint in the same order that the joints for the rest pose were loaded
std::vector<std::string> LoadJointNames(cgltf_data* data) {
	unsigned int numBones = (unsigned int)data->nodes_count;
	std::vector<std::string> result(numBones, "Not Set");
	for (unsigned int i = 0; i < numBones; ++i) {
		cgltf_node* node = &(data->nodes[i]);
		if (node->name == 0) {
			result[i] = "EMPTY NODE";
		}
		else {
			result[i] = node->name;
		}
	}
	return result;
}

std::vector<Clip> LoadAnimationClips(cgltf_data* data) {
	unsigned int numClips = data->animations_count;
	unsigned int numNodes = data->nodes_count;

	std::vector<Clip> result;
	result.resize(numClips);

	for (unsigned int i = 0; i < numClips; ++i) {
		//set the name of the clip
		result[i].SetName(data->animations[i].name);
		
		//each channel of a glTF file is an animation track
		unsigned int numChannels = data->animations[i].channels_count;
		for (unsigned int j = 0; j < numChannels; ++j) {
			cgltf_animation_channel& channel = data->animations[i].channels[j];
			cgltf_node* target = channel.target_node;

			//find the index of the node that the current channel affects
			int nodeId = GLTFHelpers::GetNodeIndex(target, data->nodes, numNodes);
			if (channel.target_path == cgltf_animation_path_type_translation) {
				VectorTrack& track = result[i][nodeId].GetPositionTrack();
				//convert the track into an animation track
				GLTFHelpers::TrackFromChannel<vec3, 3>(track, channel);
			}
			else if (channel.target_path == cgltf_animation_path_type_scale) {
				VectorTrack& track = result[i][nodeId].GetScaleTrack();
				//convert the track into an animation track
				GLTFHelpers::TrackFromChannel<vec3, 3>(track, channel);
			}
			else if (channel.target_path == cgltf_animation_path_type_rotation) {
				QuaternionTrack& track = result[i][nodeId].GetRotationTrack();
				//convert the track into an animation track
				GLTFHelpers::TrackFromChannel<quat, 4>(track, channel);
			}
		} // End num channels loop
		result[i].RecalculateDuration();
	} // End num clips loop

	return result;
}

Skeleton LoadSkeleton(cgltf_data* data) {
	return Skeleton(
		LoadRestPose(data),
		LoadBindPose(data),
		LoadJointNames(data)
	);
}

std::vector<Mesh> LoadMeshes(cgltf_data* data) {
	std::vector<Mesh> result;
	cgltf_node* nodes = data->nodes;
	unsigned int nodeCount = data->nodes_count;

	for (unsigned int i = 0; i < nodeCount; ++i) {
		cgltf_node* node = &nodes[i];
		// Only process nodes that have both a mesh and a skin
		if (node->mesh == 0 || node->skin == 0) {
			continue;
		}

		int numPrims = node->mesh->primitives_count;
		for (int j = 0; j < numPrims; ++j) {

			//create a mesh for each primitive
			result.push_back(Mesh());
			Mesh& mesh = result[result.size() - 1];

			cgltf_primitive* primitive = &node->mesh->primitives[j];

			// Loop through all the attributes in the primitive and populate the mesh data
			unsigned int ac = primitive->attributes_count;
			for (unsigned int k = 0; k < ac; ++k) {
				cgltf_attribute* attribute = &primitive->attributes[k];
				GLTFHelpers::MeshFromAttribute(mesh, *attribute, node->skin, nodes, nodeCount);

				// if the primitive has indices, the index buffer of the mesh needs to be filled out
				if (primitive->indices != 0) {
					int ic = primitive->indices->count;
					std::vector<unsigned int>& indices = mesh.GetIndices();
					indices.resize(ic);

					for (unsigned int k = 0; k < ic; ++k) {
						indices[k] = cgltf_accessor_read_index(primitive->indices, k);
					}
				}
				//for render the mesh
				mesh.UpdateOpenGLBuffers();				
			}
			
		}
	}	//return the resulting vector of meshes
	return result;}
