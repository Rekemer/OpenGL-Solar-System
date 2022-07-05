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
	_position = glm::vec3(0.f, 0.f, -4.f);
}

Mesh::~Mesh()
{
}

void Mesh::Bind()
{
	_va->Bind();
	_texture->SetActive();
	_shader->SetActive();
	_shader->SetMatrixUniform("worldMatrix", _worldMat);
	_shader->SetMatrixUniform("projMatrix", _renderer->GetPerspectiveMatrix());
	// camera/view transformation
	_shader->SetMatrixUniform("viewMatrix", _renderer->GetCamera()->GetViewMatrix());
}





void Mesh::Load(const float* verticies, const int numVerticies, const int* indicies, const int numInd)
{
	_va = new VertexArray(verticies, numVerticies, indicies, numInd);
	_texture = new Texture();
	_shader = new Shader();
	if (!_shader->Load("Shaders/basic.vert", "Shaders/basic.frag"))
	{
		std::cout << "failed to load shaders" << "\n";
	}
	_texture->Load("res/Morgana.jpg");
}

int Mesh::GetNumIndices()
{
	return _va->GetNumIndices();
}
