

#include <iostream>

#include "Application.h"

int main(int argc, char* argv[])
{
	Application* app = new Application();
	if (app->Init())
	{
		app->Run();
	}
	else
	{
		std::cout << " Failed to initialize app" << "/n";
	}
	return 0;
}

