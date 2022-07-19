#pragma once
#include <string>

class TextureDefault
{
	

public:
	TextureDefault();
	~TextureDefault();
	void Load(std::string fileName);
	void Unload();
	void SetActive();
private:
private:
	unsigned int _textureID;
	int _width;
	int _height;
};

