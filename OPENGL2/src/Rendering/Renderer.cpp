
#include <glew.h>
#include "Renderer.h"

#include <glfw3.h>

#include "VertexArray.h"
#include "Debuger.h"
#include "Shader.h"
#include "../Texture.h"

//#include <glfw3.h>



Renderer::Renderer()  
{

}

Renderer::~Renderer()
{
}

void Renderer::Draw()
{
	GLCall(glClearColor(0.3f, 0.3f, 0.3f,1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	static int a =0;
	_va->Bind();
	_texture->SetActive();
	_shader->SetActive();
	// Draw
	GLCall(glDrawElements(GL_TRIANGLES, _va->GetNumIndices(),
		GL_UNSIGNED_INT, nullptr));
	
	// clean up
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
	_va = new VertexArray(vertexPositions,4,indicies,6);
	_texture = new Texture();
	_shader = new Shader();
	if (!_shader->Load("Shaders/basic.vert", "Shaders/basic.frag"))
	{
		std::cout<< "failed to load shaders" << "\n";
	}
	_texture->Load("res/Wall.png");


}
