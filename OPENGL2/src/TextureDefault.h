#pragma once
#include <string>

class TextureDefault
{
	

public:
	TextureDefault(std::string type): type(type){};
	~TextureDefault();
	void Load(std::string fileName);
	void Unload();
	void Bind();
	unsigned int GetId() { return _textureID; }
	std::string GetType() { return type; }
private:
private:
	unsigned int _textureID;
	int _width;
	int _height;
	std::string type;
};

