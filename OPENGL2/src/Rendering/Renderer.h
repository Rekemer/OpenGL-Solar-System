#pragma once
#include <list>
#include <ext/matrix_float4x4.hpp>
#include <unordered_map>


class Renderer
{
public:
	Renderer(class GLFWwindow* window,int windowWidth, int windowHeight);
	~Renderer();
	void Draw();
	void Init();
	void LoadSolarSystem();
	class GLFWwindow* GetWindow() { return _window; }
	glm::mat4 GetPerspectiveMatrix()const { return _perspectiveMatrix; }
	glm::mat4 GetOrthographicMatrix()const { return _orthographicMatrix; }
	class Camera* GetCamera() { return  _camera; }

private:
	
private:
	class Shader* _basicShader;
	std::list<class Mesh*>_meshes;
	std::list<class Model*> models;
	std::vector<class Sphere*> spheres;
	class Sphere* _skybox;
	class Mesh * _lightMesh;
	class Sphere* sun;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
	class GLFWwindow* _window;
	class Camera* _camera;
	float lastFrameTimeStart;
};

