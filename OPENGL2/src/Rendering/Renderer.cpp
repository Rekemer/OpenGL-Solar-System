
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
	
	auto pos = _camera->GetPosition();
	auto iter = models.begin();
	

	//_instanceShader->Bind();
	//_instanceShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	

	//PrintVec(transforms[0]->GetPosition());
	

	_basicShader->Bind();
	_basicShader->SetVectorUniform("cameraPos", _camera->GetPosition());

	models[0]->Update(deltaTime, transforms );
	models[0]->DrawInstance(*_basicShader, transforms );

	


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
	_camera->SetPosition(glm::vec3(0.f, 10.f,0.f));
	_basicShader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");
	_instanceShader = new Shader("Shaders/instance.vert", "Shaders/instance.frag");
	LoadSolarSystem();

}



void Renderer::LoadSolarSystem()
{
	// mercury
	//venus
	//Earth
	// Mars
	// Jupiter
	// Saturn
	// Uranus
	//Neptune


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


	auto mercury = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/mercury.jpg";
	mercury->SetTexture(path);
	spheres.emplace_back(mercury);
	mercury->SetPosition(0, 0, 0);



	auto venus = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/venus.jpg";
	venus->SetTexture(path);
	spheres.emplace_back(venus);
	venus->SetPosition(0, 0, 0);


	auto earth = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/Earth/earth_night.jpg";
	earth->SetTexture(path);
	spheres.emplace_back(earth);
	earth->SetPosition(0,0,0);

	auto moon = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/moon.jpg";
	moon->SetTexture(path);
	spheres.emplace_back(moon);
	moon->SetScale(0.5f, 0.5f, 0.5f);
	moon->SetPosition(0, 0, 0);

	auto mars = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/mars.jpg";
	mars->SetTexture(path);
	spheres.emplace_back(mars);
	mars->SetPosition(0, 0, 0);

	auto jupiter = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/jupiter.jpg";
	jupiter->SetTexture(path);
	spheres.emplace_back(jupiter);
	jupiter->SetPosition(0, 0, 0);

	auto saturn = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/saturn.jpg";
	saturn->SetTexture(path);
	spheres.emplace_back(saturn);
	saturn->SetPosition(0, 0, 0);

	auto uranus = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/uranus.jpg";
	uranus->SetTexture(path);
	spheres.emplace_back(uranus);
	uranus->SetPosition(0, 0, 0);

	auto neptune = new Sphere(48, this);
	path = "res/Models/Cosmos/Planets/neptune.jpg";
	neptune->SetTexture(path);
	spheres.emplace_back(neptune);
	neptune->SetPosition(0, 0, 0);

	sun->selfRotationSpeed = earth->selfRotationSpeed
	= moon->selfRotationSpeed = 100;
	//sun->AddSatellite(moon, 20,6);
	sun->AddSatellite(mercury, 10 * GetRandomNumber(), 4);
	sun->AddSatellite(venus, 10 * GetRandomNumber(), 6);
	sun->AddSatellite(earth, 10 * GetRandomNumber(),8);
	earth->AddSatellite(moon, 20 * GetRandomNumber(), 4);
	sun->AddSatellite(mars, 10 * GetRandomNumber(),14);
	sun->AddSatellite(jupiter, 10 * GetRandomNumber(),17);
	sun->AddSatellite(saturn, 10 * GetRandomNumber(),20);
	sun->AddSatellite(uranus, 10 * GetRandomNumber(),25);
	sun->AddSatellite(neptune, 10* GetRandomNumber(),29);

	for (int i =0; i<rocksAmount; i++)
	{
		Entity* e = new Entity();
		e->ComputeWorldTransform();
		transforms.push_back(e);
	}
	
	//std::fill(transforms, transforms + rocksAmount, glm::mat4(1.0f));
	path = "res/Models/Cosmos/Rock/rock.obj";
	//path = "res/Models/Backpack/backpack.obj";
	Model* rock = new Model(path, this, transforms,false);
	models.emplace_back(rock);
	rock->SetPosition(9, 0, 0);
	rock->SetScale(0.1f, 0.1f, 0.1f);
	rock->selfRotationSpeed = 100;
	sun->AddSatellite(rock, 10 * GetRandomNumber(),5);

}
