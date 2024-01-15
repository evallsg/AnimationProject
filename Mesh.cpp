#include "Mesh.h"
#include "Draw.h"

Mesh::Mesh() {
	//allocate memory
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mWeightAttrib = new Attribute<vec4>();
	mInfluenceAttrib = new Attribute<ivec4>();
	mIndexBuffer = new IndexBuffer();
}

//copy constructor
Mesh::Mesh(const Mesh& other) {
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mWeightAttrib = new Attribute<vec4>();
	mInfluenceAttrib = new Attribute<ivec4>();
	mIndexBuffer = new IndexBuffer();
	*this = other;
}

//assign mesh
Mesh& Mesh::operator=(const Mesh& other) {
	if (this == &other) {
		return *this;
	}
	//copy out the CPU values
	mPosition = other.mPosition;
	mNormal = other.mNormal;
	mTexCoord = other.mTexCoord;
	mWeights = other.mWeights;
	mInfluences = other.mInfluences;
	mIndices = other.mIndices;

	//upload the attribute data to the GPU
	UpdateOpenGLBuffers();
	return *this;
}

Mesh::~Mesh() {
	delete mPosAttrib;
	delete mNormAttrib;
	delete mUvAttrib;
	delete mWeightAttrib;
	delete mInfluenceAttrib;
	delete mIndexBuffer;
}

//getters
std::vector<vec3>& Mesh::GetPosition() {
	return mPosition;
}
std::vector<vec3>& Mesh::GetNormal() {
	return mNormal;
}
std::vector<vec2>& Mesh::GetTexCoord() {
	return mTexCoord;
}
std::vector<vec4>& Mesh::GetWeights() {
	return mWeights;
}
std::vector<ivec4>& Mesh::GetInfluences() {
	return mInfluences;
}
std::vector<unsigned int>& Mesh::GetIndices() {
	return mIndices;
}

//setters

void Mesh::SetPositions(std::vector<vec3> positions) {
	mPosition = positions;
}

//upload attribute data to the GPU 
void Mesh::UpdateOpenGLBuffers() {
	if (mPosition.size() > 0) {
		mPosAttrib->Set(mPosition);
	}
	if (mNormal.size() > 0) {
		mNormAttrib->Set(mNormal);
	}
	if (mTexCoord.size() > 0) {
		mUvAttrib->Set(mTexCoord);
	}
	if (mWeights.size() > 0) {
		mWeightAttrib->Set(mWeights);
	}
	if (mInfluences.size() > 0) {
		mInfluenceAttrib->Set(mInfluences);
	}
	if (mIndices.size() > 0) {
		mIndexBuffer->Set(mIndices);
	}
}

//bind the attributes to the specified slots
void Mesh::Bind(int position, int normal, int texCoord, int weight, int influcence) {
	if (position >= 0) {
		mPosAttrib->BindTo(position);
	}
	if (normal >= 0) {
		mNormAttrib->BindTo(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->BindTo(texCoord);
	}
	if (weight >= 0) {
		mWeightAttrib->BindTo(weight);
	}
	if (influcence >= 0) {
		mInfluenceAttrib->BindTo(influcence);
	}
}

//drawing calls to the GPU
void Mesh::Draw() {
	if (mIndices.size() > 0) {
		::Draw(*mIndexBuffer, DrawMode::Triangles);
	}
	else {
		::Draw(mPosition.size(), DrawMode::Triangles);
	}
}

void Mesh::DrawInstanced(unsigned int numInstances) {
	if (mIndices.size() > 0) {
		::DrawInstanced(*mIndexBuffer, DrawMode::Triangles, numInstances);
	}
	else {
		::DrawInstanced(mPosition.size(),
			DrawMode::Triangles, numInstances);
	}
}

//unbind the attributes to the specified slots
void Mesh::UnBind(int position, int normal, int texCoord, int weight, int influence) {
	if (position >= 0) {
		mPosAttrib->UnBindFrom(position);
	}
	if (normal >= 0) {
		mNormAttrib->UnBindFrom(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->UnBindFrom(texCoord);
	}
	if (weight >= 0) {
		mWeightAttrib->UnBindFrom(weight);
	}
	if (influence >= 0) {
		mInfluenceAttrib->UnBindFrom(influence);
	}
}

//CPU skinning using Transforms
void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose) {
	unsigned int numVerts = mPosition.size();
	if (numVerts == 0) { return; }

	//Make sure the skinned vectors have enough storage space
	mSkinnedPosition.resize(numVerts);
	mSkinnedNormal.resize(numVerts);
	Pose &bindPose = skeleton.GetBindPose();

	for (unsigned int i = 0; i < numVerts; i++) {
		ivec4 &joint = mInfluences[i];
		vec4 &weight = mWeights[i];

		//calculate the skin transform for the first joint that influences the vertex
		Transform skin0 = combine(pose[joint.x], inverse(bindPose[joint.x]));
		//vertex position in skin space
		vec3 p0 = transformPoint(skin0, mPosition[i]);
		//vertex normal in skin space
		vec3 n0 = transformVector(skin0, mNormal[i]);

		//calculate the skin transform for the second joint that influences the vertex
		Transform skin1 = combine(pose[joint.y], inverse(bindPose[joint.y]));
		//vertex position in skin space
		vec3 p1 = transformPoint(skin1, mPosition[i]);
		//vertex normal in skin space
		vec3 n1 = transformVector(skin0, mNormal[i]);

		//calculate the skin transform for the third joint that influences the vertex
		Transform skin2 = combine(pose[joint.z], inverse(bindPose[joint.x]));
		//vertex position in skin space
		vec3 p2 = transformPoint(skin2, mPosition[i]);
		//vertex normal in skin space
		vec3 n2 = transformVector(skin2, mNormal[i]);

		//calculate the skin transform for the fourth joint that influences the vertex
		Transform skin3 = combine(pose[joint.w], inverse(bindPose[joint.w]));
		//vertex position in skin space
		vec3 p3 = transformPoint(skin3, mPosition[i]);
		//vertex normal in skin space
		vec3 n3 = transformVector(skin3, mNormal[i]);

		//Scale each transformed position and normal by the weight of the joint it belongs to and add all four values together.
		//The resulting sum is the skinned position and normal 
		mSkinnedPosition[i] = p0 * weight.x + p1 * weight.y + p2 * weight.z + p3 * weight.w;
		mSkinnedNormal[i] = n0 * weight.x + n1 * weight.y + n2 * weight.z + n3 * weight.w;
	}
	mPosAttrib->Set(mSkinnedPosition);
	mNormAttrib->Set(mSkinnedNormal);
}

////CPU skinning using matrices
//void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose) {
//	unsigned int numVerts = mPosition.size();
//	if (numVerts == 0) { return; }
//
//	//Make sure the skinned vectors have enough storage space
//	mSkinnedPosition.resize(numVerts);
//	mSkinnedNormal.resize(numVerts);
//	
//	//get the matrix palette from the pose
//	pose.GetMatrixPalette(mPosePalette);
//	//get the inverse bind pose stored in the skeleton
//	std::vector<mat4> invPosePalette = skeleton.GetInvBindPose();	
//
//	for (unsigned int i = 0; i < numVerts; ++i) {
//		ivec4& joint = mInfluences[i];
//		vec4& weight = mWeights[i];
//
//		//get the skin matrix for each vertex
//		// the vertex should be transformed by the inverse bind pose first, then the animated pose. 
//		// With right-to-left multiplication: skin matrix = pose matrix * inverse bind pose
//		mat4 m0 = (mPosePalette[joint.x] * invPosePalette[joint.x]) * weight.x;
//		mat4 m1 = (mPosePalette[joint.y] * invPosePalette[joint.y]) * weight.y;
//		mat4 m2 = (mPosePalette[joint.z] * invPosePalette[joint.z]) * weight.z;
//		mat4 m3 = (mPosePalette[joint.w] * invPosePalette[joint.w]) * weight.w;
//		
//		// scale the resulting matrix by the weight of the vertex
//		mat4 skinMatrix = m0 + m1 + m2 + m3;
//
//		mSkinnedPosition[i] = transformPoint(skinMatrix, mPosition[i]);
//		mSkinnedNormal[i] = transformVector(skinMatrix, mNormal[i]);
//	}
//	mPosAttrib->Set(mSkinnedPosition);
//	mNormAttrib->Set(mSkinnedNormal);
//}