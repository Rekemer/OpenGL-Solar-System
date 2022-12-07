#include "Application.h"
#include <glew.h>
#include <glfw3.h>
#include <SDL.h>
#include "Rendering\Renderer.h"
#include "Input\Input.h"
#include <glfw3.h>
#include <iostream>




Application::Application(int windowHeight, int windowWidth, bool* mod) : _isRunning(true),
_windowHeight(windowHeight),
_windowWidth(windowWidth), mod{mod}
{
   
}

void Application::UpdateShaders()
{
    _renderer->ReloadShaders();
}
Application::~Application()
{
    delete _renderer;
    delete _input;
    glfwTerminate();
}

void Application::Run()
{
    while (!glfwWindowShouldClose(_window1))
    {
        if (*mod) {
            *mod = false;
            _renderer->ReloadShaders();
        }
        _renderer->Draw();
        glfwPollEvents();
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
    _window1 = glfwCreateWindow(_windowWidth, _windowHeight, "OPENGL", NULL, NULL);
    _window2 = glfwCreateWindow(_windowWidth/2, _windowHeight/2, "OPENGL Parametrs", NULL, NULL);
    glfwSetWindowPos(_window1, 10,100);
    glfwSetWindowPos(_window2, _windowWidth + 100, _windowHeight / 2);
    if (!_window1 || !_window2)
    {
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Make the window's context current */
    glfwMakeContextCurrent(_window1);
    glfwSwapInterval(0);
    glfwSetKeyCallback(_window1, ProcessInput);
    glfwSetKeyCallback(_window2, ProcessInput);
  //  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
  


    _renderer = new Renderer(_window1,_window2,_windowWidth, _windowHeight);
    _input = new Input();
    _renderer->Init();
    
	return true;
}



