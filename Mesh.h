#pragma once
#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

#include "Attribute.h"
#include "IndexBuffer.h"
#include "Skeleton.h"

class Mesh {
protected:
	std::vector<vec3> mPosition;
	std::vector<vec3> mNormal;
	std::vector<vec2> mTexCoord;
	//each vertex can be influenced at most by 4 bones
	std::vector<vec4> mWeights;
	std::vector<ivec4> mInfluences;
	std::vector<unsigned int> mIndices;

	//for GPU skinning
	Attribute<vec3>* mPosAttrib;
	Attribute<vec3>* mNormAttrib;
	Attribute<vec2>* mUvAttrib;
	Attribute<vec4>* mWeightAttrib;
	Attribute<ivec4>* mInfluenceAttrib;
	IndexBuffer* mIndexBuffer;

	//for CPU skinning
	std::vector<vec3> mSkinnedPosition;
	std::vector<vec3> mSkinnedNormal;
	std::vector<mat4> mPosePalette;
public:
	Mesh();
	Mesh(const Mesh&);
	Mesh& operator=(const Mesh&);
	~Mesh();

	//getters
	std::vector<vec3>& GetPosition();
	std::vector<vec3>& GetNormal();
	std::vector<vec2>& GetTexCoord();
	std::vector<vec4>& GetWeights();
	std::vector<ivec4>& GetInfluences();
	std::vector<unsigned int>& GetIndices();

	// applies CPU mesh-skinning
	void CPUSkin(Skeleton& skeleton, Pose& pose);
	//syncs the vectors holding data to the GPU
	void UpdateOpenGLBuffers();
	
	void Bind(int position, int normal, int texCoord, int weight, int influence);
	void Draw();
	void DrawInstanced(unsigned int numInstances);
	void UnBind(int position, int normal, int texCoord, int weight, int influence);
};