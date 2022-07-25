
#include <glew.h>
#include "Renderer.h"

 const glm::vec3 nullVec = glm::vec3(0.f, 0.f, 0.f);

#include <glfw3.h>
#include <ext/matrix_clip_space.hpp>

#include "VertexArray.h"
#include "Debuger.h"
#include "../Entity/Mesh.h"
#include "Shader.h"
#include "../TextureDefault.h"
#include "../Entity/Camera.h"
#include "../Entity/Model.h"

//#include <glfw3.h>

#include <string>
#include <cstdlib>
#include "../Entity/Sphere.h"



Renderer::Renderer( GLFWwindow* window, int windowWidth, int windowHeight)
{
	auto aspect = (float)windowWidth / (float)windowHeight;
	 _orthographicMatrix= glm::ortho(-1.0f, 1.0f , -1.0f * 1.f/aspect, 1.0f * 1.f/aspect);
	 _perspectiveMatrix =  glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	 _window = window;
}

Renderer::~Renderer()
{
}
glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  1.2f, 8.0f),
		glm::vec3(2.3f * 2, 3.3f, 6.0f),
		glm::vec3(5.0f,  2.0f, 6.0f),
		glm::vec3(0.0f,  1.2f, 8.0f)
};

void PrintVec(glm::vec3 pos)
{
	std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
}
void Renderer::Draw()
{
	glEnable(GL_DEPTH_TEST);
	GLCall(glClearColor(135.f/225.f, 128 / 225.f, 126 / 225.f,1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	float timeAppStart = (float)glfwGetTime();
	float deltaTime = (timeAppStart - lastFrameTimeStart) / 1000.0f;
	lastFrameTimeStart = timeAppStart;
	deltaTime = 0.001f;
	static int a =0;
	_camera->Update();
	_basicShader->Bind();
	_basicShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	auto pos = _camera->GetPosition();
	auto iter = models.begin();
	//auto pos = (*iter)->GetPosition();
//	PrintVec(pos);


	PrintVec(spheres[0]->GetPosition());

	for (auto model : models)
	{
		model->Draw(*_basicShader);
	}

	for (auto sphere : spheres)
	{
		sphere->Update(deltaTime);
		sphere->Draw(*_basicShader);
	}
	if (_skybox != nullptr)
	{
		_skybox->SetPosition(_camera->GetPosition());
		_skybox->Draw(*_basicShader);
	}
		

	
	
}


void Renderer::Init()
{
	const float vertexBuffer[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f,0.f,-1.f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.f,0.f,-1.f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f,0.f,-1.f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f,0.f,-1.f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.f,0.f,-1.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f,0.f,-1.f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f,0.f,1.f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f,0.f,1.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f,0.f,1.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f,0.f,1.f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.f,0.f,1.f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.f,0.f,1.f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.f,0.f,0.f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.f,0.f,0.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.f,0.f,0.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.f,0.f,0.f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.f,0.f,0.f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.f,0.f,0.f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.f,0.f,0.f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.f,0.f,0.f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.f,0.f,0.f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.f,0.f,0.f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.f,0.f,0.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.f,0.f,0.f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f, 
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};
	
	
	const int indicies[] = {
		2,1,0 ,
		3,9,8,
		4,11,10	,

		5,11,12	,
		6,14,13	,
		7,14,15	,

		18,17,16,
		19,17,18,
		22,21,20,

		23,21,22,
		26,25,24,
		27,25,26
	};
	//camera
	_camera = new Camera(_window);
	_camera->SetPosition(glm::vec3(0.f, 0.f,20.f));
	_basicShader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");

	LoadSolarSystem();

	//std::string path("res/Models/Backpack/backpack.obj");
	//std::string path("res/Models/Cat/cat.obj");
	//std::string path("res/Models/Cosmos/planet.obj");
	

	//	model->SetPosition(_camera->GetPosition() - glm::vec3(0.f, 0.f, 20.f));
	//	//model->SetScale(glm::vec3(3.0f, 3.0f, 3.0f));
	//	model->ComputeWorldTransform();
	//	std::string path1 = "C:/dev/OPENGL/OPENGL2/OPENGL2/OPENGL2/res/Models/Cosmos/Earth/earth_daymap.jpg";
	//	std::string type = "texture_diffuse";
	//	model->SetTexture(path1,type );
	
	//std::string path1 = "res/Models/Cosmos/Planets/Earth/earth_day.jpg";
	//std::string path1 = "res/Models/Cosmos/Planets/moon.jpg";
	


	
	
	
	//spheres.emplace_back(sphere);

	//_sphere = new Sphere(48,this);
	// just objects
	//for (int i= 0; i != 5; i++)
	//{
	//	for (int j = 5; j!= 0; j--)
	//	{
	//		auto _mesh = new Mesh(this);
	//		_mesh->Load(vertexBuffer, 36, indicies, 26);
	//		_mesh->LoadShader("Shaders/basic.vert", "Shaders/basic.frag");
	//		_mesh->LoadTexture("res/Morgana.jpg");
	//		_mesh->SetPosition(glm::vec3(i *2, 0, j * 2));
	//		_meshes.emplace_back(_mesh);
	//		std::cout << i << " " << j << "\n";
	//	}
	//	
	//	
	//}
	//
	
	//// light source
	//_lightMesh = new Mesh(this);
	//_lightMesh->Load(vertexBuffer, 36, indicies, 26);
	//_lightMesh->LoadShader("Shaders/light.vert", "Shaders/light.frag");
	//_lightMesh->SetPosition(glm::vec3(4.f, 2.f, 4.f));

}

void Renderer::LoadSolarSystem()
{
	// add skybox
	std::string path = "res/Models/Cosmos/Sky/8k_stars_milky_way.jpg";
	_skybox = new Sphere(48, this);
	_skybox->SetTexture(path);
	_skybox->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));

	// add planets

	sun = new Sphere(48, this);
	path = "res/Models/Cosmos/Sun/sun.jpg";
	sun->SetTexture(path);
	spheres.emplace_back(sun);
	sun->SetScale(2.5f, 2.5f,2.5f);
	sun->SetPosition(2, 0, 0);

	auto earth = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/Earth/earth_day.jpg";
	earth->SetTexture(path);
	spheres.emplace_back(earth);
	earth->SetPosition(0,0,0);



	auto moon = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/moon.jpg";
	moon->SetTexture(path);
	spheres.emplace_back(moon);
	moon->SetScale(0.5f, 0.5f, 0.5f);
	moon->SetPosition(0, 0, 0);

	sun->selfRotationSpeed = earth->selfRotationSpeed
	= moon->selfRotationSpeed = 100;
	sun->AddSatellite(moon, 20,6);
	sun->AddSatellite(earth, 20,4);
	earth->AddSatellite(moon, 20, 6);

	path = "res/Models/Cosmos/Rock/rock.obj";
	Model* model = new Model(path, this);
	models.emplace_back(model);
	model->SetPosition(9, 0, 0);
	model->SetScale(0.4f, 0.4f, 0.4f);

}
