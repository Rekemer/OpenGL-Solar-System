#pragma once


class Application
{
public:
	Application();
	~Application();
	void  Run();
	void SetIsRunning(bool state) { _isRunning = state; }
	bool Init();
private:

	class GLFWwindow* _window;
	class Input* _input;
	class Renderer* _renderer;
	bool _isRunning;
};

