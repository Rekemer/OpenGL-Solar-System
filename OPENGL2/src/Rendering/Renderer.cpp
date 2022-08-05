
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
	//// generate buffer
	//GLCall(glGenFramebuffers(1, &frameBuffer));
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));

	//// generate texture for buffer
	//GLCall(glGenTextures(1, &textureScreen));
	//GLCall(glBindTexture(GL_TEXTURE_2D, textureScreen));
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _windowWidth, _windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//// attach it to currently bound framebuffer object
	//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureScreen, 0));


	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

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

void Renderer::DrawModel(float deltaTime)
{
	_instanceShader->SetVectorUniform("cameraPos", _camera->GetPosition());
	_instanceShader->SetVectorUniform("dirLight.direction", 0.0f, -1.0f, 0.0f);
	_instanceShader->SetVectorUniform("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	_instanceShader->SetVectorUniform("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
	_instanceShader->SetVectorUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

	


	_instanceShader->SetVectorUniform("cameraPos", _camera->GetPosition());

	_instanceShader->SetVectorUniform("pointLights[0].position", lightPos[0]);
	_instanceShader->SetVectorUniform("pointLights[0].ambient", 0.3f, 0.3f, 0.3f);
	_instanceShader->SetVectorUniform("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
	_instanceShader->SetVectorUniform("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
	_instanceShader->SetFloatUniform("pointLights[0].constant", 1.0f);
	_instanceShader->SetFloatUniform("pointLights[0].linear", 0.014f);
	_instanceShader->SetFloatUniform("pointLights[0].quadratic", 0.007f);

	_instanceShader->SetVectorUniform("pointLights[1].position", lightPos[1]);
	_instanceShader->SetVectorUniform("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
	_instanceShader->SetVectorUniform("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
	_instanceShader->SetVectorUniform("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
	_instanceShader->SetFloatUniform("pointLights[1].constant", 1.0f);
	_instanceShader->SetFloatUniform("pointLights[1].linear", 0.014f);
	_instanceShader->SetFloatUniform("pointLights[1].quadratic", 0.007f);

	_instanceShader->SetVectorUniform("pointLights[2].position", lightPos[2]);
	_instanceShader->SetVectorUniform("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
	_instanceShader->SetVectorUniform("pointLights[2].diffuse", 0.5f, 0.5f, 0.5f);
	_instanceShader->SetVectorUniform("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
	_instanceShader->SetFloatUniform("pointLights[2].constant", 1.0f);
	_instanceShader->SetFloatUniform("pointLights[2].linear", 0.014f);
	_instanceShader->SetFloatUniform("pointLights[2].quadratic", 0.007f);

	_instanceShader->SetMatrixUniform("projMatrix", GetPerspectiveMatrix());
	// camera/view transformation
	_instanceShader->SetMatrixUniform("viewMatrix", GetCamera()->GetViewMatrix());

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	_instanceShader->setInt("depthMap", 8);


	models[0]->Update(deltaTime);
	models[0]->DrawInstance(*_instanceShader);
}

void Renderer::DrawSun(float timeAppStart, float deltaTime)
{
	_sunShader->Bind();
	_sunShader->SetVectorUniform("cameraPos", _camera->GetPosition());

	_sunShader->SetFloatUniform("time", timeAppStart);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//
	//_sunShader->setInt("material.texture_diffuse1", 2);

	_sun->Update(deltaTime);
	_sun->Draw(*_sunShader);
}

void Renderer::DrawPlanets(float deltaTime)
{
	
	

	_basicShader->SetVectorUniform("dirLight.direction", 0.0f, 0.0f, 1.0f);
	_basicShader->SetVectorUniform("dirLight.ambient", 0.4f, 0.4f, 0.4f);
	_basicShader->SetVectorUniform("dirLight.diffuse", 1.4f, 1.4f, 1.4f);
	_basicShader->SetVectorUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

	


	_basicShader->SetVectorUniform("cameraPos", _camera->GetPosition());

	_basicShader->SetVectorUniform("pointLights[0].position", lightPos[0]);
	_basicShader->SetVectorUniform("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
	_basicShader->SetVectorUniform("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
	_basicShader->SetVectorUniform("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
	_basicShader->SetFloatUniform("pointLights[0].constant", 1.0f);
	_basicShader->SetFloatUniform("pointLights[0].linear", 0.014f);
	_basicShader->SetFloatUniform("pointLights[0].quadratic", 0.007f);

	_basicShader->SetVectorUniform("pointLights[1].position", lightPos[1]);
	_basicShader->SetVectorUniform("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
	_basicShader->SetVectorUniform("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
	_basicShader->SetVectorUniform("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
	_basicShader->SetFloatUniform("pointLights[1].constant", 1.0f);
	_basicShader->SetFloatUniform("pointLights[1].linear", 0.014f);
	_basicShader->SetFloatUniform("pointLights[1].quadratic", 0.007f);

	_basicShader->SetVectorUniform("pointLights[2].position", lightPos[2]);
	_basicShader->SetVectorUniform("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
	_basicShader->SetVectorUniform("pointLights[2].diffuse", 0.5f, 0.5f, 0.5f);
	_basicShader->SetVectorUniform("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
	_basicShader->SetFloatUniform("pointLights[2].constant", 1.0f);
	_basicShader->SetFloatUniform("pointLights[2].linear", 0.014f);
	_basicShader->SetFloatUniform("pointLights[2].quadratic", 0.007f);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	_basicShader->setInt("depthMap", 8);
	//_basicShader->SetFloatUniform("time", timeAppStart);

	for (auto sphere : spheres)
	{
		sphere->Update(deltaTime);
		sphere->Draw(*_basicShader);
	}
}

void Renderer::Draw()
{
	
	DrawShadows();

	GLCall(glViewport(0, 0, _windowWidth, _windowHeight));
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
	

	
	auto lightScale = _sun->GetScale();
	auto pos = _camera->GetPosition();
	auto iter = models.begin();
	

	  

	_instanceShader->Bind();
	DrawModel(deltaTime);



	

	//PrintVec(transforms[0]->GetPosition());


	DrawSun(timeAppStart, deltaTime);

	
	
	
	_basicShader->Bind();
	DrawPlanets(deltaTime);


	if (_skybox != nullptr){

		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		_skyBoxShader->Bind();
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//_skyBoxShader->setInt("material.texture_diffuse1", 1);

		_skybox->SetPosition(_camera->GetPosition());
		_skybox->Draw(*_skyBoxShader);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	
	glDisable(GL_DEPTH_TEST);

	bool horizontal = true, first_iteration = true;
	int amount = 10;
	_blurShader->Bind();
	_blurShader->setInt("image", 0);
	glActiveTexture(GL_TEXTURE0);
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		_blurShader->setInt("horizontal", horizontal);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]
		);
		// render quad
		screenQuad->Bind();
		glDrawElements(GL_TRIANGLES, screenQuad->GetNumIndices(), GL_UNSIGNED_INT, 0);
		
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	_screenShader->Bind();
	_screenShader->setInt("scene", 0);
	_screenShader->setInt("bloomBlur", 1);
	_screenShader->SetFloatUniform("exposure", 1.0);
	 // back to defaul)t
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
	screenQuad->Bind();
	glDrawElements(GL_TRIANGLES, screenQuad->GetNumIndices(), GL_UNSIGNED_INT, 0);
	
}
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


void Renderer::SetUpBlurBuffers()
{
	
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}

void Renderer::DrawShadows()
{
	GLCall(glEnable(GL_DEPTH_TEST));
	

	GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));


	// render scene

	_depthInstanceShader->Bind();

	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string a = "shadowMatrices[" + std::to_string(i) + "]";
		_depthInstanceShader->SetMatrixUniform(a.c_str(), shadowTransforms[i]);
	}
	_depthInstanceShader->SetFloatUniform("far_plane", 25);
	_depthInstanceShader->SetVectorUniform("lightPos", lightPos[0]);
	models[0]->DrawInstance(*_depthInstanceShader);


	_depthShader->Bind();

	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string a = "shadowMatrices[" + std::to_string(i) + "]";

		_depthShader->SetMatrixUniform(a.c_str(), shadowTransforms[i]);
	}

	_depthShader->SetFloatUniform("far_plane", 25);
	_depthShader->SetVectorUniform("lightPos", lightPos[0]);

	for (auto sphere : spheres)
	{
		//sphere->Update(deltaTime);
		sphere->Draw(*_depthShader);
	}

}

void Renderer::SetUpShadowBuffer()
{
	glGenTextures(1, &depthCubemap);
	glGenFramebuffers(1, &depthMapFBO);

	
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//GLint default_draw_fbo_;
	//glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &default_draw_fbo_);
	//GLint default_read_fbo_;
	//glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &default_read_fbo_);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//
	//std::cout << depthMapFBO << "\n";
	//std::cout << default_draw_fbo_ << "\n";
	//std::cout << default_read_fbo_ << "\n";

	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos[0], lightPos[0] + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

}

void Renderer::Init()
{
	
	SetUpFrameBuffer();
	SetUpBlurBuffers();
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
	_skyBoxShader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
	_depthShader = new Shader("Shaders/depth.vert", "Shaders/depth.frag", "Shaders/depth.geom");
	_depthInstanceShader = new Shader("Shaders/depth_instance.vert", "Shaders/depth.frag", "Shaders/depth.geom");
	_blurShader = new Shader("Shaders/blur.vert", "Shaders/blur.frag");

	LoadSolarSystem();

	SetUpShadowBuffer();
	

	
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

	_sun = new Sphere(48*2, this,true);
	path = "res/Models/Cosmos/Sun/sun.jpg";
	_sun->SetTexture(path);
	path = "res/Textures/uv_distortion.png";
//	path = "res/Textures/uv_texture.png";
	_sun->SetTexture(path);
	//spheres.emplace_back(_sun);
	_sun->SetScale(2.5f, 2.5f,2.5f);
	_sun->SetPosition(2, 0, 0);


	lightPos.emplace_back(_sun->GetPosition());
	lightPos.emplace_back(lightPos[0] + glm::vec3(0, 1, 0)*2.5f);
	lightPos.emplace_back(lightPos[0] - glm::vec3(0, 1, 0)*2.5f);
	


	auto mercury = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/mercury.jpg";
	mercury->SetTexture(path);
	path = "res/Textures/Normal/mercury.png";
	mercury->SetTexture(path);
	spheres.emplace_back(mercury);
	mercury->SetPosition(0, 0, 0);



	auto venus = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/venus.jpg";
	venus->SetTexture(path);
	path = "res/Textures/Normal/venus.png";
	venus->SetTexture(path);
	spheres.emplace_back(venus);
	venus->SetPosition(0, 0, 0);


	auto earth = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/Earth/earth_day.jpg";
	earth->SetTexture(path);
	path = "res/Textures/Normal/normal_earth_day.png";
	earth->SetTexture(path);
	spheres.emplace_back(earth);
	earth->SetPosition(0,0,0);
	//earth->SetRotation(90, 0, 0);
	auto moon = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/moon.jpg";
	moon->SetTexture(path);
	path = "res/Textures/Normal/normal_moon.png";
	moon->SetTexture(path);
	spheres.emplace_back(moon);
	moon->SetScale(0.5f, 0.5f, 0.5f);
	moon->SetPosition(0, 0, 0);

	auto mars = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/mars.jpg";
	mars->SetTexture(path);
	path = "res/Textures/Normal/mars.png";
	mars->SetTexture(path);
	spheres.emplace_back(mars);
	mars->SetPosition(0, 0, 0);

	auto jupiter = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/jupiter.jpg";
	jupiter->SetTexture(path);
	path = "res/Textures/Normal/jupiter.png";
	jupiter->SetTexture(path);
	spheres.emplace_back(jupiter);
	jupiter->SetPosition(0, 0, 0);

	auto saturn = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/saturn.jpg";
	saturn->SetTexture(path);
	path = "res/Textures/Normal/saturn.png";
	saturn->SetTexture(path);
	spheres.emplace_back(saturn);
	saturn->SetPosition(0, 0, 0);

	auto uranus = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/uranus.jpg";
	uranus->SetTexture(path);
	path = "res/Textures/Normal/uranus.png";
	uranus->SetTexture(path);
	spheres.emplace_back(uranus);
	uranus->SetPosition(0, 0, 0);

	auto neptune = new Sphere(48 * 2, this);
	path = "res/Models/Cosmos/Planets/neptune.jpg";
	neptune->SetTexture(path);
	path = "res/Textures/Normal/neptune.png";
	neptune->SetTexture(path);
	spheres.emplace_back(neptune);
	neptune->SetPosition(0, 0, 0);

	earth->selfRotationSpeed= 
		 moon->selfRotationSpeed = 
		 mercury->selfRotationSpeed=
		 venus->selfRotationSpeed=
		 mars->selfRotationSpeed= jupiter->selfRotationSpeed= saturn->selfRotationSpeed=
		 uranus->selfRotationSpeed=
		 neptune->selfRotationSpeed=100*2;
	//sun->AddSatellite(moon, 20,6);
	 float minSpeed = 1;
	_sun->AddSatellite(mercury, 10 * GetRandomNumber()+minSpeed, 4);
	_sun->AddSatellite(venus, 10 * GetRandomNumber()+minSpeed, 6);
	_sun->AddSatellite(earth, 10 * GetRandomNumber()+minSpeed,8);
	earth->AddSatellite(moon, 10 * GetRandomNumber()+minSpeed, 4);
	_sun->AddSatellite(mars, 10 * GetRandomNumber()+minSpeed,14);
	_sun->AddSatellite(jupiter, 10 * GetRandomNumber()+minSpeed,17);
	_sun->AddSatellite(saturn, 10 * GetRandomNumber() + minSpeed, 20);
	_sun->AddSatellite(uranus, 10 * GetRandomNumber()+minSpeed,25);
	_sun->AddSatellite(neptune, 10* GetRandomNumber()+minSpeed,29);



	//std::fill(transforms, transforms + rocksAmount, glm::mat4(1.0f));
	path = "res/Models/Cosmos/Rock/rock.obj";
	//path = "res/Models/Backpack/backpack.obj";
	Model* rock = new Model(path, this, rocksAmount,false);
	models.emplace_back(rock);
	rock->SetPosition(9, 0, 0);
	
	rock->selfRotationSpeed = 100;
	_sun->AddSatellite(rock, 10 * GetRandomNumber(),12);

}
