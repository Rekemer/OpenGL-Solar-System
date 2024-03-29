#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "Mesh.h"



class Sphere: public Entity
{
public:
	bool isLight;
	float currentAngle;
	float speedSatelite;
	std::vector<class Model*> m_satellites;
	std::vector<class Sphere*> s_satellites;
	std::vector<class TextureDefault*> textures;
	float radiusSatelite;
	float selfRotationSpeed;
	Sphere* owner;
	Sphere(int precision, Renderer* renderer, bool isLight = false);
	void Init(int precision);
	~Sphere();
	void Draw(class Shader& shader,glm::mat4x4& viewMat, glm::mat4x4& projMat);
	void SetTexture(std::string& path, std::string type = "texture_diffuse",bool isCube = false);
	void AddSatellite(Sphere* satellite, float speed,float radius = 3);
	void AddSatellite(Model* satellite, float speed,float radius = 3);
	void Update(float time);
	void UpdateSatellites(float time);
	void UpdateSelfRot(float time);
private:
	int numVertices;
	int numIndices;
	
	float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
    VertexArray _va;
	class Renderer* _renderer;
	
};

