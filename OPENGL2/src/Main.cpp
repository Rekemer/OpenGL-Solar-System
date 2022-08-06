

#include <iostream>

#include "Application.h"

int main(int argc, char* argv[])
{
	int fullA = 1920;
	int fullB = 1080;
	int testA = 480;
	int testb = 640;
	Application* app = new Application(fullB, fullA);
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

