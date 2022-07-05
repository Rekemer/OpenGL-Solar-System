#pragma once
#include <ext/matrix_float4x4.hpp>

#include "Entity.h"

class Mesh : public Entity
{
public:
	Mesh(class Renderer*);
	~Mesh();
	void Bind();
	void Load(const float* verticies, const int  numVerticies, const int* indicies, const int  numInd);
	int GetNumIndices();
private:
private:
	class VertexArray* _va;
	
	class Shader* _shader;
	class Texture* _texture;
	class Renderer* _renderer;
};

