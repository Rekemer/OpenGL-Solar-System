#pragma once

struct GLFWwindow;
class Application
{
public:
	Application(int windowH,int windowW);
	~Application();
	void  Run();
	void SetIsRunning(bool state) { _isRunning = state; }
	bool Init();
private:

	GLFWwindow* _window1;
	GLFWwindow* _window2;
	int _windowHeight;
	int _windowWidth;
	class Input* _input;
	class Renderer* _renderer;
	bool _isRunning;
};

