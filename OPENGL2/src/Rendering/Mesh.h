#pragma once
#include <ext/matrix_float4x4.hpp>

#include "Renderer.h"

class Mesh
{
public:
	Mesh(class Renderer*);
	~Mesh();
	void Bind();
	void ComputeWorldTransform();
	glm::mat4 GetWorldMatrix() const { return _worldMat; }
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);
	void Load(const float* verticies, const int  numVerticies, const int* indicies, const int  numInd);
	int GetNumIndices();
private:
private:
	class VertexArray* _va;
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	glm::mat4 _worldMat;
	bool _recomputeWorldMat;
	class Shader* _shader;
	class Texture* _texture;
	class Renderer* _renderer;
};

