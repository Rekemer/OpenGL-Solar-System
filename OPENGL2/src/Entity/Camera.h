#pragma once


#include "Entity.h"
class Camera : public Entity
{
public:
	void OnMouseEnter( );
	Camera(class GLFWwindow* _window);
	void ProcessMouseInput();
	void Update();
	void UpdateCursor();
	void ProcessKeyboardInput(float deltaTime);
	glm::mat4 GetViewMatrix() { return _view; }
	void UpdateViewMatrix();
private:

protected:
	class GLFWwindow* _window;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	float lastFrame = 0.f;
	bool firstMouse = true;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 _view;
	float yaw = -90.0f; // because -z is forward 
	float pitch = 0.f; // up and down axis
	float _lastX;
	float _lastY;
	bool isCursorHoveringWindow;
};

