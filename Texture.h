#pragma once

//load a texture from a file, bind a texture index to a uniform index, and deactivate a texture index
class Texture
{
protected:
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mChannels;
	unsigned int mHandle;
private:
	Texture(const Texture& other);
	Texture& operator=(const Texture& other);
public:
	Texture();
	Texture(const char* path);
	~Texture();
	void Load(const char* path);
	void Set(unsigned int uniform, unsigned int texIndex);
	void UnSet(unsigned int textureIndex);
	unsigned int GetHandle();
};
