#pragma once
#include <ext/matrix_float4x4.hpp>

class Renderer
{
public:
	Renderer(int windowHeight, int windowWidth);
	~Renderer();
	void Draw();
	void Init();
	glm::mat4 GetPerspectiveMatrix()const { return _perspectiveMatrix; }
	glm::mat4 GetOrthographicMatrix()const { return _orthographicMatrix; }

private:
	
private:
	class Mesh * _mesh;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
};

