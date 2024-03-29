#pragma once
#include <vector>
#include <xstring>
#include <ext/matrix_float4x4.hpp>

#include "Entity.h"
#include "../Rendering/VertexArray.h"

#define MAX_BONE_INFLUENCE 4
	struct Vertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];


		Vertex(float x, float y, float z,float u,float v)
		{
			Position.x = x;
			Position.y = y;
			Position.z = z;
			TexCoords.x = u;
			TexCoords.y = v;
		};
		Vertex() {};
	};


struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh : public Entity
{
public:
	/*Mesh() = default;
	Mesh(const Mesh& mesh) = default;*/
	Mesh(class Renderer*);
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture>& textures,Renderer* renderer);
	~Mesh();
	void Draw(class Shader& shader, class Model& model, glm::mat4x4& viewMat, glm::mat4x4& projMat);
	void Bind();
	void Bind(class Shader& shader);
	void LoadShader(const std::string& vertShaderPath, const std::string& fragShaderPath);
	void LoadTexture(std::string filepath);
	void Load(const float* verticies, const int  numVerticies, const int* indicies, const int  numInd);
	void Load(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	int GetNumIndices();
	class VertexArray& GetVertexArray() { return _va; }
	void SetupMesh();
	
private:
private:
	VertexArray _va;

	class Renderer* _renderer;

	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
};

