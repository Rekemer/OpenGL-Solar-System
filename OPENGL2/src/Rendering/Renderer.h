#pragma once
#include <ext/matrix_float4x4.hpp>

class Renderer
{
public:
	Renderer(class GLFWwindow* window,int windowWidth, int windowHeight);
	~Renderer();
	void Draw();
	void Init();
	class GLFWwindow* GetWindow() { return _window; }
	glm::mat4 GetPerspectiveMatrix()const { return _perspectiveMatrix; }
	glm::mat4 GetOrthographicMatrix()const { return _orthographicMatrix; }
	class Camera* GetCamera() { return  _camera; }

private:
	
private:
	class Mesh * _mesh;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
	class GLFWwindow* _window;
	class Camera* _camera;
};

