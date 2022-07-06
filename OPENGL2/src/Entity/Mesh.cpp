#include "Mesh.h"


#include <iostream>
#include <ext/matrix_transform.hpp>

#include "Entity.h"
#include "../Rendering/Renderer.h"
#include "../Texture.h"
#include "../Rendering/Shader.h"
#include "../Rendering/VertexArray.h"
#include <glfw3.h>
#include "Camera.h"
Mesh::Mesh(Renderer* renderer): Entity()
{
	_renderer = renderer;
}

Mesh::~Mesh()
{
}

void Mesh::Bind()
{
	_va->Bind();
	if (_texture !=nullptr)
	{
		_texture->SetActive();
	}
	if (_shader != nullptr)
	{
		_shader->SetActive();
		_shader->SetMatrixUniform("worldMatrix", _worldMat);
		_shader->SetMatrixUniform("projMatrix", _renderer->GetPerspectiveMatrix());
		// camera/view transformation
		_shader->SetMatrixUniform("viewMatrix", _renderer->GetCamera()->GetViewMatrix());
	}
	
	
}


void Mesh::LoadShader(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	_shader = new Shader();
	if (!_shader->Load(vertShaderPath, fragShaderPath))
	{
		std::cout << "failed to load shaders" << "\n";
	}
}

void Mesh::LoadTexture(const std::string filepath)
{
	_texture = new Texture();
	_texture->Load("res/Wall.png");
}


void Mesh::Load(const float* verticies, const int numVerticies, const int* indicies, const int numInd)
{
	_va = new VertexArray(verticies, numVerticies, indicies, numInd);
	
}

int Mesh::GetNumIndices()
{
	return _va->GetNumIndices();
}
