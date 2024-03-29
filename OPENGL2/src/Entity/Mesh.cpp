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

#include "Model.h"

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
	_va.Init(vertices, indices);
}

Mesh::~Mesh()
{
	
}

void Mesh::Draw(Shader& shader, Model& model, glm::mat4x4& viewMat, glm::mat4x4& projMat)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	//shader.SetMatrixUniform("worldMatrix", model.GetWorldMatrix());
	shader.SetMatrixUniform("projMatrix", projMat);
	// camera/view transformation
	shader.SetMatrixUniform("viewMatrix", viewMat);

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
	_va.Bind();
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);*/
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

void Mesh::Load(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
{
	this -> vertices = vertices;
	this -> indices  = indices;
	this -> textures = textures;
	_va.Init(vertices, indices);
}

void Mesh::SetupMesh()
{
	
    
}



void Mesh::Bind(Shader&shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;


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
	_va.Bind();
	
	
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
	return _va.GetNumIndices();
}


