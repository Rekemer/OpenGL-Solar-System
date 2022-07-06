#include "Application.h"
#include <glew.h>
#include <glfw3.h>
#include <SDL.h>
#include "Rendering\Renderer.h"
#include "Input\Input.h"
#include <glfw3.h>
#include <iostream>


Application::Application(int windowHeight, int windowWidth ) : _isRunning(true),
_windowHeight(windowHeight),
_windowWidth(windowWidth)
{
  
}


Application::~Application()
{
}

void Application::Run()
{
    while (!glfwWindowShouldClose(_window))
    {
        _renderer->Draw();
        glfwPollEvents();
        glfwSwapBuffers(_window);
    }
	
	
	
}
static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int nods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

bool Application::Init()
{

   

    /* Create a windowed mode window and its OpenGL context */
    if (!glfwInit())
        return -1;
    _window = glfwCreateWindow(_windowWidth, _windowHeight, "OPENGL", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(2);
    glfwSetKeyCallback(_window, ProcessInput);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW");
        return false;
    }
   
    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();
  


    _renderer = new Renderer(_window,_windowWidth, _windowHeight);
    _input = new Input();
    _renderer->Init();

	return true;
}



