#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "Mesh.h"

class Sphere:public Entity
{
public:
	Sphere(int precision, Renderer* renderer);
	void Init(int precision);
	~Sphere();
	void Draw(class Shader& shader);
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

