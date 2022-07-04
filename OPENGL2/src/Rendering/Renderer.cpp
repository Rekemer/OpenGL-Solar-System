
#include <glew.h>
#include "Renderer.h"

#include <glfw3.h>
#include <imgui.h>
#include <ext/matrix_clip_space.hpp>

#include "VertexArray.h"
#include "Debuger.h"
#include "Mesh.h"
#include "Shader.h"
#include "../Texture.h"

//#include <glfw3.h>



Renderer::Renderer( int windowWidth, int windowHeight)
{
	auto aspect = (float)windowWidth / (float)windowHeight;
	 _orthographicMatrix= glm::ortho(-1.0f, 1.0f , -1.0f * 1.f/aspect, 1.0f * 1.f/aspect);
	
	 _perspectiveMatrix =  glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
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
void Renderer::UpdateUI()
{
	
}

void Renderer::Init()
{
	const float vertexPositions[] = {
	-0.5, -0.5, -0.5, 0, 0, -1,  0, 0 ,
	0.5, -0.5, -0.5, 0, 0, -1,  1, 0,
	-0.5, 0.5, -0.5, 0, 0, -1,  0, 1,
	0.5, 0.5, -0.5, 0, 0, -1,  1, 1,

	-0.5, 0.5, 0.5, 0, 1, 0,    1, 0 ,
	0.5, 0.5, 0.5, 0, 1, 0,    1, 1,
	-0.5, -0.5, 0.5, 0, 0, 1,  0, 0,
	0.5, -0.5, 0.5, 0, 0, 1,  1, 0,

	-0.5, 0.5, -0.5, 0, 0, -1,   0, 1 ,
	0.5, -0.5, -0.5, 0, 0, -1,  1, 0,
	-0.5, 0.5, -0.5, 0, 1, 0,   1, 0,
	0.5, 0.5, -0.5, 0, 1, 0,   1, 1,

	-0.5, 0.5, 0.5, 0, 1, 0,    0, 0 ,
	-0.5, 0.5, 0.5, 0, 0, 1,   0, 1,
	0.5, 0.5, 0.5, 0, 0, 1,    1, 1,
	-0.5, -0.5, 0.5, 0, 0, 1,  0, 0,

	-0.5, -0.5, 0.5, 0, -1, 0,   0, 0 ,
	0.5, -0.5, 0.5, 0, -1, 0,   0, 1,
	-0.5, -0.5, -0.5, 0, -1, 0, 1, 0,
	0.5, -0.5, -0.5, 0, -1, 0, 1, 1,

	0.5, -0.5, -0.5, 1, 0, 0,    0, 0 ,
	0.5, -0.5, 0.5, 1, 0, 0,    0, 1,
	0.5, 0.5, -0.5, 1, 0, 0,    1, 0,
	0.5, 0.5, 0.5, 1, 0, 0,    1, 1,

	-0.5, -0.5, 0.5, -1, 0, 0,      0, 0 ,
	-0.5, -0.5, -0.5, -1, 0, 0,    0, 1,
	-0.5, 0.5, 0.5, -1, 0, 0,      1, 0,
	-0.5, 0.5, -0.5, -1, 0, 0,    1, 1,
	};
	std::cout << sizeof(vertexPositions);
	
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
	_mesh = new Mesh(this);
	_mesh->Load(vertexPositions, 28, indicies, 36);
	


}
