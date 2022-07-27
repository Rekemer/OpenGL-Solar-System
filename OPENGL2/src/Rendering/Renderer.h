#pragma once
#include <list>
#include <random>
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
	float GetRandomNumber()
	{
		 // or std::default_random_engine e{rd()};
		std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };

		// get random numbers with:
		return dist(e);
		
	}
	
private:
	unsigned int rocksAmount =50;
	class std::vector<class Entity*> transforms;
	class Shader* _basicShader;
	std::vector<class Mesh*>_meshes;
	std::vector<class Model*> models;
	std::vector<class Sphere*> spheres;
	class Sphere* _skybox;
	class Mesh * _lightMesh;
	class Sphere* sun;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
	class GLFWwindow* _window;
	class Camera* _camera;
	float lastFrameTimeStart;
	std::random_device rand;
	std::mt19937 e{ rand() };
	Shader* _instanceShader;
};

