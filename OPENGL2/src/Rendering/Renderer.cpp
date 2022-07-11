
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
glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  1.2f, 8.0f),
		glm::vec3(2.3f * 2, 3.3f, 6.0f),
		glm::vec3(5.0f,  2.0f, 6.0f),
		glm::vec3(0.0f,  1.2f, 8.0f)
};
void Renderer::Draw()
{
	glEnable(GL_DEPTH_TEST);
	GLCall(glClearColor(0.3f, 0.3f, 0.3f,1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	static int a =0;
	_camera->Update();
	

	float time = (float)glfwGetTime();
	for (auto mesh : _meshes)
	{
		mesh->ComputeWorldTransform();
		mesh->Bind();
		//_mesh->GetShader()->SetVectorUniform("material.ambient", glm::vec3(0.2, 0.8, 0.5));
		//_mesh->GetShader()->SetVectorUniform("material.diffuse", glm::vec3(0.2, 0.8, 0.5));
		mesh->GetShader()->SetVectorUniform("material.specular", glm::vec3(0.8, 0.8, 0.8));
		mesh->GetShader()->SetFloatUniform("material.shininess", 32.0f);

		mesh->GetShader()->SetVectorUniform("dirLight.ambient", glm::vec3(0.2, 0.2, 0.2));
		mesh->GetShader()->SetVectorUniform("dirLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f)); // darken diffuse light a bit
		mesh->GetShader()->SetVectorUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//mesh->GetShader()->SetVectorUniform("light.position", _lightMesh->GetPosition());

		auto dir = glm::vec4(-0.5f, 1.0f, -1.0f, 1.0f) ;
	//	std::cout << dir.x << " " << dir.y << " " << dir.z << "\n";
		mesh->GetShader()->SetVectorUniform("dirLight.direction", (glm::vec3)dir);
		//mesh->GetShader()->SetFloatUniform("light.cutOff", glm::cos(glm::radians(12.5f)));
		//mesh->GetShader()->SetVectorUniform("cameraPos", _camera->GetPosition());

		 // point light 1
		mesh->GetShader()->SetVectorUniform("pointLights[0].position", pointLightPositions[0]);
		mesh->GetShader()->SetVectorUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		mesh->GetShader()->SetVectorUniform("pointLights[0].diffuse", 0.0f, 0.8f, 0.0f);
		//mesh->GetShader()->SetVectorUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[0].constant", 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[0].linear", 0.09f);
		mesh->GetShader()->SetFloatUniform("pointLights[0].quadratic", 0.032f);
		//mesh->GetShader()->2
		mesh->GetShader()->SetVectorUniform("pointLights[1].position", pointLightPositions[1]);
		mesh->GetShader()->SetVectorUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		mesh->GetShader()->SetVectorUniform("pointLights[1].diffuse", 0.0f, 0.8f, 0.0f);
		//mesh->GetShader()->SetVectorUniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[1].constant", 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[1].linear", 0.09f);
		mesh->GetShader()->SetFloatUniform("pointLights[1].quadratic", 0.032f);
		//mesh->GetShader()->3
		mesh->GetShader()->SetVectorUniform("pointLights[2].position", pointLightPositions[2]);
		mesh->GetShader()->SetVectorUniform("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		mesh->GetShader()->SetVectorUniform("pointLights[2].diffuse", 0.0f, 0.8f, 0.0f);
		//mesh->GetShader()->SetVectorUniform("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[2].constant", 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[2].linear", 0.09f);
		mesh->GetShader()->SetFloatUniform("pointLights[2].quadratic", 0.032f);
		//mesh->GetShader()->4
		mesh->GetShader()->SetVectorUniform("pointLights[3].position", pointLightPositions[3]);
		mesh->GetShader()->SetVectorUniform("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		mesh->GetShader()->SetVectorUniform("pointLights[3].diffuse", 0.0f, 0.8f, 0.0f);
		//mesh->GetShader()->SetVectorUniform("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[3].constant", 1.0f);
		mesh->GetShader()->SetFloatUniform("pointLights[3].linear", 0.09f);
		mesh->GetShader()->SetFloatUniform("pointLights[3].quadratic", 0.032f);

		auto front = glm::vec4(0.0f, 0.0f, -1.f,1.0f) * _camera->GetViewMatrix();
		mesh->GetShader()->SetVectorUniform("spotLight.position", _camera->GetPosition());
		mesh->GetShader()->SetVectorUniform("spotLight.direction", (glm::vec3)front);
		mesh->GetShader()->SetVectorUniform("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		mesh->GetShader()->SetVectorUniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetVectorUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
		mesh->GetShader()->SetFloatUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));

		// Draw
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36););
	}
	


	
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
	// just objects
	for (int i= 0; i != 5; i++)
	{
		for (int j = 5; j!= 0; j--)
		{
			auto _mesh = new Mesh(this);
			_mesh->Load(vertexBuffer, 36, indicies, 26);
			_mesh->LoadShader("Shaders/basic.vert", "Shaders/basic.frag");
			_mesh->LoadTexture("res/Morgana.jpg");
			_mesh->SetPosition(glm::vec3(i *2, 0, j * 2));
			_meshes.emplace_back(_mesh);
			std::cout << i << " " << j << "\n";
		}
		
		
	}
	
	//camera
	_camera = new Camera(_window);
	_camera->SetPosition(glm::vec3(0.f, 0.f, 5.f));
	// light source
	_lightMesh = new Mesh(this);
	_lightMesh->Load(vertexBuffer, 36, indicies, 26);
	_lightMesh->LoadShader("Shaders/light.vert", "Shaders/light.frag");
	_lightMesh->SetPosition(glm::vec3(4.f, 2.f, 4.f));

}
