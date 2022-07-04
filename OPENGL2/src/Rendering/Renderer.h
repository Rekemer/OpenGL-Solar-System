#pragma once
#include <ext/matrix_float4x4.hpp>

class Renderer
{
public:
	Renderer( int windowWidth, int windowHeight);
	~Renderer();
	void Draw();
	void UpdateUI();
	void Init();
	glm::mat4 GetPerspectiveMatrix()const { return _perspectiveMatrix; }
	glm::mat4 GetOrthographicMatrix()const { return _orthographicMatrix; }

private:
	
private:
	class Mesh * _mesh;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
};

