#pragma once
#include <xstring>
#include <ext/matrix_float4x4.hpp>

#include "Entity.h"

class Mesh : public Entity
{
public:
	Mesh(class Renderer*);
	~Mesh();
	void Bind();
	void LoadShader(const std::string& vertShaderPath, const std::string& fragShaderPath);
	void LoadTexture(std::string filepath);
	void Load(const float* verticies, const int  numVerticies, const int* indicies, const int  numInd);
	int GetNumIndices();
private:
private:
	class VertexArray* _va;
	
	class Shader* _shader;
	class Texture* _texture;
	class Renderer* _renderer;
};

