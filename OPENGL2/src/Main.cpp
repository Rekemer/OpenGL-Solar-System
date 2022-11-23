

#include <iostream>

#include "Application.h"

int main(int argc, char* argv[])
{
	int fullA = 1920;
	int fullB = 1080;
	int testA = 16*80;
	int testb = 9*80;
	Application* app = new Application(testb,testA);
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

