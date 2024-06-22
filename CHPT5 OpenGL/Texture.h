#pragma once

#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
private:
	unsigned int mTextureID = 0;
	int mWidth = 0;
	int mHeight = 0;
};

