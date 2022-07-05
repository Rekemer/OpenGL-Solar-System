#pragma once
#include "Entity.h"
class Camera : Entity
{
public:
	Camera(class GLFWwindow* _window);
	void Update();
	void ProcessInput();
	glm::mat4 GetViewMatrix() { return _view; }
	void UpdateViewMatrix();
private:
protected:
	class GLFWwindow* _window;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 _view;
};

