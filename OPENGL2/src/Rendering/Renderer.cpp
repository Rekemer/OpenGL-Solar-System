
#include <glew.h>
#include "Renderer.h"

#include <glfw3.h>
#include <ext/matrix_clip_space.hpp>

#include "VertexArray.h"
#include "Debuger.h"
#include "../Entity/Mesh.h"
#include "Shader.h"
#include "../Texture.h"
#include "../Entity/Camera.h"

//#include <glfw3.h>



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

void Renderer::Draw()
{
	glEnable(GL_DEPTH_TEST);
	GLCall(glClearColor(0.3f, 0.3f, 0.3f,1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	static int a =0;
	_camera->Update();
	_mesh->ComputeWorldTransform();

	float time = (float)glfwGetTime();
	_mesh->Bind();

	_mesh->GetShader()->SetVectorUniform("material.ambient", glm::vec3(0.2, 0.8, 0.5));
	_mesh->GetShader()->SetVectorUniform("material.diffuse", glm::vec3(0.2, 0.8, 0.5));
	_mesh->GetShader()->SetVectorUniform("material.specular", glm::vec3(0.2, 0.8, 0.5));
	_mesh->GetShader()->SetFloatUniform("material.shininess", 32.0f);

	_mesh->GetShader()->SetVectorUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	_mesh->GetShader()->SetVectorUniform("light.diffuse",glm::vec3( 0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
	_mesh->GetShader()->SetVectorUniform("light.specular",glm::vec3( 1.0f, 1.0f, 1.0f));
	_mesh->GetShader()->SetVectorUniform("light.position", _lightMesh->GetPosition());
	_mesh->GetShader()->SetVectorUniform("cameraPos", _camera->GetPosition());


	// Draw
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36););
	_lightMesh->SetPosition(glm::vec3(cos(time) *4.f, 2.f, sin(time) * 4.f));
	_lightMesh->ComputeWorldTransform();
	_lightMesh->Bind();
	auto lightShader = _lightMesh->GetShader();
	
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36););
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
	std::cout << sizeof(vertexBuffer);
	
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
	// just object
	_mesh = new Mesh(this);
	_mesh->Load(vertexBuffer, 36, indicies, 26);
	_mesh->LoadShader("Shaders/basic.vert", "Shaders/basic.frag");
	_mesh->LoadTexture("res/Morgana.jpg");
	//camera
	_camera = new Camera(_window);
	_camera->SetPosition(glm::vec3(0.f, 0.f, 5.f));
	// light source
	_lightMesh = new Mesh(this);
	_lightMesh->Load(vertexBuffer, 36, indicies, 26);
	_lightMesh->LoadShader("Shaders/light.vert", "Shaders/light.frag");
	_lightMesh->SetPosition(glm::vec3(4.f, 2.f, 4.f));

}
