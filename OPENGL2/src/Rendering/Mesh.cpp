#include "Mesh.h"

#include <iostream>
#include <ext/matrix_transform.hpp>

#include "Renderer.h"
#include "../Texture.h"
#include "Shader.h"
#include "VertexArray.h"

Mesh::Mesh(Renderer* renderer):_position(glm::vec3(0.0f,0.0f,0.0f)),
             _rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
             _scale(glm::vec3(1.0f, 1.0f, 1.0f)),
			_recomputeWorldMat(true),
			_renderer(renderer)
{
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
}


void Mesh::ComputeWorldTransform()
{
	if (!_recomputeWorldMat) return;
	// create scale matrix
	glm::mat4 identityMat = glm::mat4(1.0f);

	
	_worldMat = glm::translate(identityMat, _position);
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	_worldMat *= glm::scale(identityMat, _scale);

	if (_shader != nullptr)
	{
		_shader->SetActive();
		_shader->SetMatrixUniform("worldMatrix", _worldMat);
		_shader->SetMatrixUniform("viewProjMatrix", _renderer->GetOrthographicMatrix());
	}
	_recomputeWorldMat = false;
	

}

void Mesh::SetPosition(glm::vec3 pos)
{
	_position = pos;
}

void Mesh::SetRotation(glm::vec3 rot)
{
	_rotation = rot;
}

void Mesh::SetScale(glm::vec3 scale)
{
	_scale = scale;
}

void Mesh::Load(const float* verticies, const int numVerticies, const int* indicies, const int numInd)
{
	_va = new VertexArray(verticies, 4, indicies, 6);
	_texture = new Texture();
	_shader = new Shader();
	if (!_shader->Load("Shaders/basic.vert", "Shaders/basic.frag"))
	{
		std::cout << "failed to load shaders" << "\n";
	}
	_texture->Load("res/Wall.png");
}

int Mesh::GetNumIndices()
{
	return _va->GetNumIndices();
}
