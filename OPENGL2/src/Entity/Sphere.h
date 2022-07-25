#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "Mesh.h"

class Sphere:public Entity
{
public:
	float currentAngle;
	float speedSatelite;
	std::vector<class Model*> m_satellites;
	std::vector<class Sphere*> s_satellites;
	float radiusSatelite;
	float selfRotationSpeed;
	Sphere* owner;
	Sphere(int precision, Renderer* renderer);
	void Init(int precision);
	~Sphere();
	void Draw(class Shader& shader);
	void SetTexture(std::string& path);
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
	class VertexArray* _va;
	class TextureDefault* _texture;
	class TextureDefault* _textureSpecular;
	class Renderer* _renderer;
	
};

