#include "Mesh.h"


#include <iostream>
#include <ext/matrix_transform.hpp>
#include "../Rendering/Shader.h"
#include "Entity.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Shader.h"
#include "../Rendering/VertexArray.h"
#include <glfw3.h>
#include "Camera.h"
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
Mesh::Mesh(Renderer* renderer): Entity()
{
	_renderer = renderer;
}

Mesh::Mesh(std::vector<Vertex>& vertices,
	std::vector<unsigned> indices,
	std::vector<Texture>& textures,
	Renderer *renderer)
{
	_renderer = renderer;
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	_va = new VertexArray(vertices, indices);
}

Mesh::~Mesh()
{
	//delete _va;
	//_va = nullptr;
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	shader.SetMatrixUniform("worldMatrix", _worldMat);
	shader.SetMatrixUniform("projMatrix", _renderer->GetPerspectiveMatrix());
	// camera/view transformation
	shader.SetMatrixUniform("viewMatrix", _renderer->GetCamera()->GetViewMatrix());

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		
		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	

	// draw mesh
	_va->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
}

void Mesh::Load(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
{
	this -> vertices = vertices;
	this -> indices  = indices;
	this -> textures = textures;
	_va = new VertexArray(vertices, indices);
}

void Mesh::SetupMesh()
{
	
    
}

void Mesh::Bind()
{
	//_va->Bind();
	//if (_texture !=nullptr)
	//{
	//	_texture->Bind();
	//}
	//if (_shader != nullptr)
	//{
	//	_shader->Bind();
	//	_shader->SetMatrixUniform("worldMatrix", _worldMat);
	//	_shader->SetMatrixUniform("projMatrix", _renderer->GetPerspectiveMatrix());
	//	// camera/view transformation
	//	_shader->SetMatrixUniform("viewMatrix", _renderer->GetCamera()->GetViewMatrix());
	//}
	
	
}



void Mesh::LoadTexture(const std::string filepath)
{
	//_texture = new TextureDefault();
	//_texture->Load(filepath);
}


void Mesh::Load(const float* verticies, const int numVerticies, const int* indicies, const int numInd)
{
	//_va = new VertexArray(verticies, numVerticies, indicies, numInd);
	
}

int Mesh::GetNumIndices()
{
	return _va->GetNumIndices();
}


