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
	void SetUpFrameBuffer();
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
	unsigned int renderBufferObject;
	class VertexArray* screenQuad;
	unsigned int textureScreen;
	unsigned int frameBuffer;
	unsigned int rocksAmount =700;
	class std::vector<class Entity*> transforms;
	
	std::vector<class Mesh*>_meshes;
	std::vector<class Model*> models;
	std::vector<class Sphere*> spheres;
	class Sphere* _skybox;
	class Mesh * _lightMesh;
	class Sphere* _sun;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
	class GLFWwindow* _window;
	class Camera* _camera;
	float lastFrameTimeStart;
	std::random_device rand;
	std::mt19937 e{ rand() };
	class Shader* _basicShader;
	 Shader* _instanceShader;
	 Shader* _screenShader;
	 Shader* _sunShader;
	int _windowHeight;
	int _windowWidth;
};

