#pragma once
#include <list>
#include <random>
#include <ext/matrix_float4x4.hpp>
#include <unordered_map>
#include "VertexArray.h"
#include "Shader.h"
#include "../Entity/Mesh.h"
#include "../TextureDefault.h"
#include "../Entity/Camera.h"
#include "../Entity/Model.h"
#include "../Entity/Sphere.h"
#include "../Entity/Camera.h"

class Renderer
{
public:
	Renderer(class GLFWwindow* window,int windowWidth, int windowHeight);
	~Renderer();
	void SetUpFrameBuffer();
	void DrawModel(float deltaTime);
	void DrawSun(float timeAppStart, float deltaTime);
	void DrawPlanets(float deltaTime);
	void Draw();
	void SetUpBlurBuffers();
	void DrawShadows();
	void SetUpShadowBuffer();
	void Init();
	void LoadSolarSystem();
	class GLFWwindow* GetWindow() { return _window; }
	glm::mat4 GetPerspectiveMatrix()const { return _perspectiveMatrix; }
	glm::mat4 GetOrthographicMatrix()const { return _orthographicMatrix; }
	Camera& GetCamera() { return  _camera; }
	float GetRandomNumber()
	{
		 // or std::default_random_engine e{rd()};
		std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };

		// get random numbers with:
		return dist(e);
		
	}
	
private:
	unsigned int renderBufferObject;
	VertexArray screenQuad;
	unsigned int textureScreen;
	unsigned int frameBuffer;
	unsigned int depthCubemap;
	unsigned int depthMapFBO;
	unsigned int rocksAmount =700;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
	unsigned int blackFBO;
	unsigned int blackBuffer;
	class std::vector<Entity*> transforms;
	std::vector< glm::vec3> lightPos;

	std::vector<Model*> models;
	std::vector<Sphere*> spheres;
	std::vector<glm::mat4> shadowTransforms;
	class Sphere* _skybox;
	class Mesh * _lightMesh;
	class Sphere* _sun;
	class Sphere* _blackHole;
	glm::mat4 _perspectiveMatrix;
	glm::mat4 _orthographicMatrix;
	class GLFWwindow* _window;
	Camera _camera;
	float lastFrameTimeStart;
	std::random_device rand;
	std::mt19937 e{ rand() };
	int _windowHeight;
	int _windowWidth;
	Shader* _basicShader;
	Shader* _instanceShader;
	Shader* _screenShader;
	Shader* _sunShader;
	Shader* _skyBoxShader;
	Shader* _depthShader;
	Shader* _depthInstanceShader;
	Shader* _blurShader;
	Shader* _blackHoleShader;
	Shader* _blackHoleShaderScreen;
};

