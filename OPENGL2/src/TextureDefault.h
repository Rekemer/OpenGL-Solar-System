#pragma once
#include <string>

class TextureDefault
{
	

public:
	TextureDefault(std::string type, bool isCube = false):isCube(isCube), type(type){};
	~TextureDefault();
	void LoadCubeMap(const char* mapDir);
	void Load(std::string fileName);
	void Unload();
	void Bind();
	unsigned int GetId() { return _textureID; }
	std::string GetType() { return type; }
private:
private:
	unsigned int _textureID;
	bool isCube;
	int _width;
	int _height;
	std::string type;
};

