
#include <glew.h>
#include "Renderer.h"

#include <glfw3.h>
#include <ext/matrix_clip_space.hpp>

#include "VertexArray.h"
#include "Debuger.h"
#include "Mesh.h"
#include "Shader.h"
#include "../Texture.h"

//#include <glfw3.h>



Renderer::Renderer( int windowWidth, int windowHeight)
{
	 _orthographicMatrix= glm::ortho(-windowWidth/2.0f, windowWidth / 2.0f, -windowHeight / 2.0f , windowHeight / 2.0f);
	 _perspectiveMatrix =  glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
}

Renderer::~Renderer()
{
}

void Renderer::Draw()
{
	GLCall(glClearColor(0.3f, 0.3f, 0.3f,1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	static int a =0;
	_mesh->ComputeWorldTransform();
	_mesh->Bind();
	// Draw
	GLCall(glDrawElements(GL_TRIANGLES, _mesh->GetNumIndices(),
		GL_UNSIGNED_INT, nullptr));
}

void Renderer::Init()
{
	const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,0.0f,//0
	0.75f, -0.75f, 0.0f, 1.0f,1.0f, // 1
	-0.75f, -0.75f, 0.0f, 0.0f,1.0f, // 2
	-0.75f, 0.75f, 0.0f , 0.0f,0.0f// 3
	};
	
	const int indicies[] = {
		0,1,2,
		0,3,2
	};
	//glm::mat4 viewMatrix = 
	_mesh = new Mesh(this);
	_mesh->Load(vertexPositions, 4, indicies, 6);
	


}
