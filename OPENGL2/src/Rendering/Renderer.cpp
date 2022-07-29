
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
	_windowHeight = windowHeight;
	_windowWidth = windowWidth;
	auto aspect = (float)windowWidth / (float)windowHeight;
	 _orthographicMatrix= glm::ortho(-1.0f, 1.0f , -1.0f * 1.f/aspect, 1.0f * 1.f/aspect);
	 _perspectiveMatrix =  glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	 _window = window;
}

Renderer::~Renderer()
{
}

void Renderer::SetUpFrameBuffer()
{
	// generate buffer
	GLCall(glGenFramebuffers(1, &frameBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));

	// generate texture for buffer
	GLCall(glGenTextures(1, &textureScreen));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureScreen));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _windowWidth, _windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// attach it to currently bound framebuffer object
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureScreen, 0));


	GLCall(glGenRenderbuffers(1, &renderBufferObject));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
	GLCall(glEnable(GL_DEPTH_TEST));
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
	

	_instanceShader->Bind();
	_instanceShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	models[0]->Update(deltaTime);
	models[0]->DrawInstance(*_instanceShader);

	//PrintVec(transforms[0]->GetPosition());


	_sunShader->Bind();
	_sunShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	_sunShader->SetFloatUniform("time", timeAppStart);
	
	_sun->Update(deltaTime);
	_sun->Draw(*_sunShader);


	_basicShader->Bind();
	_basicShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	//_basicShader->SetFloatUniform("time", timeAppStart);

	

	
	//PrintVec(spheres[1]->GetPosition());
	//PrintVec(models[0]->transforms[0]->GetPosition());
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


	_screenShader->Bind();
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // back to defaul)t
	GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	screenQuad->Bind();
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureScreen);
	glDrawElements(GL_TRIANGLES, screenQuad->GetNumIndices(),GL_UNSIGNED_INT, 0);
	
}


void Renderer::Init()
{
	SetUpFrameBuffer();

	std::vector<Vertex> verticesScreen = {
	 Vertex(-1.0f, -1.0f, 0.0f,		0.f,0.f),
	 Vertex(1.0f, -1.0f, 0.0f,		1.f,0.f),
	 Vertex(-1.0f,  1.0f, 0.0f,	0.f,1.f),
	 Vertex(1.0f,  1.0f, 0.0f,		1.f,1.f)
	};
	std::vector<unsigned int> indiciesScreen = {
		0,1,2,
		1,3,2
	};

	screenQuad = new VertexArray(verticesScreen, indiciesScreen);


	//camera
	_camera = new Camera(_window);
	_camera->SetPosition(glm::vec3(0.f, 10.f,0.f));
	_basicShader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");
	_instanceShader = new Shader("Shaders/instance.vert", "Shaders/instance.frag");
	_screenShader = new Shader("Shaders/screen.vert", "Shaders/screen.frag");
	_sunShader = new Shader("Shaders/sun.vert", "Shaders/sun.frag");

	LoadSolarSystem();


	// generate quad

	
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

	_sun = new Sphere(48, this,true);
	path = "res/Models/Cosmos/Sun/sun.jpg";
	_sun->SetTexture(path);
	path = "res/Textures/perlin_noise.png";
	_sun->SetTexture(path);
	//spheres.emplace_back(_sun);
	_sun->SetScale(2.5f, 2.5f,2.5f);
	_sun->SetPosition(2, 0, 0);


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

	 earth->selfRotationSpeed
	= moon->selfRotationSpeed = 100;
	//sun->AddSatellite(moon, 20,6);
	 float minSpeed = 1;
	_sun->AddSatellite(mercury, 10 * GetRandomNumber()+minSpeed, 4);
	_sun->AddSatellite(venus, 10 * GetRandomNumber()+minSpeed, 6);
	_sun->AddSatellite(earth, 10 * GetRandomNumber()+minSpeed,8);
	earth->AddSatellite(moon, 20 * GetRandomNumber()+minSpeed, 4);
	_sun->AddSatellite(mars, 10 * GetRandomNumber()+minSpeed,14);
	_sun->AddSatellite(jupiter, 10 * GetRandomNumber()+minSpeed,17);
	_sun->AddSatellite(saturn, 10 * GetRandomNumber() + minSpeed, 20);
	_sun->AddSatellite(uranus, 10 * GetRandomNumber()+minSpeed,25);
	_sun->AddSatellite(neptune, 10* GetRandomNumber()+minSpeed,29);

	spheres.emplace_back(mercury);
	spheres.emplace_back(venus);
	spheres.emplace_back(earth);
	spheres.emplace_back(moon);
	spheres.emplace_back(mars);
	spheres.emplace_back(jupiter);
	spheres.emplace_back(saturn);
	spheres.emplace_back(uranus);
	spheres.emplace_back(neptune);
	

	//std::fill(transforms, transforms + rocksAmount, glm::mat4(1.0f));
	path = "res/Models/Cosmos/Rock/rock.obj";
	//path = "res/Models/Backpack/backpack.obj";
	Model* rock = new Model(path, this, rocksAmount,false);
	models.emplace_back(rock);
	rock->SetPosition(9, 0, 0);
	rock->SetScale(0.7f, 0.7, 0.7f);
	rock->selfRotationSpeed = 100;
	_sun->AddSatellite(rock, 10 * GetRandomNumber(),12);

}
